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

/**
 * SerialInit (void)
 * @brief Initialize Serial port
 */
void SerialInit (void) {
#ifndef __DBG_ITM

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

//#ifdef __UART
  PMC->PMC_PCER0 = ((1UL << ID_PIOB) |    /* enable PIOA clock                */
                    (1UL << ID_UART1)  ); /* enable UART clock                */

  /* Configure UART1 Pins (PB3 = TX, PB2 = RX). */
  PIOB->PIO_IDR        =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_PUDR       =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_ABCDSR[0] &= ~(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_ABCDSR[1] &= ~(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_PDR        =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);

  /* Configure UART for 115200 baud. */
  UART1->UART_CR   = (UART_CR_RSTRX | UART_CR_RSTTX) |
                     (UART_CR_RXDIS | UART_CR_TXDIS);
  UART1->UART_IDR  = 0xFFFFFFFF;
  UART1->UART_BRGR   = BAUD(115200);
  UART1->UART_MR   =  (0x4 <<  9);        /* (UART) No Parity                 */
  UART1->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  UART1->UART_CR   = UART_CR_RXEN | UART_CR_TXEN;
//#endif

// configure uart0 for Q2
//#ifdef __UART
  PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    /* enable PIOA clock                */
                    (1UL << ID_UART0)  ); /* enable UART clock                */

  /* Configure UART0 Pins (PA10 = TX, PA9 = RX). */
  PIOA->PIO_IDR        =  (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
  PIOA->PIO_PUDR       =  (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
  PIOA->PIO_ABCDSR[0] &= ~(PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
  PIOA->PIO_ABCDSR[1] &= ~(PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
  PIOA->PIO_PDR        =  (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);

  /* Configure UART for 115200 baud. */
  UART0->UART_CR   = (UART_CR_RSTRX | UART_CR_RSTTX) |
                     (UART_CR_RXDIS | UART_CR_TXDIS);
  UART0->UART_IDR  = 0xFFFFFFFF;
  UART0->UART_BRGR   = BAUD(115200);
  UART0->UART_MR   =  (0x4 <<  9);        /* (UART) No Parity                 */
  UART0->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  UART0->UART_CR   = UART_CR_RXEN | UART_CR_TXEN;
//#endif

//	configure usart1 for Q1
//#ifdef __UART
	PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    // enable PIOA clock                
                    (1UL << ID_USART1)  ); // enable USART clock                

  /* Configure USART1 Pins (PA22 = TX, PA21 = RX). */
  PIOA->PIO_IDR        =  (PIO_PA21A_RXD1 | PIO_PA22A_TXD1);
  PIOA->PIO_PUDR       =  (PIO_PA21A_RXD1 |  PIO_PA22A_TXD1);
  PIOA->PIO_ABCDSR[0] &= ~(PIO_PA21A_RXD1 | PIO_PA22A_TXD1);
  PIOA->PIO_ABCDSR[1] &= ~(PIO_PA21A_RXD1 | PIO_PA22A_TXD1);
  PIOA->PIO_PDR        =  (PIO_PA21A_RXD1 | PIO_PA22A_TXD1);
	
	/* configure USART1 enable Pin (PA23) */
  PIOA->PIO_PUDR   =  (PIO_PA23);
  PIOA->PIO_CODR   =  (PIO_PA23);
  PIOA->PIO_OER    =  (PIO_PA23);
  PIOA->PIO_PER    =  (PIO_PA23);

  /* Configure USART for 115200 baud. */
  USART1->US_CR   = (US_CR_RSTRX | US_CR_RSTTX) |
                     (US_CR_RXDIS | US_CR_TXDIS);
  USART1->US_IDR  = 0xFFFFFFFF;
  USART1->US_BRGR   = BAUD(115200);
  USART1->US_MR   =  (US_MR_USART_MODE_NORMAL | US_MR_USCLKS_MCK | US_MR_CHRL_8_BIT |
												US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL) ;//(0x4 <<  9);        /* (USART) No Parity                 */
  USART1->US_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  USART1->US_CR   = US_CR_RXEN | US_CR_TXEN;
	
//#endif

//	configure usart0 for Q3
//#ifdef __UART
	PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    // enable PIOA clock                
                    (1UL << ID_USART0)  ); // enable USART clock                

  /* Configure USART0 Pins (PA6 = TX, PA5 = RX). */
  PIOA->PIO_IDR        =  (PIO_PA5A_RXD0 | PIO_PA6A_TXD0);
  PIOA->PIO_PUDR       =  (PIO_PA5A_RXD0 | PIO_PA6A_TXD0);
  PIOA->PIO_ABCDSR[0] &= ~(PIO_PA5A_RXD0 | PIO_PA6A_TXD0 | PIO_PA2B_SCK0);
  PIOA->PIO_ABCDSR[1] &= ~(PIO_PA5A_RXD0 | PIO_PA6A_TXD0 | PIO_PA2B_SCK0);
  PIOA->PIO_PDR        =  (PIO_PA5A_RXD0 | PIO_PA6A_TXD0);
	
	/* configure USART0 enable Pin (PA2) Peripheral-B */
  PIOA->PIO_PUDR   =  (PIO_PA2);
  PIOA->PIO_CODR   =  (PIO_PA2);
  PIOA->PIO_OER    =  (PIO_PA2);
  PIOA->PIO_PER    =  (PIO_PA2);

  /* Configure USART for 115200 baud. */
  USART0->US_CR   = (US_CR_RSTRX | US_CR_RSTTX) |
                     (US_CR_RXDIS | US_CR_TXDIS);
  USART0->US_IDR  = 0xFFFFFFFF;
  USART0->US_BRGR   = BAUD(115200);
  USART0->US_MR   =  (US_MR_USART_MODE_NORMAL | US_MR_USCLKS_MCK | US_MR_CHRL_8_BIT |
												US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL) ;//(0x4 <<  9);        /* (USART) No Parity                 */
  USART0->US_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  USART0->US_CR   = US_CR_RXEN | US_CR_TXEN;
	
//#endif

////	configure SPI for Q2
//#ifdef __UART
//	PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    // enable PIOA clock                
//                    (1UL << ID_SPI)  ); // enable USART clock                

//  /* Configure USART1 Pins (PA22 = TX, PA21 = RX). */
//  PIOA->PIO_IDR        =  (PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//  PIOA->PIO_PUDR       =  (PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//  PIOA->PIO_ABCDSR[0] &= ~(PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//  PIOA->PIO_ABCDSR[1] &= ~(PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//  PIOA->PIO_PDR        =  (PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//	
//	/* configure USART1 enable Pin (PA23) */
//  PIOA->PIO_PUDR   =  (PIO_PA23);
//  PIOA->PIO_CODR   =  (PIO_PA23);
//  PIOA->PIO_OER    =  (PIO_PA23);
//  PIOA->PIO_PER    =  (PIO_PA23);

//  /* Configure USART for 115200 baud. */
//  SPI->SPI_CR   = (SPI_CR_SPIEN);
//  SPI->SPI_IDR  = 0xFFFFFFFF;
//  SPI->SPI_MR   =  (SPI_MR_MSTR |  US_MR_CHRL_8_BIT |
//												US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL) ;//(0x4 <<  9);        /* (USART) No Parity                 */
//  USART1->US_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
//  USART1->US_CR   = UART_CR_RXEN | UART_CR_TXEN;
//	
//#endif

	//PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */

#endif
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
	if((UART1->UART_SR & UART_SR_RXRDY)&&(UART1->UART_SR & UART_SR_RXBUFF))
	{
		UART1->UART_IER=UART_IDR_RXBUFF;
		return UART1->UART_RHR;
		UART1->UART_IER=UART_IER_RXBUFF;
	}
//#endif
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
int SerialPrint(int c, char str[20])
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
	
	if(c==SS)		//Send to SS
	{
		for (a=0;a<j;a++)
		{			
			SerialPutCharUart1(str[a]);
		}
	}
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
