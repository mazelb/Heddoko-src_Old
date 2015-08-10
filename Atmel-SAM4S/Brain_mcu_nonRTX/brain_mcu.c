/*----------------------------------------------------------------------------
 * Name:    brain_mcu.c
 * Purpose: The main tasks for Brain MCU
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <SAM4S.h>
#include "Serial.h"
#include "LED.h"
#include "BTN.h"
#include "commands.h"
#include "BrainMCU.h"
//#include "usart0.h"

//Interrupt Handler for reading UART0
int read_uart0(){

	uint8_t i=0;
	if((UART0->UART_SR & UART_SR_RXRDY)&&(UART0->UART_SR & UART_SR_RXBUFF)){
			//printf("IF loop");
			UART0->UART_IER=UART_IDR_RXBUFF;
			qn.id[1].data[q2i][q2j][q2k]=UART0->UART_RHR;
//			i=UART0->UART_RHR;
//			SER_PutChar(i);
//			i=0;
			UART0->UART_IER=UART_IER_RXBUFF;
		}
}

	//Interrupt Handler for USART1
int read_usart1(){
	
	uint8_t i=0;
	if((USART1->US_CSR & US_CSR_RXRDY)&&(USART1->US_CSR & US_CSR_RXBUFF)){
		//printf("IF loop");
		USART1->US_IER=US_IDR_RXBUFF;
		qn.id[0].data[q1i][q1j][q1k]=USART1->US_RHR;
//		i=USART1->US_RHR;	
//		SER_PutChar(i);
//		i=0;
		USART1->US_IER=US_IER_RXBUFF;
	}
}

//Interrupt Handler for USART0
int read_usart0(){
	
	uint8_t i=0;
	if((USART0->US_CSR & US_CSR_RXRDY)&&(USART0->US_CSR & US_CSR_RXBUFF)){
		//printf("IF loop");
		USART0->US_IER=US_IDR_RXBUFF;
		qn.id[2].data[q3i][q3j][q3k]=USART0->US_RHR;
//		i=USART0->US_RHR;
//		SER_PutChar(i);
//		i=0;
		USART0->US_IER=US_IER_RXBUFF;
	}
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  
	int32_t idx  = -1, dir = 1;
	uint32_t btns = 0;
	uint8_t i=0, j=0, k=0;
	
  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */

  SER_Init();
  LED_Init();
  BTN_Init();
	
	//	Enable interrupts on UART0
	UART0->UART_IER=UART_IER_RXBUFF;
	UART0->UART_IER=UART_IER_ENDRX;
	UART0->UART_IER=UART_IER_RXRDY;
	
	//	Enable interrupts on USART1
	USART1->US_IER=US_IER_RXBUFF;
	USART1->US_IER=US_IER_ENDRX;
	USART1->US_IER=US_IER_RXRDY;
	
  if (SysTick_Config(SystemCoreClock / 1000)) {  /* SysTick 10 msec interrupts */
    while (1) __NOP();                          /* Capture error              */
  }
	
	// UART debug stuff
	printf ("Hello World\r\n");
	//SER_PutChar0('j');
	//SER_PutChar0('\r');
	//SER_PutChar0('\n');
	//SER_PutChar_us1('j');
	//SER_PutChar_us1('\r');
	//SER_PutChar_us1('\n');
	// end of uart debug	
	
	//strncpy(nod.idx[0].addr,"A0E5E9001598\r\n",20);
	
	/*	Send wake-up command and NOD addresses to Quintics	*/	
		
	Q1_init();
	Q2_init();
	Q3_init();
	Qn_start();
	
  while((q1i<bufsize)&&(q2i<bufsize)&&(q3i<bufsize)) {    		/* Loop forever               */
		
		// Application interface //
		
		//printf("While loop");
		
		if(q1i<bufsize)		
			read_usart1();
		
		if(q2i<bufsize)	
			read_uart0();
		
		if(q3i<bufsize)	
			read_usart0();
		
		if(qn.id[0].data[q1i][q1j][q1k]!='\0')		//Heddoko: To increment the buffer only if a character is received
			q1k++;
		if(qn.id[1].data[q2i][q2j][q2k]!='\0')
			q2k++;
		if(qn.id[2].data[q3i][q3j][q3k]!='\0')
			q3k++;
		
		if(q1k>=datasize){
			
			q1j++;
			q1k=0;
		}
		if(q1j>=elmntnb){
			
			q1i++;
			q1j=0;
			q1k=0;
		}
		
		if(q2k>=datasize){
			
			q2j++;
			q2k=0;
		}
		if(q2j>=elmntnb){
			
			q2i++;
			q2j=0;
			q2k=0;
		}
		
		if(q3k>=datasize){
			
			q3j++;
			q3k=0;
		}
		if(q3j>=elmntnb){
			
			q3i++;
			q3j=0;
			q3k=0;
		}
		
		btns = BTN_Get();
			
    if (btns == 0) {                            /* no push button pressed     */
//      /* Calculate 'idx': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...           */
//      idx += dir;
//      if (idx == LED_NUM) { dir = -1; idx =  LED_NUM-1; }
//      else if   (idx < 0) { dir =  1; idx =  0;         }

//      LED_On (idx);                             /* Turn on LED 'idx'          */
//      Delay(20);                                /* Delay 200ms                */
//      LED_Off(idx);                             /* Turn off LED 'idx'         */
//      Delay(20);                                /* Delay 200ms                */
    }
    else {
      LED_Out(0x01);
      Delay(20);                                /* Delay 200ms                */
      LED_Out(0x00);
			ser_print(Q1,"stop\r\n");
			ser_print(Q2,"stop\r\n");
			ser_print(Q3,"stop\r\n");
    }
	
    //printf ("Hello World\n\r");
  }
	
	
	ser_print(Q1,"stop\r\n");
	ser_print(Q2,"stop\r\n");
	ser_print(Q3,"stop\r\n");
	
	printf("Printing Buffers\r\n");
	
	printf("Q1\r\n\r\n");
	for(i=0;i<bufsize;i++){
		
		for(j=0;j<elmntnb;j++){
		
			printf("%s\r\n",qn.id[0].data[i][j]);
		}
	}
	
	printf("Q2\r\n\r\n");
	for(i=0;i<bufsize;i++){
		
		for(j=0;j<elmntnb;j++){
		
			printf("%s\r\n",qn.id[1].data[i][j]);
		}
	}
	
	printf("Q3\r\n\r\n");
	for(i=0;i<bufsize;i++){
		
		for(j=0;j<elmntnb;j++){
		
			printf("%s\r\n",qn.id[2].data[i][j]);
		}
	}

}

