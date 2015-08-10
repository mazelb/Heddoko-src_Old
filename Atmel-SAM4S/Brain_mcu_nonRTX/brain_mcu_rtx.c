/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <RTL.h>
#include <stdio.h>
#include <SAM4S.h>
#include "Serial.h"
#include "LED.h"
#include "BTN.h"

OS_TID t_ledOn;                         /* assigned task id of task: ledOn   */
OS_TID t_ledOff;                        /* assigned task id of task: ledOff  */
OS_TID t_ser;


//volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
///*----------------------------------------------------------------------------
//  SysTick_Handler
// *----------------------------------------------------------------------------*/
//void SysTick_Handler(void) {
//  msTicks++;
//}

///*----------------------------------------------------------------------------
//  delays number of tick Systicks (happens every 10 ms)
// *----------------------------------------------------------------------------*/
//void Delay (uint32_t dlyTicks) {
//  uint32_t curTicks;

//  curTicks = msTicks;
//  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
//}

/*----------------------------------------------------------------------------
  Task 1 'ledOn': switches the LED on
 *---------------------------------------------------------------------------*/
__task void ledOn (void) {
  for (;;) {
    LED_On(0);                          /* Turn LED On                       */
    os_evt_set (0x0001, t_ledOff);      /* send event to task 'ledoff'       */
    os_dly_wait (50);                   /* delay 50 clock ticks              */
  }
}

/*----------------------------------------------------------------------------
  Task 2 'ledOff': switches the LED off
 *---------------------------------------------------------------------------*/
__task void ledOff (void) {
  for (;;) {
    os_evt_wait_and (0x0001, 0xffff);   /* wait for an event flag 0x0001     */
    os_dly_wait (8);                    /* delay 8 clock ticks               */
    LED_Off(0);                         /* Turn LED Off                      */
  }
}

/*----------------------------------------------------------------------------
  Task 3 'ser': Serial Interface
 *---------------------------------------------------------------------------*/
__task void ser (void) {

  //SER_Init();
  uint8_t i;
	
	if((UART0->UART_SR & UART_SR_RXRDY)&&(UART0->UART_SR & UART_SR_RXBUFF)){
			printf("IF loop");
			UART0->UART_IER=UART_IDR_RXBUFF;
			i=UART0->UART_RHR;
			SER_PutChar(i);
			i=0;
			UART0->UART_IER=UART_IER_RXBUFF;
		}
}

/*----------------------------------------------------------------------------
  Task 4 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {

  //SER_Init();
  BTN_Init();
	LED_Init();                              /* Initialize LEDs                  */

  t_ledOn  = os_tsk_create (ledOn, 0);   /* start task 'ledOn'               */
  t_ledOff = os_tsk_create (ledOff, 0);  /* start task 'ledOff'              */
	t_ser = os_tsk_create (ser, 0);
  os_tsk_delete_self ();
}



/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t idx  = -1, dir = 1;
	uint32_t btns = 0;
	

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */

  SER_Init();
  LED_Init();
  BTN_Init();
	UART0->UART_IER=UART_IER_RXBUFF;
	UART0->UART_IER=UART_IER_ENDRX;
	UART0->UART_IER=UART_IER_RXRDY;
	
  if (SysTick_Config(SystemCoreClock / 100)) {  /* SysTick 10 msec interrupts */
    while (1) __NOP();                          /* Capture error              */
  }
	
	printf ("Hello World\n\r");
	SER_PutChar0('j');
	SER_PutChar0('\r');
	SER_PutChar0('\n');
	
	os_sys_init(init);                        /* Initialize RTX and start init */
	
//  while(1) {    		/* Loop forever               */
//    btns = BTN_Get();                           /* Read button states         */
//		
//		// Application interface //
//		//printf ("Hello World\n\r");
//		//SER_PutChar0('j');
//		//SER_PutChar0('\r');
//		//SER_PutChar0('\n');
//		//SER_GetChar0();
//		
//		if((UART0->UART_SR & UART_SR_RXRDY)&&(UART0->UART_SR & UART_SR_RXBUFF)){
//			//printf("IF loop");
//			UART0->UART_IER=UART_IDR_RXBUFF;
//			i=UART0->UART_RHR;
//			SER_PutChar(i);
//			i=0;
//			UART0->UART_IER=UART_IER_RXBUFF;
//		}
//			
//		//SER_PutChar(i);
//	
////    if (btns == 0) {                            /* no push button pressed     */
////      /* Calculate 'idx': 0,1,...,LED_NUM-1,LED_NUM-1,...,1,0,0,...           */
////      idx += dir;
////      if (idx == LED_NUM) { dir = -1; idx =  LED_NUM-1; }
////      else if   (idx < 0) { dir =  1; idx =  0;         }

////      LED_On (idx);                             /* Turn on LED 'idx'          */
////      Delay(20);                                /* Delay 200ms                */
////      LED_Off(idx);                             /* Turn off LED 'idx'         */
////      Delay(20);                                /* Delay 200ms                */
////    }
////    else {
////      LED_Out(0x01);
////      Delay(20);                                /* Delay 200ms                */
////      LED_Out(0x00);
////    }
//	
//    //printf ("Hello World\n\r");
//  }

}
