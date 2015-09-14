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

#define TASK_LED_STACK_SIZE                (1024/sizeof(portSTACK_TYPE))
#define TASK_LED_STACK_PRIORITY            (tskIDLE_PRIORITY

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);

extern void SysTickHandler(void);

/**
 * startup_test(void)
 * @brief Banner/test function on initialization
 */
static void StartupTest(void) {
	//printf ("Heddoko MCU\n\r");
	SerialPrint(SS, "Heddoko MCU\n\r");
	SerialPutCharUart0('j');
	SerialPutCharUart0('\r');
	SerialPutCharUart0('\n');
	SerialPutCharUsart1('j');
	SerialPutCharUsart1('\r');
	SerialPutCharUsart1('\n');
}

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
static void task_led(void *pvParameters)
{
	UNUSED(pvParameters);
	
	for (;;) {
	/*	Debug code */
	
	// Is button pressed?
	if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
	// Yes, so turn LED on.
	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
	} else {
	// No, so turn LED off.
	ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
	}

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

int main (void)
{	
	char test_file_name[] = "0:sd_mmc_test.txt";
	Ctrl_status status;
	FRESULT res;
	FATFS fs;
	FIL file_object;
	
	sysclk_init();
	board_init();
	
	SerialInit();
	ButtonInit();
	UartUsartInit();
	sd_mmc_init();
	StartupTest();
	
	
	// Fill the NOD buffer with addresses
	for(int i=0; i<NOD_MAX_CNT; i++)
	{
		for(int j=0;j<20;j++)
		{
			Nod.id[i].Addr[j]=sgaNodAddr[i][j];
		}
	}
	
	// Insert application code here, after the board has been initialized.
	Q1Init();
	//ser_print(Q1,"Hello");
	// This skeleton code simply sets the LED to the state of the button.
	
	/* Create task to make led blink */
	//if (xTaskCreate(task_led, "Led", TASK_LED_STACK_SIZE, NULL, TASK_LED_STACK_PRIORITY, NULL ) != pdPASS) {
		////printf("Failed to create test led task\r\n");
		//ser_print(SS,"Task creation failed\r\n");
	//}
	//
	/* Start the scheduler. */
	//vTaskStartScheduler();
	
	/* Wait card present and ready */
	//do {
		//status = sd_mmc_test_unit_ready(0);
		//if (CTRL_FAIL == status) {
			//ser_print(SS, "Card install FAIL\n\r");
			//ser_print(SS, "Please unplug and re-plug the card.\n\r");
			//while (CTRL_NO_PRESENT != sd_mmc_check(0)) {
			//}
		//}
	//} while (CTRL_GOOD != status);
	//
	//ser_print(SS, "Card detected");
	//
	//memset(&fs, 0, sizeof(FATFS));
	//res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
	//if (FR_INVALID_DRIVE != res) {
		//
		//test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
		//res = f_open(&file_object,
		//(char const *)test_file_name,
		//FA_CREATE_ALWAYS | FA_WRITE);
		//if (res = FR_OK) {
			//if (f_puts("Test SD/MMC stack\n", &file_object)!=0) {
				//f_close(&file_object);
			//}
		//}
	//}
	
	/*	Debug code */
	while (1) 
	{		
		SerialPrint(Q1,"send1\r\n");
		cnt=Nod.id[0].ElementCount;
		SerialPrint(SS,"\r\nstart1\r\n");
		while(Nod.id[0].ElementCount<(cnt+21))
		{
			if(Nod.id[0].BufCount<gBufSize)
			ReadUsart1(0);
			if(Nod.id[0].DataCount>=gDataSize)
			{				
				Nod.id[0].ElementCount++;
				Nod.id[0].DataCount=0;
			}
			if(Nod.id[0].ElementCount>=gElementNum)
			{				
				Nod.id[0].BufCount++;
				Nod.id[0].ElementCount=0;
				Nod.id[0].DataCount=0;
			}
			if(Nod.id[0].BufCount>gBufSize)
			Nod.id[0].BufCount=0;
		}
		SerialPrint(SS, "\r\nend1\r\n");
		
		SerialPrint(Q1,"send2\r\n");
		cnt=Nod.id[1].ElementCount;
		SerialPrint(SS, "\r\nstart2\r\n");
		while(Nod.id[1].ElementCount<(cnt+21))
		{
			if(Nod.id[1].BufCount<gBufSize)
			ReadUsart1(1);
			if(Nod.id[1].DataCount>=gDataSize)
			{				
				Nod.id[1].ElementCount++;
				Nod.id[1].DataCount=0;
			}
			if(Nod.id[1].ElementCount>=gElementNum)
			{				
				Nod.id[1].BufCount++;
				Nod.id[1].ElementCount=0;
				Nod.id[1].DataCount=0;
			}
			if(Nod.id[1].BufCount>gBufSize)
			Nod.id[1].BufCount=0;
		}
		SerialPrint(SS, "\r\nend2\r\n");
		
		SerialPrint(Q1,"send3\r\n");
		cnt=Nod.id[2].ElementCount;
		SerialPrint(SS, "\r\nstart3\r\n");
		while(Nod.id[2].ElementCount<(cnt+21))
		{
			if(Nod.id[2].BufCount<gBufSize)
			ReadUsart1(2);
			if(Nod.id[2].DataCount>=gDataSize)
			{				
				Nod.id[2].ElementCount++;
				Nod.id[2].DataCount=0;
			}
			if(Nod.id[2].ElementCount>=gElementNum)
			{				
				Nod.id[2].BufCount++;
				Nod.id[2].ElementCount=0;
				Nod.id[2].DataCount=0;
			}
			if(Nod.id[2].BufCount>gBufSize)
			Nod.id[2].BufCount=0;
		}
		SerialPrint(SS, "\r\nend3\r\n");
	}
	return 0;
}
