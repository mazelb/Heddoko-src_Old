/*
 * Ser.c
 *
 * Created: 9/9/2015 2:56:03 PM
 *  Author: Hriday Mehta
 */ 
/**
 * @file  Serial.c
 * @brief Low level Serial functions
 * @author Heddoko
 * @note possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __UART      - UART  (UART1)  interface
 *                        - disabled  (default)
 *
 * @copy Heddoko (2015)
 */

#include "Ser.h"
#include "BrainMCU.h"
#include <string.h>
#include <stdio.h>

/* Clock Definitions */
#define BAUD(b) ((SystemCoreClock + 8*b)/(16*b))

/** Baudrate setting : 115200 */
#define CONF_BAUDRATE   115200
/** Char setting     : 8-bit character length (don't care for UART) */
#define CONF_CHARLENGTH 0
/** Parity setting   : No parity check */
#define CONF_PARITY     UART_MR_PAR_NO
/** Stopbit setting  : No extra stopbit, i.e., use 1 (don't care for UART) */
#define CONF_STOPBITS   false

#define ALL_INTERRUPT_MASK  0xffffffff

const usart_serial_options_t uart_options =
{
	.baudrate   = CONF_BAUDRATE,
	.charlength = CONF_CHARLENGTH,
	.paritytype = CONF_PARITY,
	.stopbits   = CONF_STOPBITS,
};
#define FIFO_BUFFER_SIZE 255
/*
typedef struct 
{
	char data_buf[FIFO_BUFFER_SIZE]; 
	uint16_t i_first;                   // index of oldest data byte in buffer
	uint16_t i_last;                     // index of newest data byte in buffer
	uint16_t num_bytes;                  // number of bytes currently in buffer
}sw_fifo_typedef;
*/
volatile uint8_t uart_rx_fifo_not_empty_flag; // this flag is automatically set and cleared by the software buffer
volatile uint8_t uart_rx_fifo_full_flag;      // this flag is automatically set and cleared by the software buffer
volatile uint8_t uart_rx_fifo_ovf_flag;       // this flag is not automatically cleared by the software buffer
typedef struct  
{
	uint8_t data_buf[FIFO_BUFFER_SIZE];
	uint16_t i_first;
	uint16_t i_last;
	uint16_t num_bytes; 
}sw_fifo_typedef;
sw_fifo_typedef rx_fifo = { {0}, 0, 0, 0 }; // declare a receive software buffer
/**
 * SerialInit (void)
 * @brief Initialize Serial ports for Q1,Q2 and Q3 115200 baud N81
 *
 */

void SerialInit (void)
{
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

	usart_serial_init(UART1,&uart_options);
	//setup interrupts for the UART
	/* Disable all the interrupts. */
	usart_disable_interrupt(UART1, ALL_INTERRUPT_MASK);	
	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(UART1_IRQn);
	usart_enable_interrupt(UART1, 0x01); //enable RXRDY	
	
	//usart_serial_init(Q1,&uart_options);
	//usart_serial_init(Q2,&uart_options);
	//usart_serial_init(Q3,&uart_options);
}


void UART1_Handler()
{
	
	int32_t bufferVal = 0; 	
	if(rx_fifo.num_bytes == FIFO_BUFFER_SIZE) // if the sw buffer is full
	{      
		uart_rx_fifo_ovf_flag = 1;                     // set the overflow flag
	}
	else if(rx_fifo.num_bytes < FIFO_BUFFER_SIZE)  // if there's room in the sw buffer
	{	 
		 uint32_t val = 0;
		 while((UART1->UART_SR & UART_SR_RXRDY) == 0);
		 usart_getchar(UART1,&val);	 
		 rx_fifo.data_buf[rx_fifo.i_last] = val;
		 rx_fifo.i_last++;                              // increment the index of the most recently added element
		 rx_fifo.num_bytes++;                           // increment the bytes counter
	 }
	 if(rx_fifo.num_bytes == FIFO_BUFFER_SIZE)
	 {      // if sw buffer just filled up
		 uart_rx_fifo_full_flag = 1;                    // set the RX FIFO full flag
	 }
	 if(rx_fifo.i_last == FIFO_BUFFER_SIZE) 
	 {         // if the index has reached the end of the buffer,
		 rx_fifo.i_last = 0;                            // roll over the index counter
	 }
	 uart_rx_fifo_not_empty_flag = 1;                 // set received-data flag	
}

