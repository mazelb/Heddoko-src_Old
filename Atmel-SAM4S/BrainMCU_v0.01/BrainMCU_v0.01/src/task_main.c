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
#include "task_dataProcessor.h"
#include "Functionality_Tests.h"
#include <string.h>
#include "DebugLog.h"

extern drv_uart_config_t uart0Config;
extern drv_uart_config_t uart1Config;
extern drv_uart_config_t usart0Config;
extern drv_uart_config_t usart1Config;
extern brainSettings_t brainSettings;

//imuConfiguration array, defined here for now
//has maximum amount of NODs possible is 10
imuConfiguration_t imuConfig[] =
{
	{.macAddress = "1ABBCCDDEEFF", .imuId = 0},
	{.macAddress = "2ABBCCDDEEFF", .imuId = 1},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 2},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 3},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 4},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 5},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 6},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 7},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 8},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 9}
	
};

quinticConfiguration_t qConfig[] =
{
	{
		.imuArray =	{&imuConfig[0],&imuConfig[1],&imuConfig[2]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice = &uart0Config
	},
	{
		.imuArray = {&imuConfig[3],&imuConfig[4],&imuConfig[5]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice = &uart1Config
	},
	{
		.imuArray = {&imuConfig[6],&imuConfig[7],&imuConfig[8]},
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

status_t processCommand(char* command, size_t cmdSize)
{
	status_t status = STATUS_PASS; 
	if(strncmp(command, "SDCardTest\r\n",cmdSize) == 0)
	{
		printf("received the SD card test command\r\n");				
	}
	else if(strncmp(command, "dataBoardGpioTest\r\n",cmdSize) == 0)
	{
		printf("received the GPIO test command\r\n");
	}
	else if(strncmp(command, "BLE Test\r\n",cmdSize) == 0)
	{
		printf("received the GPIO test command\r\n",cmdSize);
	}
	else
	{
		printf("Received unknown command: %s \r\n", command);
		status = STATUS_PASS; 
	}
	return status;	
}
/**
 * \brief This task, when started will loop back \r\n terminated strings
 */
static void task_serialReceiveTest(void *pvParameters)
{
	UNUSED(pvParameters);
	int result = 0;
	char buffer[100] = {0};
	int pointer = 0;
	//char val = 0xA5; 
	while(1)
	{
		if(drv_uart_getline(&uart1Config,buffer,sizeof(buffer)) == STATUS_PASS)
		{
			processCommand(buffer,sizeof(buffer)); 
		}
		
		vTaskDelay(10);
	}
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
	if (xTaskCreate(task_dataHandler, "dataHandler", TASK_DATA_HANDLER_STACK_SIZE, NULL, TASK_DATA_HANDLER_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create data handler task\r\n");
	}	
	if (xTaskCreate(task_quinticHandler, "Q1", TASK_QUINTIC_STACK_SIZE, (void*)&qConfig[0], TASK_QUINTIC_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create Q1 task task\r\n");
	}
	if (xTaskCreate(task_quinticHandler, "Q2", TASK_QUINTIC_STACK_SIZE, (void*)&qConfig[1], TASK_QUINTIC_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create Q2 task\r\n");
	}
	if (xTaskCreate(task_quinticHandler, "Q3", TASK_QUINTIC_STACK_SIZE, (void*)&qConfig[2], TASK_QUINTIC_STACK_PRIORITY, NULL ) != pdPASS)
	{
		printf("Failed to create Q3 task\r\n");
	}
	//if (xTaskCreate(task_serialReceiveTest, "Serial_Task", TASK_QUINTIC_STACK_SIZE,NULL, TASK_QUINTIC_STACK_PRIORITY+5, NULL ) != pdPASS)
	//{
		//printf("Failed to create test led task\r\n");
	//}
		
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
		vTaskDelay(1000);
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
		for(i=0; i<brainSettings.numberOfImus; i++)
		{
			imuConfig[i].imuId = brainSettings.imuSettings[i].imuId;
			strncpy(imuConfig[i].macAddress,brainSettings.imuSettings[i].imuMacAddress, 15);
			imuConfig[i].imuValid = true;
			//assign it to a quintic
			//use modulus 3 on the index to determine which quintic gets it.
			qConfig[i%3].imuArray[quinticNodIndex[i%3]++] = &imuConfig[i];
			//maybe assign it in the settings file?
		}
	}
	else
	{
		status = STATUS_FAIL;
	}
	return status;
}
