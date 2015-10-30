/*
 * task_main.c
 *
 * Created: 9/23/2015 3:33:48 PM
 *  Author: Hriday Mehta
 */ 

#include "task_main.h"
#include "Board_Init.h"
#include "common.h"
#include "settings.h"
#include "drv_uart.h"
#include "drv_gpio.h"
#include "task_quinticInterface.h"
#include "task_dataProcessor.h"
#include "task_fabricSense.h"
#include "task_sdCardWrite.h"
#include "task_stateMachine.h"
#include "task_commandProc.h"
#include "Functionality_Tests.h"
#include <string.h>
#include "DebugLog.h"
#include "drv_led.h"



extern xQueueHandle queue_dataHandler;
extern uint32_t totalBytesWritten; 
extern uint32_t totalFramesWritten;
extern unsigned long sgSysTickCount;
extern quinticConfiguration_t quinticConfig[];
extern imuConfiguration_t imuConfig[];
extern fabricSenseConfig_t fsConfig; 
extern commandProcConfig_t cmdConfig; 
bool toggle;
uint32_t oldSysTick, newSysTick;
static uint8_t SleepTimerHandle; 
xTimerHandle SleepTimer;
 

static void checkInputGpio(void);

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



//TEMP REMOVE THIS
extern FIL dataLogFile_obj; 

void vTimerCallback( xTimerHandle xTimer )
{
	drv_gpio_pin_state_t pinState;
	drv_gpio_getPinState(DRV_GPIO_PIN_PW_SW, &pinState);
	if (pinState == DRV_GPIO_PIN_STATE_LOW)
	{
		SleepTimerHandle = 1;
	}
}

/**
 * \brief This task is initialized first to initiate the board peripherals and run the initial tests
 */
void TaskMain(void *pvParameters)
{
	int retCode = 0; 
	UNUSED(pvParameters);
	/*	Create a Semaphore to pass between tasks	*/
	//vSemaphoreCreateBinary(DebugLogSemaphore);
	powerOnInit();
	
	//reset the quintics
	//drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST1, DRV_GPIO_PIN_STATE_LOW);
	SleepTimer = xTimerCreate("Sleep Timer", (5000/portTICK_RATE_MS), pdFALSE, NULL, vTimerCallback);
	if (SleepTimer == NULL)
	{
		printf("Failed to create timer task code %d\r\n", SleepTimer);
	}
	
	retCode = xTaskCreate(task_quinticHandler, "Q1", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[0], TASK_QUINTIC_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to create Q1 task code %d\r\n", retCode);
	}
	//retCode = xTaskCreate(task_quinticHandler, "Q2", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[1], TASK_QUINTIC_STACK_PRIORITY, NULL );
	//if (retCode != pdPASS)
	//{
		//printf("Failed to create Q2 task code %d\r\n", retCode);
	//}
	retCode = xTaskCreate(task_quinticHandler, "Q3", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[2], TASK_QUINTIC_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to create Q3 task code %d\r\n", retCode);
	}
	
	retCode = xTaskCreate(task_fabSenseHandler, "FS", TASK_FABSENSE_STACK_SIZE,(void*)&fsConfig, TASK_FABSENSE_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to fabric sense task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_commandHandler, "cmd", TASK_SERIAL_RECEIVE_STACK_SIZE,(void*)&cmdConfig, TASK_SERIAL_RECEIVE_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to Serial handler task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_dataHandler, "DH", TASK_DATA_HANDLER_STACK_SIZE, NULL, TASK_DATA_HANDLER_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to create data handler task code %d\r\n", retCode);
	}	
	retCode = xTaskCreate(task_sdCardHandler, "SD", TASK_SD_CARD_WRITE_STACK_SIZE, NULL, TASK_SD_CARD_WRITE_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to sd card task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_stateMachineHandler, "SM", TASK_STATE_MACHINE_STACK_SIZE, NULL, TASK_STATE_MACHINE_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to sd card task code %d\r\n", retCode);
	}
	
	printf("Program start\r\n");
	uint8_t interval = 0;
	for (;;) 
	{
		/*	Hardware Test routine	*/
		checkInputGpio();
		
		vTaskDelay(250);
		//res = f_write(&log_file_object,testData ,sizeof(testData), &numBytes);
		//ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
		//res = f_sync(&log_file_object); //sync the file
		//ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		//
		//
		
		
	}
}

/***********************************************************************************************
 * checkInputGpio(void)
 * @brief Check for interrupt flags on every GPIO pins, process them and raise State machine events
 * @param 
 * @return 
 ***********************************************************************************************/
static void checkInputGpio(void)
{
	//TODO maybe the enqueing of event should be done in the interrupts??
	if ((drv_gpio_check_Int(DRV_GPIO_PIN_PW_SW) == 1) | (SleepTimerHandle == 1))
	{
		unsigned long PinFlag;
		
		if (toggle == FALSE)
		{
			SleepTimerHandle = 0;
			xTimerStop(SleepTimer, 0);
			xTimerReset(SleepTimer, 0);
			oldSysTick = sgSysTickCount;
			drv_gpio_config_interrupt(DRV_GPIO_PIN_PW_SW, DRV_GPIO_INTERRUPT_HIGH_EDGE);	//Power pin pressed; configure interrupt for Rising edge
			xTimerStart(SleepTimer, 0);
			toggle = TRUE;
		}
		else if((toggle == TRUE)|(SleepTimerHandle == 1))
		{
			xTimerStop(SleepTimer, 0);
			newSysTick = sgSysTickCount;
			drv_gpio_config_interrupt(DRV_GPIO_ID_PIN_PW_SW, DRV_GPIO_INTERRUPT_LOW_EDGE);	//Power pin released; configure interrupt for Falling edge
			toggle = FALSE;
			if (SleepTimerHandle == 1)
			{
				printf("Sleep mode enabled\r\n");
				task_stateMachine_EnqueueEvent(SYS_EVENT_POWER_SWITCH,0); 
			}
			else
			{
				printf("PW SW pressed\r\n");
			}
			newSysTick = oldSysTick = 0;
			SleepTimerHandle = 0;
		}
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_AC_SW1) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_RECORD_SWITCH,0); 
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_AC_SW2) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_RESET_SWITCH,0); 
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_OC1) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_OVER_CURRENT,1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_OC2) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_OVER_CURRENT,2);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_DC1) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_JACK_DETECT,1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_DC2) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_JACK_DETECT,2);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_LBO) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_LOW_BATTERY,0);
	}
	//no idea what to do with this one...	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_STAT) == 1)
	{
		printf("STAT detected\r\n");
		vTaskDelay(1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_SD_CD) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_SD_CARD_DETECT,0);
	}
}