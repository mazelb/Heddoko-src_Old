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

#define TASK_SERIAL_RECEIVE_STACK_SIZE                (2048/sizeof(portSTACK_TYPE))
#define TASK_SERIAL_RECEIVE_STACK_PRIORITY            (tskIDLE_PRIORITY)
#define ACCESS_MEM_TO_RAM_ENABLED
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void xPortSysTickHandler(void);
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
static void task_serialReceiveTest(void *pvParameters)
{
	UNUSED(pvParameters);
	int val = 0;
	char buffer[100] = {0};
	int pointer = 0;
	
	while(1)
	{
		val = SerialGetCharUart1nb();
		if(val != EOF && val != NULL)
		{
			//
			if(pointer < sizeof(buffer))
			{
				buffer[pointer++] = (char)val; //add the char to the temporary buffer
				if((char)val == '\r')
				{
					buffer[pointer] = NULL; //terminate the string
					SerialPrint(SS,buffer);
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
	const usart_serial_options_t usart_serial_options =
	{
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS,
	};
	SerialInit();
	//ButtonInit();
	UartUsartInit();
	stdio_serial_init(UART1, &usart_serial_options);
	//sd_mmc_init();
	//StartupTest();
	
	
	// Fill the NOD buffer with addresses
	for(int i=0; i<NOD_MAX_CNT; i++)
	{
		for(int j=0;j<20;j++)
		{
			Nod.id[i].Addr[j]=sgaNodAddr[i][j];
		}
	}
	
	// Insert application code here, after the board has been initialized.
	//Q1Init();
	//ser_print(Q1,"Hello");
	// This skeleton code simply sets the LED to the state of the button.
	
	/* Create task to make led blink */
	if (xTaskCreate(task_serialReceiveTest, "Led", TASK_SERIAL_RECEIVE_STACK_SIZE, NULL, TASK_SERIAL_RECEIVE_STACK_PRIORITY, NULL ) != pdPASS) 
	{
		printf("Failed to create test led task\r\n");
		//ser_print(SS,"Task creation failed\r\n");
	}
	
	char buf[200] = {0};
	snprintf(buf,sizeof(buf), "Starting RTOS \n\r");
	
	SerialPrint(SS,buf);

	/* Start the scheduler. */
	vTaskStartScheduler();

	while(1); 
	
	/* Wait card present and ready */


	do
	{		
		status = sd_mmc_test_unit_ready(0);
		if (CTRL_FAIL == status)
		{			
			SerialPrint(SS, "Card install FAIL\n\r");
			SerialPrint(SS, "Please unplug and re-plug the card.\n\r");
			while (CTRL_NO_PRESENT != sd_mmc_check(0))
			{
			}
		}
	} while (CTRL_GOOD != status);
	
	SerialPrint(SS, "Card detected");

	memset(&fs, 0, sizeof(FATFS));
	res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
	if (FR_INVALID_DRIVE != res)
	{
		
		test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
		res = f_open(&file_object,
		(char const *)test_file_name,
		FA_CREATE_ALWAYS | FA_WRITE);
		//f_puts("Test SD/MMC stack\n", ); 
		uint8_t buf[] = "Hello this is a test\r\n"; 
		unsigned int numBytes = 0; 
		
		if (res == FR_OK)
		{
			res = f_write (&file_object, buf, sizeof(buf), &numBytes);
			if(res == FR_OK)
			{
				printf("Write successful! Number of bytes written: %d\r\n", numBytes);
			}
			else
			{
				printf("write Failed with code %d\r\n", res);
				
			}
			//if (f_puts("Test SD/MMC stack\n", &file_object)!=0)
			//{
				//
			//}
		}
	}
	f_close(&file_object);
	//printf("\x0C\n\r-- SD/MMC/SDIO Card Example on FatFs --\n\r");
	//printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
	//while (1)
	//{
		//
		//printf("Please plug an SD, MMC or SDIO card in slot.\n\r");
//
		///* Wait card present and ready */
		//do 
		//{
			//status = sd_mmc_test_unit_ready(0);
			//if (CTRL_FAIL == status) 
			//{
				//printf("Card install FAIL\n\r");
				//printf("Please unplug and re-plug the card.\n\r");
				//while (CTRL_NO_PRESENT != sd_mmc_check(0)) 
				//{
				//}
			//}
		//} while (CTRL_GOOD != status);
//
		//printf("Mount disk (f_mount)...\r\n");
		//memset(&fs, 0, sizeof(FATFS));
		//res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
		//if (FR_INVALID_DRIVE == res) 
		//{
			//printf("[FAIL] res %d\r\n", res);
			//goto main_end_of_test;
		//}
		//printf("[OK]\r\n");
//
		//printf("Create a file (f_open)...\r\n");
		//test_file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
		//res = f_open(&file_object,
		//(char const *)test_file_name,
		//FA_CREATE_ALWAYS | FA_WRITE);
		//if (res != FR_OK) 
		//{
			//printf("[FAIL] res %d\r\n", res);
			//goto main_end_of_test;
		//}
		//printf("[OK]\r\n");
		////res = f_puts("Test SD/MMC stack\n", &file_object); 
		//res = f_printf( &file_object, "Hello world\r\n"); 
		//printf("Write to test file (f_puts)...\r\n");
		///*
		//if (res == 0) 
		//{
			//f_close(&file_object);
			//printf("[FAIL]\r\n");
			//goto main_end_of_test;
		//}
		//*/
		//
		//printf("[OK]\r\n");
		//f_close(&file_object);
		//printf("Test is successful.\n\r");
//
		//main_end_of_test:
		//printf("Please unplug the card.\n\r");
		//while (CTRL_NO_PRESENT != sd_mmc_check(0)) 
		//{
		//}
	//}



	while(1); //stay here forever

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
