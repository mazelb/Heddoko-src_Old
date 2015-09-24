/*
 * task_main.c
 *
 * Created: 9/23/2015 3:33:48 PM
 *  Author: Hriday Mehta
 */ 

#include "task_main.h"
#include "Board_Init.h"
#include "common.h"
#include "Config_Settings.h"
#include "drv_uart.h"
#include "task_quinticInterface.h"
#include "Functionality_Tests.h"
#include <string.h>
#include "DebugLog.h"

extern drv_uart_config_t uart0Config;
extern drv_uart_config_t uart1Config;
extern drv_uart_config_t usart0Config;
extern drv_uart_config_t usart1Config;
extern brainSettings_t brainSettings;

//nodConfiguration array, defined here for now
//has maximum amount of NODs possible is 10
nodConfiguration_t nodConfig[] =
{
	{.macAddress = "1ABBCCDDEEFF", .nodId = 0},
	{.macAddress = "2ABBCCDDEEFF", .nodId = 1},
	{.macAddress = "3ABBCCDDEEFF", .nodId = 2},
	{.macAddress = "3ABBCCDDEEFF", .nodId = 3},
	{.macAddress = "3ABBCCDDEEFF", .nodId = 4},
	{.macAddress = "3ABBCCDDEEFF", .nodId = 5},
	{.macAddress = "3ABBCCDDEEFF", .nodId = 6},
	{.macAddress = "3ABBCCDDEEFF", .nodId = 7},
	{.macAddress = "3ABBCCDDEEFF", .nodId = 8},
	{.macAddress = "3ABBCCDDEEFF", .nodId = 9}
	
};

quinticConfiguration_t qConfig[] =
{
	{
		.nodArray =	{&nodConfig[0],&nodConfig[1],&nodConfig[2]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice = &uart0Config
	},
	{
		.nodArray = {&nodConfig[3],&nodConfig[4],&nodConfig[5]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice = &uart1Config
	},
	{
		.nodArray = {&nodConfig[6],&nodConfig[7],&nodConfig[8]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice = &usart0Config
	}
};

//static function declarations
static status_t initializeNodsAndQuintics();

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
 * \brief This task is initialized first to initiate the board peripherals and run the initial tests
 */
void TaskMain(void *pvParameters)
{
	// This skeleton code simply sets the LED to the state of the button.
	
	UNUSED(pvParameters);
	/*	Create a Semaphore to pass between tasks	*/
	vSemaphoreCreateBinary(DebugLogSemaphore);
	
	powerOnInit();
	
	initializeNodsAndQuintics();
	
	if (xTaskCreate(task_quinticHandler, "Q1", TASK_QUINTIC_STACK_SIZE, (void*)&qConfig[0], TASK_QUINTIC_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create test led task\r\n");
	}
	if (xTaskCreate(task_quinticHandler, "Q2", TASK_QUINTIC_STACK_SIZE, (void*)&qConfig[1], TASK_QUINTIC_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create test led task\r\n");
	}	
		
	for (;;) 
	{
		/*	Debug code */
		// Is button pressed?
		if (ioport_get_pin_level(PIN_SW0_GPIO) == BUTTON_0_ACTIVE)
		{
			// Yes, so turn LED on.
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
		} 
		else
		{
			// No, so turn LED off.
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		}
		vTaskDelay(10);
	}
}


//initializes the structures used by the
static status_t initializeNodsAndQuintics()
{
	status_t status = STATUS_PASS;
	int quinticNodIndex[] = {0,0,0};
	if(brainSettings.isLoaded)
	{
		int i = 0;
		for(i=0; i<brainSettings.numberOfNods; i++)
		{
			nodConfig[i].nodId = brainSettings.nodSettings[i].nodId;
			strncpy(nodConfig[i].macAddress,brainSettings.nodSettings[i].nodMacAddress, 15);
			nodConfig[i].nodValid = true;
			//assign it to a quintic
			//use modulus 3 on the index to determine which quintic gets it.
			qConfig[i%3].nodArray[quinticNodIndex[i%3]++] = &nodConfig[i];
			//maybe assign it in the settings file?
		}
	}
	else
	{
		status = STATUS_FAIL;
	}
	return status;
}
