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
//#include "conf_example.h"
#include "conf_board.h"
#include "BrainMCU.h"
#include "Tasks.h"
#include "Ser.h"
#include "Commands.h"
#include "GPIO.h"
#include "drv_uart.h"
#include "task_quinticInterface.h"

#define TASK_SERIAL_RECEIVE_STACK_SIZE                (2048/sizeof(portSTACK_TYPE))
#define TASK_SERIAL_RECEIVE_STACK_PRIORITY            (tskIDLE_PRIORITY)
#define ACCESS_MEM_TO_RAM_ENABLED

/** Baudrate setting : 115200 */
#define CONF_BAUDRATE   115200
/** Char setting     : 8-bit character length (don't care for UART) */
#define CONF_CHARLENGTH 0
/** Parity setting   : No parity check */
#define CONF_PARITY     UART_MR_PAR_NO
/** Stopbit setting  : No extra stopbit, i.e., use 1 (don't care for UART) */
#define CONF_STOPBITS   false


extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void xPortSysTickHandler(void);
extern void SysTickHandler(void);

static char sgaNodAddr[9][15]={"A0E5E900133B\r\n",
								"A0E5E90016DD\r\n",
								"A0E5E90012CD\r\n",
								"A0E5E900139B\r\n",
								"A0E5E9001717\r\n",
								"A0E5E90011FD\r\n",
								"A0E5E90016E6\r\n",
								"A0E5E9001398\r\n",
								"A0E5E9000387\r\n"
};

drv_uart_config_t uart0Config =
{
	.p_usart = UART0,
	.mem_index = 0,
	.uart_options =
	{
		.baudrate   = CONF_BAUDRATE,
		.charlength = CONF_CHARLENGTH,
		.paritytype = CONF_PARITY,
		.stopbits   = CONF_STOPBITS
	}
};
drv_uart_config_t uart1Config =
{
	.p_usart = UART1,
	.mem_index = 1,
	.uart_options =
	{
		.baudrate   = CONF_BAUDRATE,
		.charlength = CONF_CHARLENGTH,
		.paritytype = CONF_PARITY,
		.stopbits   = CONF_STOPBITS
	}
};
drv_uart_config_t usart0Config =
{
	.p_usart = USART0,
	.mem_index = 0,
	.uart_options =
	{
		.baudrate   = CONF_BAUDRATE,
		.charlength = CONF_CHARLENGTH,
		.paritytype = CONF_PARITY,
		.stopbits   = CONF_STOPBITS
	}
};
drv_uart_config_t usart1Config =
{
	.p_usart = USART1,
	.mem_index = 0,
	.uart_options =
	{
		.baudrate   = CONF_BAUDRATE,
		.charlength = CONF_CHARLENGTH,
		.paritytype = CONF_PARITY,
		.stopbits   = CONF_STOPBITS
	}
};

quinticConfiguration_t q1 = 
{
	.nodArray =
	{
		{.macAddress = "1ABBCCDDEEFF"},
		{.macAddress = "2ABBCCDDEEFF"},
		{.macAddress = "3ABBCCDDEEFF"},	
	},
	.expectedNumberOfNods = 3,
	.isinit = 0,
	.uartDevice = &uart0Config
};

quinticConfiguration_t q2 =
{
	.nodArray =
	{
		{.macAddress = "4ABBCCDDEEFF"},
		{.macAddress = "5ABBCCDDEEFF"},
		{.macAddress = "6ABBCCDDEEFF"},
	},
	.expectedNumberOfNods = 3,
	.isinit = 0,
	.uartDevice = &uart1Config
};

/**
 * \brief Called if stack overflow during execution
 */
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed char *pcTaskName)
{
	//printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;) {
	}
}

/**
 * \brief This function is called by FreeRTOS idle task
 */
extern void vApplicationIdleHook(void)
{
}

/**
 * \brief This function is called by FreeRTOS each tick
 */
extern void vApplicationTickHook(void)
{
}

/**
 * \brief This task, when activated, make LED blink at a fixed rate
 */
