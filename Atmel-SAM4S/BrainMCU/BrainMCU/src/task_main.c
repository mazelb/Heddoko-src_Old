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
extern drv_uart_config_t uart0Config;
bool toggle, resetSwToggle, recordSwToggle, cpuResetFlag, resetSwSet, recordSwSet;	
uint32_t oldSysTick, newSysTick;
static uint8_t SleepTimerHandle, SystemResetTimerHandle; 
xTimerHandle SleepTimer, SystemResetTimer;
 

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

void vSleepTimerCallback( xTimerHandle xTimer )
{
	drv_gpio_pin_state_t pinState;
	drv_gpio_getPinState(DRV_GPIO_PIN_PW_SW, &pinState);
	if (pinState == DRV_GPIO_PIN_STATE_LOW)
	{
		SleepTimerHandle = 1;
	}
}

void vSystemResetTimerCallback( xTimerHandle xTimer )
{
	drv_gpio_pin_state_t pinState1, pinState2;
	drv_gpio_getPinState(DRV_GPIO_PIN_AC_SW1, &pinState1);
	drv_gpio_getPinState(DRV_GPIO_PIN_AC_SW2, &pinState2);
	//check if both the switches are still pressed
	if ((pinState1 == DRV_GPIO_PIN_STATE_LOW) & (pinState2 == DRV_GPIO_PIN_STATE_LOW))
	{
		//if yes reset the system
		SystemResetTimerHandle = 1;
		//rstc_start_software_reset(RSTC);
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
	

	SleepTimer = xTimerCreate("Sleep Timer", (SLEEP_ENTRY_WAIT_TIME/portTICK_RATE_MS), pdFALSE, NULL, vSleepTimerCallback);
	if (SleepTimer == NULL)
	{
		printf("Failed to create timer task code %d\r\n", SleepTimer);
	}
	
	SystemResetTimer = xTimerCreate("System Reset Timer", (FORCED_SYSTEM_RESET_TIMEOUT/portTICK_RATE_MS), pdFALSE, NULL, vSystemResetTimerCallback);
	if (SystemResetTimer == NULL)
	{
		printf("Failed to create timer task code %d\r\n", SystemResetTimer);
	}
	
	retCode = xTaskCreate(task_quinticHandler, "Q1", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[0], TASK_QUINTIC_PRIORITY, &quinticConfig[0].taskHandle );
	if (retCode != pdPASS)
	{
		printf("Failed to create Q1 task code %d\r\n", retCode);
	}
	//retCode = xTaskCreate(task_quinticHandler, "Q2", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[1], TASK_QUINTIC_STACK_PRIORITY, &quinticConfig[1].taskHandle );
	//if (retCode != pdPASS)
	//{
		//printf("Failed to create Q2 task code %d\r\n", retCode);
	//}
	retCode = xTaskCreate(task_quinticHandler, "Q3", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[2], TASK_QUINTIC_PRIORITY, &quinticConfig[2].taskHandle );
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
	
	printString("Program start\r\n");
	uint8_t interval = 0;
	for (;;) 
	{
		/*	Hardware Test routine	*/
		wdt_restart(WDT);
		checkInputGpio();

		vTaskDelay(100);

		
		
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
				printString("Sleep mode enabled\r\n");
				task_stateMachine_EnqueueEvent(SYS_EVENT_POWER_SWITCH,0); 
			}
			else
			{
				printString("PW SW pressed\r\n");
			}
			newSysTick = oldSysTick = 0;
			SleepTimerHandle = 0;
		}
	}	
	
	if ((drv_gpio_check_Int(DRV_GPIO_PIN_AC_SW1) == 1) | (SystemResetTimerHandle == 1))
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_RECORD_SWITCH,0); 
		if (recordSwToggle == FALSE)
		{
			recordSwSet = TRUE;	//set the flag to as the pin is pulled low
			if (resetSwSet == TRUE)	//check if reset switch was previously pressed
			{
				//initiate the timer as both the switches are pressed
				SystemResetTimerHandle = 0;
				xTimerReset(SystemResetTimer, 0);
			}
			drv_gpio_config_interrupt(DRV_GPIO_PIN_AC_SW1, DRV_GPIO_INTERRUPT_HIGH_EDGE);	//Record pin pressed; configure interrupt for Rising edge
			recordSwToggle = TRUE;
		}
		else if((recordSwToggle == TRUE) | (SystemResetTimerHandle == 1))
		{
			recordSwSet = FALSE;
			xTimerStop(SystemResetTimer, 0);
			drv_gpio_config_interrupt(DRV_GPIO_PIN_AC_SW1, DRV_GPIO_INTERRUPT_LOW_EDGE);	//Record pin released; configure interrupt for Falling edge
			recordSwToggle = FALSE;
			if (SystemResetTimerHandle == 1)
			{
				printString("System reset triggered\r\n");
				rstc_start_software_reset(RSTC);
			}
			else
			{
				task_stateMachine_EnqueueEvent(SYS_EVENT_RECORD_SWITCH,0);
				printString("Record switch pressed\r\n");
			}
			SystemResetTimerHandle = 0;
		}
	}	
	
	if ((drv_gpio_check_Int(DRV_GPIO_PIN_AC_SW2) == 1) | (SystemResetTimerHandle == 1))
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_RESET_SWITCH,0); 		
		if (resetSwToggle == FALSE)
		{
			resetSwSet = TRUE;	//set the flag to as the pin is pulled low
			if (recordSwSet == TRUE)	//check if record switch was previously pressed
			{
				//initiate the timer as both the switches are pressed
				SystemResetTimerHandle = 0;
				xTimerReset(SystemResetTimer, 0);
			}
			drv_gpio_config_interrupt(DRV_GPIO_PIN_AC_SW2, DRV_GPIO_INTERRUPT_HIGH_EDGE);	//Reset pin pressed; configure interrupt for Rising edge
			resetSwToggle = TRUE;
		}
		else if((resetSwToggle == TRUE) | (SystemResetTimerHandle == 1))
		{
			resetSwSet = FALSE;
			xTimerStop(SystemResetTimer, 0);
			drv_gpio_config_interrupt(DRV_GPIO_PIN_AC_SW2, DRV_GPIO_INTERRUPT_LOW_EDGE);	//Reset pin released; configure interrupt for Falling edge
			resetSwToggle = FALSE;
			if (SystemResetTimerHandle == 1)
			{
				printString("System reset triggered\r\n");
				rstc_start_software_reset(RSTC);
			}
			else
			{
				task_stateMachine_EnqueueEvent(SYS_EVENT_RESET_SWITCH,0);
				printString("Reset switch pressed\r\n");
			}
			SystemResetTimerHandle = 0;
		}
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_OC1) == 1)
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_OVER_CURRENT,1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_OC2) == 1)
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_OVER_CURRENT,2);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_DC1) == 1)
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_JACK_DETECT,1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_DC2) == 1)
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_JACK_DETECT,2);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_LBO) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_LOW_BATTERY,0);
	}
	//no idea what to do with this one...	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_STAT) == 1)
	{
		printString("STAT detected\r\n");
		vTaskDelay(1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_SD_CD) == 1)
	{
		drv_gpio_pin_state_t sdCdPinState;
		drv_gpio_getPinState(DRV_GPIO_PIN_SD_CD, &sdCdPinState);
		if (sdCdPinState == DRV_GPIO_PIN_STATE_LOW)
		{
			printString("SD-card removed\r\n");
			//SD card not present, set the respective event
			task_stateMachine_EnqueueEvent(SYS_EVENT_SD_FILE_ERROR,0);
			//reconfigure the SD-card interrupt to look for insertion of card
			drv_gpio_config_interrupt(DRV_GPIO_PIN_SD_CD, DRV_GPIO_INTERRUPT_HIGH_EDGE);
		}
		else if (sdCdPinState == DRV_GPIO_PIN_STATE_HIGH)
		{
			printString("SD-card inserted\r\n");
			//SD card present or inserted, set the respective event
			task_stateMachine_EnqueueEvent(SYS_EVENT_SD_CARD_DETECT,0);
		}
	}
}