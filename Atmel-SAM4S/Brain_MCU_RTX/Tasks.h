/**
 * @file Tasks.h
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
#include "LED.h"
#include "Button.h"

#ifndef __TASKS_H
#define __TASKS_H
__task void task_main(void);
__task void task_uart0(void);
__task void task_uart1(void);
__task void task_usart0(void);
__task void task_usart1(void);
__task void task_swdbutton(void);

#endif
