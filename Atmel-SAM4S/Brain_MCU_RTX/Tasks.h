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
#include <stdbool.h>
#include "Serial.h"
#include "UART_functionality.h"
#include "LED.h"
#include "Button.h"

#ifndef __TASKS_H
#define __TASKS_H

static bool strt_stop=0, pw_irq, sw1_irq, sw2_irq, btn_irq, init_flg=0, main_flg=0;
static uint8_t btn_menu_lvl=0;
static uint32_t cur_ticks, old_ticks, pioa=0;

__task void task_main(void);
__task void task_idle(void);
__task void task_init(void);
__task void task_uart0(void);
__task void task_uart1(void);
__task void task_usart0(void);
__task void task_usart1(void);
__task void task_swdbutton(void);
int call_timer (void);

#endif
