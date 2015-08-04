/*----------------------------------------------------------------------------
 * Name:    brain_mcu.c
 * Purpose: The main tasks for Brain MCU
 * Note(s):
 *----------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/

//#define nod [9][]={0};	/* The data structure to store the address of NODs	*/

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

struct id{	/* The data structure to stores the address and data of NODs	*/
	
	//nod address
	uint8_t addr[6];
	
	//nod data
	uint8_t data[6];
};

struct nod{		/* The data structure to store the ID of NODs	*/ 
	
	//	nod ID
	struct id idx;
};

//Interrupt Handler for reading UART0
int read_uart0(){

	uint8_t i=0;
	if((UART0->UART_SR & UART_SR_RXRDY)&&(UART0->UART_SR & UART_SR_RXBUFF)){
			//printf("IF loop");
			UART0->UART_IER=UART_IDR_RXBUFF;
			i=UART0->UART_RHR;
			SER_PutChar(i);
			i=0;
			UART0->UART_IER=UART_IER_RXBUFF;
		}
}

	//Interrupt Handler for USART1
int read_usart1(){
	
	uint8_t i=0;
	if((USART1->US_CSR & US_CSR_RXRDY)&&(USART1->US_CSR & US_CSR_RXBUFF)){
		//printf("IF loop");
		USART1->US_IER=US_IDR_RXBUFF;
		i=USART1->US_RHR;
		SER_PutChar(i);
		i=0;
		USART1->US_IER=US_IER_RXBUFF;
	}
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  
	int32_t idx  = -1, dir = 1;
	uint32_t btns = 0;
	uint8_t i;
	
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
	SER_PutChar0('j');
	SER_PutChar0('\r');
	SER_PutChar0('\n');
	SER_PutChar_us1('j');
	//SER_PutChar_us1('\r');
	//SER_PutChar_us1('\n');
	// end of uart debug
	
	/*	Send wake-up command and NOD addresses to Quintics	*/	
		
	Q1_init();
	Q2_init();
	Q3_init();
	Qn_start();
	
  while(1) {    		/* Loop forever               */
		
		// Application interface //
		
		read_uart0();
		read_usart1();
		
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
			ser_print(1,"stop\r\n");
    }
	
    //printf ("Hello World\n\r");
  }

}