uint8_t uart_get_byte(void) 
{
	//add semaphore here instead of disabling global interrupts 
	Disable_global_interrupt(); 
	uint8_t byte = 0;
	if(rx_fifo.num_bytes == FIFO_BUFFER_SIZE) 
	{ // if the sw buffer is full
		uart_rx_fifo_full_flag = 0;               // clear the buffer full flag because we are about to make room
	}
	if(rx_fifo.num_bytes > 0) 
	{                 // if data exists in the sw buffer
		byte = rx_fifo.data_buf[rx_fifo.i_first]; // grab the oldest element in the buffer
		rx_fifo.i_first++;                        // increment the index of the oldest element
		rx_fifo.num_bytes--;                      // decrement the bytes counter
	}
	else
	{                                      // RX sw buffer is empty
		uart_rx_fifo_not_empty_flag = 0;          // clear the rx flag
	}
	if(rx_fifo.i_first == FIFO_BUFFER_SIZE) 
	{   // if the index has reached the end of the buffer,
		rx_fifo.i_first = 0;                      // roll over the index counter
	}
	Enable_global_interrupt(); 
	return byte; 
}
/***********************************************************************************************
 * serial_put_char_ur1(int c)
 * @brief Put character on UART1
 * @param input_char
 * @return input_char
 ***********************************************************************************************
 */	
int SerialPutCharUart1 (int input_char) 
{
//	#ifdef __UART
		while ((UART1->UART_SR & UART_SR_TXEMPTY) == 0);
		UART1->UART_THR = input_char;
//	#endif
		return (input_char);
}


/**********************************************************************************************
 * serial_get_char_ur1 (void)
 * @brief Read character from Serial Port - UART1  (blocking read)
 * @return integer upon receive
 **********************************************************************************************
 */
int SerialGetCharUart1 (void) 
{
//#ifdef __UART
    while((UART1->UART_SR & UART_SR_RXRDY) == 0);
    return UART1->UART_RHR;
//#endif
}

/***********************************************************************************************
 * serial_put_char_ur0(int c)
 * @brief Put character on UART0
 * @param input_char
 * @return input_char
 ***********************************************************************************************
 */
int SerialPutCharUart0 (int c) 
{
//#ifdef __UART
    while ((UART0->UART_SR & UART_SR_TXEMPTY) == 0);
    UART0->UART_THR = c;
//#endif
  return (c);
}


/**********************************************************************************************
 * serial_get_char_ur0 (void)
 * @brief Read character from Serial Port - UART0   (blocking read)
 * @return integer upon recieve
 ***********************************************************************************************
 */
int SerialGetCharUart0 (void) 
{
//#ifdef __UART
    while((UART0->UART_SR & UART_SR_RXRDY) == 0);
    return UART0->UART_RHR;
//#endif
}


/***********************************************************************************************
 * serial_put_char_us1(int c)
 * @brief Put character on USART1
 * @param input_char
 * @return input_char
 ***********************************************************************************************
 */
int SerialPutCharUsart1 (int input_char) 
{
//#ifdef __UART
    while ((USART1->US_CSR & US_CSR_TXEMPTY) == 0);
    USART1->US_THR = input_char;
//#endif
  return (input_char);
}


/**********************************************************************************************
 * serial_get_char_us1 (void)
 * @brief Read character from Serial Port - USART1   (blocking read)
 * @return integer upon recieve
 **********************************************************************************************
 */
int SerialGetCharUsart1 (void) 
{
//#ifdef __UART
	while((USART1->US_CSR & US_CSR_RXRDY) == 0);
    return USART1->US_RHR;
//#endif
}

/***********************************************************************************************
 * serial_put_char_us0(int c)
 * @brief Put character on USART0
 * @param input_char
 * @return input_char
 ***********************************************************************************************
 */
