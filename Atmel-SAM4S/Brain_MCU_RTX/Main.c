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

/**
 * StartupTest(void)
 * @brief Banner/test function on initialization
 */
static void StartupTest(void) 
{
	printf ("Heddoko MCU\n\r");
	SerialPutCharUart0('j');
	SerialPutCharUart0('\r');
	SerialPutCharUart0('\n');
	SerialPutCharUsart1('j');
	SerialPutCharUsart1('\r');
	SerialPutCharUsart1('\n');
}


/**
 * main(void)
 * 
 */
int main (void) 
{
	
	int32_t idx  = -1;

  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  
	SerialInit();
  LedInit();
  ButtonInit();
	UartUsartInit();
	
  if (SysTick_Config(SystemCoreClock / 100)) 		/* SysTick 1 msec interrupts */
	{  
    while (1) __NOP();                          /* Capture error              */
  }
	
	StartupTest();
//	cmd_fill(NULL);															/*	Initialize SD-card	*/
	
	os_sys_init(task_idle);

}

