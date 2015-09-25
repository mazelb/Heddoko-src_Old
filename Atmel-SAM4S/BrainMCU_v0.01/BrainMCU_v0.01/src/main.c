/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Functionality_Tests.h"
#include "Board_Init.h"
#include "Config_Settings.h"
#include "conf_board.h"
#include "BrainMCU.h"
#include "task_main.h"
#include "Ser.h"
#include "Commands.h"
#include "GPIO.h"
#include "drv_uart.h"
#include "DebugLog.h"
#include "task_quinticInterface.h"

#define TASK_SERIAL_RECEIVE_STACK_SIZE                (2048/sizeof(portSTACK_TYPE))
#define TASK_SERIAL_RECEIVE_STACK_PRIORITY            (tskIDLE_PRIORITY)
//these values are actually defined in Board_Init.c
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void xPortSysTickHandler(void);
extern void SysTickHandler(void);
extern drv_uart_config_t uart0Config;
extern drv_uart_config_t uart1Config;
extern drv_uart_config_t usart0Config;
extern drv_uart_config_t usart1Config;
extern brainSettings_t brainSettings; 
volatile unsigned long sgSysTickCount = 0;

/**
 * \brief Handler for System Tick interrupt.
 */
void SysTick_Handler(void)
{
	sgSysTickCount++;
	xPortSysTickHandler();
}






int main (void)
{	
	irq_initialize_vectors();
	cpu_irq_enable();
	
	//Initialize system clock and peripherals
	sysclk_init();
	board_init();
	
	
	/*	Create task Main	*/
	if (xTaskCreate(TaskMain, "Main", TASK_MAIN_STACK_SIZE, NULL, TASK_MAIN_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create Main task\r\n");
	}
	
	/*	Create a task to maintain a Debug Log routine	*/
	if (xTaskCreate(TaskDebugLog, "DebugLog", TASK_DEBUGLOG_STACK_SIZE, NULL, TASK_DEBUGLOG_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create Debug Log task\r\n");
	}
	
	//char buf[200] = {0};
	//snprintf(buf,sizeof(buf), "Starting RTOS \n\r");
	
	//SerialPrint(SS,buf);

	/* Start the scheduler. */
	vTaskStartScheduler();
	//we should never get here. 
	while(1); 

	
	/*	Debug code */
	while (1) 
	{		

		//SerialPrint(Q1,"send1\r\n");
		//cnt=Nod.id[0].ElementCount;
		//SerialPrint(SS,"\r\nstart1\r\n");
		//while(Nod.id[0].ElementCount<(cnt+21))
		//{
			//if(Nod.id[0].BufCount<gBufSize)
			//ReadUsart1(0);
			//if(Nod.id[0].DataCount>=gDataSize)
			//{				
				//Nod.id[0].ElementCount++;
				//Nod.id[0].DataCount=0;
			//}
			//if(Nod.id[0].ElementCount>=gElementNum)
			//{				
				//Nod.id[0].BufCount++;
				//Nod.id[0].ElementCount=0;
				//Nod.id[0].DataCount=0;
			//}
			//if(Nod.id[0].BufCount>gBufSize)
			//Nod.id[0].BufCount=0;
		//}
		//SerialPrint(SS, "\r\nend1\r\n");
		//
		//SerialPrint(Q1,"send2\r\n");
		//cnt=Nod.id[1].ElementCount;
		//SerialPrint(SS, "\r\nstart2\r\n");
		//while(Nod.id[1].ElementCount<(cnt+21))
		//{
			//if(Nod.id[1].BufCount<gBufSize)
			//ReadUsart1(1);
			//if(Nod.id[1].DataCount>=gDataSize)
			//{				
				//Nod.id[1].ElementCount++;
				//Nod.id[1].DataCount=0;
			//}
			//if(Nod.id[1].ElementCount>=gElementNum)
			//{				
				//Nod.id[1].BufCount++;
				//Nod.id[1].ElementCount=0;
				//Nod.id[1].DataCount=0;
			//}
			//if(Nod.id[1].BufCount>gBufSize)
			//Nod.id[1].BufCount=0;
		//}
		//SerialPrint(SS, "\r\nend2\r\n");
		//
		//SerialPrint(Q1,"send3\r\n");
		//cnt=Nod.id[2].ElementCount;
		//SerialPrint(SS, "\r\nstart3\r\n");
		//while(Nod.id[2].ElementCount<(cnt+21))
		//{
			//if(Nod.id[2].BufCount<gBufSize)
			//ReadUsart1(2);
			//if(Nod.id[2].DataCount>=gDataSize)
			//{				
				//Nod.id[2].ElementCount++;
				//Nod.id[2].DataCount=0;
			//}
			//if(Nod.id[2].ElementCount>=gElementNum)
			//{				
				//Nod.id[2].BufCount++;
				//Nod.id[2].ElementCount=0;
				//Nod.id[2].DataCount=0;
			//}
			//if(Nod.id[2].BufCount>gBufSize)
			//Nod.id[2].BufCount=0;
		//}
		//SerialPrint(SS, "\r\nend3\r\n");
	}
	return 0;
}