int SerialPutCharUsart0 (int input_char) 
{
//#ifdef __UART
   while ((USART0->US_CSR & US_CSR_TXEMPTY) == 0);
   USART0->US_THR = input_char;
//#endif
  return (input_char);
}


/**********************************************************************************************
 * serial_get_char_us0 (void)
 * @brief Read character from Serial Port - USART0   (blocking read)
 * @return integer upon recieve
 **********************************************************************************************
 */
int SerialGetCharUsart0 (void) 
{
//#ifdef __UART
while((USART0->US_CSR & US_CSR_RXRDY) == 0);
	return USART0->US_RHR;
//#endif
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (non-blocking read)
 *----------------------------------------------------------------------------*/
int SerialGetCharUsart1nb (void) 
{
//#ifdef __UART
	if((USART1->US_CSR & US_CSR_RXRDY)&&(USART1->US_CSR & US_CSR_RXBUFF))
	{
		USART1->US_IER=US_IDR_RXBUFF;
		return USART1->US_RHR;
		USART1->US_IER=US_IER_RXBUFF;
	}
//#endif
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (non-blocking read)
 *----------------------------------------------------------------------------*/
int SerialGetCharUart1nb (void) 
{
//#ifdef __UART
	//if((UART1->UART_SR & UART_SR_RXRDY)&&(UART1->UART_SR & UART_SR_RXBUFF))
	//{
		//UART1->UART_IER=UART_IDR_RXBUFF;
		//return UART1->UART_RHR;
		//UART1->UART_IER=UART_IER_RXBUFF;
	//}
//#endif
	
	if(uart_rx_fifo_not_empty_flag == 1) //check if the buffer has information in it
	{
		return uart_get_byte(); //get the byte from the buffer
	}
	else
	{
		return EOF; //there's no data return End Of File error code. 
	}


}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (non-blocking read)
 *----------------------------------------------------------------------------*/
int SerialGetCharUsart0nb (void) 
{
//#ifdef __UART
	if((USART0->US_CSR & US_CSR_RXRDY)&&(USART0->US_CSR & US_CSR_RXBUFF))
	{
		USART0->US_IER=US_IDR_RXBUFF;
		return USART0->US_RHR;
		USART0->US_IER=US_IER_RXBUFF;
	}
//#endif
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (non-blocking read)
 *----------------------------------------------------------------------------*/
int SerialGetCharUart0nb (void) 
{
//#ifdef __UART
	if((UART0->UART_SR & UART_SR_RXRDY)&&(UART0->UART_SR & UART_SR_RXBUFF))
	{
		UART0->UART_IER=UART_IDR_RXBUFF;
		return UART0->UART_RHR;
		UART0->UART_IER=UART_IER_RXBUFF;
	}
//#endif
}

/*
 *****************************************************************************************
 * @brief Print string to requested Quintic
 * @param[in] 1, 2 or 3 specifies the Quintic module to print to
 * @param[in] Input string to print
 * @description 
 *
 * This function prints the input string to the required Serial port or requested Quintic.
 *****************************************************************************************
 */
int SerialPrint(Uart* channel, char* str)
{
	int i=0;
	int size = strlen(str); 

	for (i=0;i<size;)
	{
		if(uart_write(channel, str[i]) == 0)
		{
			i++; //increment only if 1 is returned (it means the data has been sent)
		}		
	}
	
	//if(c==Q1)		// Send to Q1
	//{
//
	//}
	//
	//if(c==Q2)		//Send to Q2
	//{
		//for (a=0;a<size;a++)
		//{
			//SerialPutCharUart0(str[a]);
		//}
	//}
	//
	//if(c==Q3)		//Send to Q3
	//{
		//for (a=0;a<size;a++)
		//{
			//SerialPutCharUsart0(str[a]);
		//}
	//}
	//
	//if(c==SS)		//Send to SS
	//{
		//for (a=0;a<size;a++)
		//{			
			//SerialPutCharUart1(str[a]);
		//}
	//}

//printf("%s",str);
}

int SerialPrint_p(int c, char *str)
{	
	int i=0,a=0,j=0;

	while(str[j]!='\0')
	{	
		j++;
	}
	
	if(c==Q1)		// Send to Q1
	{
		for (a=0;a<j;a++)
		{		
			SerialPutCharUsart1(str[a]);
		}
	}
	
	if(c==Q2)		//Send to Q2
	{
		for (a=0;a<j;a++)
		{		
			SerialPutCharUart0(str[a]);
		}
	}
	
	if(c==Q3)		//Send to Q3
	{
		for (a=0;a<j;a++)
		{		
			SerialPutCharUsart0(str[a]);
		}
	}
}


/*----------------------------------------------------------------------------
  Calculate the number of characters in a string  
 *----------------------------------------------------------------------------*/
int StringLength(char *t)
{    
	int count=0;
    while(*t!='\0')
	{
        count++;
        t++;
    }
  return(count);
}

/*
 *****************************************************************************************
 * @brief Wait for Acknowledge signal from a desired Quintic module
 * @param[in] 1,2 or 3 specifies the Quintic module to wait for
 * @return 1 if successful
 * @description 
 *
 * This function waits for an Ack signal from desired Quintic module. This is a blocking
 * function.
 *****************************************************************************************
 */
int QnAck(int c){

	char QnAck[20]="QnAck";
	char ReceiveDataBuffer[128]={0};	
	uint8_t j=0, i=0;

	if(c==Q1)
	{
		do
		{
			j=0;
			do
			{
				ReceiveDataBuffer[j]=SerialGetCharUsart1();
			}while(ReceiveDataBuffer[j++]!=0x0a);
			
			//		for(i=0;i<j;i++){				// Debug print function
			//
			//		  printf("%c",ReceiveDataBuffer[i]);
			//		}
			//		printf("\r\n");
		}while(memcmp(ReceiveDataBuffer,QnAck,5)!=0);
		return 1;
	}
	
	if(c==Q2)
	{
		do
		{
			j=0;
			do
			{
				ReceiveDataBuffer[j]=SerialGetCharUart0();
			}while(ReceiveDataBuffer[j++]!=0x0a);
			
			//		for(i=0;i<j;i++){				// Debug print function
			//
			//		  printf("%c",ReceiveDataBuffer[i]);
			//		}
			//		printf("\r\n");
		}while(memcmp(ReceiveDataBuffer,QnAck,5)!=0);
		return 1;
	}
	
	if(c==Q3)
	{
		do
		{
			j=0;
			do
			{
				ReceiveDataBuffer[j]=SerialGetCharUsart0();
			}while(ReceiveDataBuffer[j++]!=0x0a);
			
			//		for(i=0;i<j;i++){				// Debug print function
			//
			//		  printf("%c",ReceiveDataBuffer[i]);
			//		}
			//		printf("\r\n");
		}while(memcmp(ReceiveDataBuffer,QnAck,5)!=0);
		return 1;
	}
}

/*
 *****************************************************************************************
 * @brief Wait for Scan Acknowledge signal from a desired Quintic module
 * @param[in] 1,2 or 3 specifies the Quintic module to wait for
 * @return 1 if successful
 * @description 
 *
 * This function waits for an Scan Ack signal from desired Quintic module. This is a blocking
 * function. The Quintic modules scan for the desired NODs and after the scan complete event
 * sends this ScanAck signal
 *****************************************************************************************
 */
int QnScanAck(int c){

	uint8_t QnScanAck[20]="QnScanAck";
	uint8_t QnScanErr[20]="QnScanErr";
	char ReceiveDataBuffer[128]={0};	
	uint8_t j=0, i=0, flag=0;
	
	if(c==Q1)
		{
			do
			{
				j=0;
				do
				{
					ReceiveDataBuffer[j]=SerialGetCharUsart1();
				}while(ReceiveDataBuffer[j++]!=0x0a);
				
				//		for(i=0;i<j;i++){				// Debug print function
				//
				//			printf("%c",ReceiveDataBuffer[i]);
				//		}
				//		printf("\r\n");
				if(memcmp(ReceiveDataBuffer,QnScanAck,9)==0)
				flag=1;
				else if(memcmp(ReceiveDataBuffer,QnScanErr,9)==0)
				flag=2;
			}while(flag==0);
			return flag;
		}
		
		if(c==Q2)
		{
			do
			{
				j=0;
				do
				{
					ReceiveDataBuffer[j]=SerialGetCharUart0();
				}while(ReceiveDataBuffer[j++]!=0x0a);
				
				//			for(i=0;i<j;i++){				// Debug print function
				//
				//			  printf("%c",ReceiveDataBuffer[i]);
				//			}
				//			printf("\r\n");
				
				if(memcmp(ReceiveDataBuffer,QnScanAck,9)==0)
				flag=1;
				else if(memcmp(ReceiveDataBuffer,QnScanErr,9)==0)
				flag=2;
			}while(flag==0);
			return flag;
		}
		
		if(c==Q3)
		{
			do
			{
				j=0;
				do
				{
					ReceiveDataBuffer[j]=SerialGetCharUsart0();
				}while(ReceiveDataBuffer[j++]!=0x0a);
				
				//		for(i=0;i<j;i++){				// Debug print function
				//
				//		  printf("%c",ReceiveDataBuffer[i]);
				//		}
				//		printf("\r\n");
				if(memcmp(ReceiveDataBuffer,QnScanAck,9)==0)
				flag=1;
				else if(memcmp(ReceiveDataBuffer,QnScanErr,9)==0)
				flag=2;
			}while(flag==0);
			return flag;
		}
}

/*
 *****************************************************************************************
 * @brief Wait for Connection Acknowledge signal from a desired Quintic module
 * @param[in] 1,2 or 3 specifies the Quintic module to wait for
 * @return 1 if successful
 * @description 
 *
 * This function waits for an Connection Ack signal from desired Quintic module. This is a 
 * blocking function. The Quintic modules initializes connection with the NODs and sends 
 * this ConAck signal when connected successfully.
 *****************************************************************************************
 */
int QnConAck(int c){

	uint8_t QnConAck[20]="QnConAck";
	uint8_t QnConErr[20]="QnConErr";
	char ReceiveDataBuffer[128]={0};	
	uint8_t j=0, i=0, flag=0;
	
	if(c==Q1)
	{
		do
		{
			j=0;
			do
			{
				ReceiveDataBuffer[j]=SerialGetCharUsart1();
			}while(ReceiveDataBuffer[j++]!=0x0a);
			
			//		for(i=0;i<j;i++){				// Debug print function
			//
			//			printf("%c",ReceiveDataBuffer[i]);
			//		}
			//		printf("\r\n");
			if(memcmp(ReceiveDataBuffer,QnConAck,8)==0)
			flag=1;
			else if(memcmp(ReceiveDataBuffer,QnConErr,8)==0)
			flag=2;
		}while(flag==0);
		return flag;
	}
	
	if(c==Q2)
	{
		do
		{
			j=0;
			do
			{
				ReceiveDataBuffer[j]=SerialGetCharUart0();
			}while(ReceiveDataBuffer[j++]!=0x0a);
			
			//		for(i=0;i<j;i++){				// Debug print function
			//
			//			printf("%c",ReceiveDataBuffer[i]);
			//		}
			//		printf("\r\n");
			if(memcmp(ReceiveDataBuffer,QnConAck,8)==0)
			flag=1;
			else if(memcmp(ReceiveDataBuffer,QnConErr,8)==0)
			flag=2;
		}while(flag==0);
		return flag;
	}
	
	if(c==Q3)
	{
		do
		{
			j=0;
			do
			{
				ReceiveDataBuffer[j]=SerialGetCharUsart0();
			}while(ReceiveDataBuffer[j++]!=0x0a);
			
			//		for(i=0;i<j;i++){				// Debug print function
			//
			//			printf("%c",ReceiveDataBuffer[i]);
			//		}
			//		printf("\r\n");
			if(memcmp(ReceiveDataBuffer,QnConAck,8)==0)
			flag=1;
			else if(memcmp(ReceiveDataBuffer,QnConErr,8)==0)
			flag=2;
		}while(flag==0);
		return flag;
	}
}
