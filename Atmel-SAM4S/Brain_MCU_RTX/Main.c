/**
 * @file Main.c
 * @author Heddoko
 * @date July 24, 2015
 * @copy
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 * @copy Heddoko 2015
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rtl.h>
#include <string.h>
#include <SAM4S.h>
#include "MCI_SAM4S.h"
#include "BrainMCU.h"
#include "commands.h"
#include "Serial.h"
#include "Tasks.h"
#include "LED.h"
#include "Button.h"
#include "UART_functionality.h"
#include "SDcard_functionality.h"

//volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */

///*----------------------------------------------------------------------------
//  delays number of tick Systicks (happens every 10 ms)
// *----------------------------------------------------------------------------*/
//void delay (uint32_t dlyTicks) {
//  uint32_t curTicks;

//  curTicks = msTicks;
//  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
//}

/**
 * startup_test(void)
 * @brief Banner/test function on initialization
 */
static void startup_test(void) {
	printf ("Heddoko MCU\n\r");
	ser_putchar_ur0('j');
	ser_putchar_ur0('\r');
	ser_putchar_ur0('\n');
	ser_putchar_us1('j');
	ser_putchar_us1('\r');
	ser_putchar_us1('\n');
}


/**
 * main(void)
 * 
 */
int main (void) {
	
	int32_t idx  = -1, dir = 1;
	uint32_t btns = 0;
	uint8_t i=0, j=0, k=0;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */

//	NVIC_EnableIRQ(PIOA_IRQn);
  
	serial_init();
  LED_init();
  button_init();
	UART_USART_init();
//	if(sd_init()==1)
//		printf("SD_INIT SUCCESS\r\n");
//	else
//		printf("SD_INIT FAILED\r\n");
	
  if (SysTick_Config(SystemCoreClock / 100)) {  /* SysTick 1 msec interrupts */
    while (1) __NOP();                          /* Capture error              */
  }
	startup_test();
	//cmd_fill(NULL);
	
	os_sys_init(task_idle);

}

