/**
 * @file Tasks.c
 * @author Ron Brash - Heddoko 
 * @date July 22nd, 2015
 * @brief Handle all RTX tasks
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <rtl.h>
#include <SAM4S.h>

#include "Serial.h"
#include "UART_functionality.h"
#include "Tasks.h"
#include "LED.h"
#include "Button.h"

OS_TID id0, id1, id2, id3, id4, id5;

volatile uint8_t buttonState;
/*
 * task_uart0(void)
 * @brief UART0 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_uart0(void) {

	for(;;) 
	{
				os_evt_wait_or(0x0004,0xFFFF);
				
				read_uart0();
				os_evt_set(0x0004,id0);

	}
}

/*
 * task_uart1(void)
 * @brief UART1 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_uart1(void) {

	for(;;) {
				os_evt_wait_or(0x0004,0xFFFF);
				
				//read_uart1();
				os_evt_set(0x0004,id0);

	}
}

/*
 * task_usart0(void)
 * @brief USART0 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_usart0(void) {
	for(;;) {
				os_evt_wait_or(0x0004,0xFFFF);
				
				//read_usart0();
				os_evt_set(0x0004,id1);

	}
}

/*
 * task_usart1(void)
 * @brief USART1 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_usart1(void) {
	for(;;) {
				os_evt_wait_or(0x0004,0xFFFF);
				
			  read_usart1();
				os_evt_set(0x0004,id1);

	}
}

/*
 * task_swdbutton(void)
 * @brief SWD BUTTON task  with basic debouncing (press registered on release)
 * @note PINS: SWD button
 */
__task void task_swdbutton(void) {
	
	for(;;) {
				os_evt_wait_or(0x0004,0xFFFF);
				if ((button_get() != 0) && (buttonState !=1)) {
					buttonState = 1;
					LED_on(0);
				} else if ((button_get() == 0) && (buttonState ==1)){
					printf("Button press and release\n\r");
					LED_blink(3);
					//LED_off(0);
					buttonState = 0;
				}
		
				os_evt_set(0x0004,id1);

	}
}

/*
 * task_main(void)
 * @brief Main task loop
 * @note Can be tidied up a bit - may need some performance tuning
 */
__task void task_main(void) {
	id0 = os_tsk_self();
	id1 = os_tsk_create(task_uart0,0);
	id2 = os_tsk_create(task_uart1,0);
	id3 = os_tsk_create(task_usart0,0);
	id4 = os_tsk_create(task_usart1,0);
	id5 = os_tsk_create(task_swdbutton,0);
	
	for(;;) {
		// UARTS & USARTS
		os_evt_set(0x0004,id1);
		os_evt_wait_or(0x0004,0xFFFF);
		os_evt_set(0x0004,id2);
		os_evt_wait_or(0x0004,0xFFFF);
		os_evt_set(0x0004,id3);
		os_evt_wait_or(0x0004,0xFFFF);
		
		// Reserved for stretch sense
		os_evt_set(0x0004,id4);
		os_evt_wait_or(0x0004,0xFFFF);
	
		// Reserved for SWD button - is a signal better???
		os_evt_set(0x0004,id5);
		os_evt_wait_or(0x0004,0xFFFF);
		
	}
}
