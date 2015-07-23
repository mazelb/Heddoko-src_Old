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

#define nod [9][]={0};

#include <stdio.h>
#include <stdint.h>
#include <rtl.h>
#include <SAM4S.h>
#include "MCI_SAM4S.h"

#include "Serial.h"
#include "Tasks.h"
#include "LED.h"
#include "Button.h"
#include "UART_functionality.h"
#include "SDcard_functionality.h"

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 10 ms)
 *----------------------------------------------------------------------------*/
void delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

/**
 * startup_test(void)
 * @brief Banner/test function on initialization
 */
static void startup_test(void) {
	printf ("Hello Worlda\n\r");
	serial_put_char_ur0('j');
	serial_put_char_ur0('\r');
	serial_put_char_ur0('\n');
	serial_put_char_us1('j');
	serial_put_char_us1('\r');
	serial_put_char_us1('\n');
}


/**
 * main(void)
 * 
 */
int main (void) {

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */

  serial_init();
  LED_init();
  button_init();
	UART_USART_init();
	
	
  if (SysTick_Config(SystemCoreClock / 100)) {  /* SysTick 1 msec interrupts */
    while (1) __NOP();                          /* Capture error              */
  }
	
	startup_test();
	//cmd_fill(NULL);
	
	os_sys_init(task_main);

}