static void task_serialReceiveTest(void *pvParameters)
{
	UNUSED(pvParameters);
	int result = 0;
	char buffer[100] = {0};
	int pointer = 0;
	char val = 0xA5; 
	while(1)
	{
		result = drv_uart_getChar(&uart1Config,&val);
		if(result != STATUS_EOF && val != NULL)
		{
			//
			if(pointer < sizeof(buffer))
			{
				buffer[pointer++] = val; //add the char to the temporary buffer
				if(val == '\r')
				{
					buffer[pointer] = NULL; //terminate the string
					SerialPrint(SS,buffer);
					vTaskDelay(3000);
					pointer = 0; //reset the pointer.
				}				
			}
			else
			{
				SerialPrint(SS, "Error buffer full \n\r"); 
				pointer = 0;
			}
		}
		vTaskDelay(10);
	}
}

/**
 * \brief Configure the console UART.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#if (defined CONF_UART_CHAR_LENGTH)
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#if (defined CONF_UART_STOP_BITS)
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);

	/* Specify that stdout should not be buffered. */
#if defined(__GNUC__)
	setbuf(stdout, NULL);
#else
	/* Already the case in IAR's Normal DLIB default configuration: printf()
	 * emits one character at a time.
	 */
#endif
}
/**
 * \brief Handler for Sytem Tick interrupt.
 */
void SysTick_Handler(void)
{
	xPortSysTickHandler();
}
/*
void UART1_Handler()
{
	uint32_t val = 0;
	while((UART1->UART_SR & UART_SR_RXRDY) == 0);
	usart_getchar(UART1,&val);	
	usart_write(UART1, val);  	
}
*/
/** USART Interface  : Console UART */
#define CONF_TEST_USART      CONSOLE_UART
/** Baudrate setting : 115200 */
#define CONF_TEST_BAUDRATE   115200
/** Char setting     : 8-bit character length (don't care for UART) */
#define CONF_TEST_CHARLENGTH 0
/** Parity setting   : No parity check */
#define CONF_TEST_PARITY     UART_MR_PAR_NO
/** Stopbit setting  : No extra stopbit, i.e., use 1 (don't care for UART) */
#define CONF_TEST_STOPBITS   false


int main (void)
{	
	char test_file_name[] = "0:sd_mmc_test1.txt";
	Ctrl_status status;
	FRESULT res;
	FATFS fs;
	FIL file_object;
	
	irq_initialize_vectors();
	cpu_irq_enable();

	sysclk_init();
	board_init();

	//SerialInit();
	//ButtonInit();
	//UartUsartInit();
	//stdio_serial_init(UART1, &usart_serial_options);
	//sd_mmc_init();
	//StartupTest();
	
	
	if(drv_uart_init(&uart0Config) != STATUS_PASS)
	{
		while(1); //spin here
	}
	if(drv_uart_init(&uart1Config) != STATUS_PASS)
	{
		while(1); //spin here
	}
	if(drv_uart_init(&usart0Config) != STATUS_PASS)
	{
		while(1); //spin here
	}
	if(drv_uart_init(&usart1Config) != STATUS_PASS)
	{
		while(1); //spin here
	}		
	/* Create task to make led blink */
	//if (xTaskCreate(task_serialReceiveTest, "Led", TASK_SERIAL_RECEIVE_STACK_SIZE, NULL, TASK_SERIAL_RECEIVE_STACK_PRIORITY, NULL ) != pdPASS) 
	//{
		//printf("Failed to create test led task\r\n");
		////ser_print(SS,"Task creation failed\r\n");
	//}

	if (xTaskCreate(task_quinticHandler, "Q1", TASK_QUINTIC_STACK_SIZE, (void*)&q1, TASK_MONITOR_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create test led task\r\n");

	}
	if (xTaskCreate(task_quinticHandler, "Q2", TASK_QUINTIC_STACK_SIZE, (void*)&q2, TASK_MONITOR_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create test led task\r\n");

	}

	
	char buf[200] = {0};
	snprintf(buf,sizeof(buf), "Starting RTOS \n\r");
	
	SerialPrint(SS,buf);

	/* Start the scheduler. */
	vTaskStartScheduler();

	while(1); 

	
	return 0;
}
