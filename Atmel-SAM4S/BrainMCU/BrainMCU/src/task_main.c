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
bool pwSwToggle = FALSE, resetSwToggle = FALSE, recordSwToggle = FALSE, cpuResetFlag = FALSE, resetSwSet = FALSE, recordSwSet = FALSE;	
uint32_t oldSysTick, newSysTick;
static uint8_t pwSwitchTimerFlag, SystemResetTimerFlag; 
xTimerHandle SleepTimer, SystemResetTimer;
xTaskHandle fabSenseTaskHandle = NULL, cmdHandlerTaskHandle = NULL, dataHandlerTaskHandle = NULL, sdCardTaskHandle = NULL, stateMachineTaskHandle = NULL;
xTaskHandle timerTaskHandle = NULL;
static int vTaskStackSize[8] = {4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000};

static void checkInputGpio(void);
static void checkRtosStack(int loopCount);

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
		pwSwitchTimerFlag = 1;
		debugPrintString("Sleep mode enabled\r\n");
		task_stateMachine_EnqueueEvent(SYS_EVENT_POWER_SWITCH,0);	
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
		SystemResetTimerFlag = 1;
		//rstc_start_software_reset(RSTC);
	}
}

/**
 * \brief This task is initialized first to initiate the board peripherals and run the initial tests
 */
void TaskMain(void *pvParameters)
{
	int retCode = 0, vLoopCount = 0; 
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
	#ifdef USE_ALL_QUINTICS
	retCode = xTaskCreate(task_quinticHandler, "Q2", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[1], TASK_QUINTIC_PRIORITY, &quinticConfig[1].taskHandle );
	if (retCode != pdPASS)
	{
		printf("Failed to create Q2 task code %d\r\n", retCode);
	}
	#endif
	retCode = xTaskCreate(task_quinticHandler, "Q3", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[2], TASK_QUINTIC_PRIORITY, &quinticConfig[2].taskHandle );
	if (retCode != pdPASS)
	{
		printf("Failed to create Q3 task code %d\r\n", retCode);
	}
	
	retCode = xTaskCreate(task_fabSenseHandler, "FS", TASK_FABSENSE_STACK_SIZE,(void*)&fsConfig, TASK_FABSENSE_PRIORITY, &fabSenseTaskHandle);
	if (retCode != pdPASS)
	{
		printf("Failed to fabric sense task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_commandHandler, "cmd", TASK_SERIAL_RECEIVE_STACK_SIZE,(void*)&cmdConfig, TASK_SERIAL_RECEIVE_PRIORITY, &cmdHandlerTaskHandle );
	if (retCode != pdPASS)
	{
		printf("Failed to Serial handler task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_dataHandler, "DH", TASK_DATA_HANDLER_STACK_SIZE, NULL, TASK_DATA_HANDLER_PRIORITY, &dataHandlerTaskHandle );
	if (retCode != pdPASS)
	{
		printf("Failed to create data handler task code %d\r\n", retCode);
	}	
	retCode = xTaskCreate(task_sdCardHandler, "SD", TASK_SD_CARD_WRITE_STACK_SIZE, NULL, TASK_SD_CARD_WRITE_PRIORITY, &sdCardTaskHandle );
	if (retCode != pdPASS)
	{
		printf("Failed to sd card task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_stateMachineHandler, "SM", TASK_STATE_MACHINE_STACK_SIZE, NULL, TASK_STATE_MACHINE_PRIORITY, &stateMachineTaskHandle );
	if (retCode != pdPASS)
	{
		printf("Failed to sd card task code %d\r\n", retCode);
	}
	
	uint8_t interval = 0;
	for (;;) 
	{
		/*	Hardware Test routine	*/
		wdt_restart(WDT);
		checkInputGpio();
		if (getCurrentState() != (SYS_STATE_OFF))
		{
			checkRtosStack(vLoopCount);
			(vLoopCount)++;
			 if (vLoopCount > 7)
			 {
				 vLoopCount = 0;
			 }
		}
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
	//TODO maybe the enqueueing of event should be done in the interrupts??
	if ((drv_gpio_check_Int(DRV_GPIO_PIN_PW_SW) == 1) || (pwSwitchTimerFlag == 1))
	{
		unsigned long PinFlag;		
		if (pwSwToggle == FALSE)
		{
			pwSwitchTimerFlag = 0;
			drv_gpio_config_interrupt(DRV_GPIO_PIN_PW_SW, DRV_GPIO_INTERRUPT_HIGH_EDGE);	//Power pin pressed; configure interrupt for Rising edge
			xTimerReset(SleepTimer, 0);
			pwSwToggle = TRUE;
		}
		else if((pwSwToggle == TRUE)||(pwSwitchTimerFlag == 1))
		{
			xTimerStop(SleepTimer, 0);
			drv_gpio_config_interrupt(DRV_GPIO_ID_PIN_PW_SW, DRV_GPIO_INTERRUPT_LOW_EDGE);	//Power pin released; configure interrupt for Falling edge
			pwSwToggle = FALSE;
			if (pwSwitchTimerFlag != 1)
			{
				debugPrintString("PW SW pressed\r\n");
			}
			pwSwitchTimerFlag = 0;
		}
	}	
	
	if ((drv_gpio_check_Int(DRV_GPIO_PIN_AC_SW1) == 1) || (SystemResetTimerFlag == 1))
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_RECORD_SWITCH,0); 
		if (recordSwToggle == FALSE)
		{
			recordSwSet = TRUE;	//set the flag to as the pin is pulled low
			if (resetSwSet == TRUE)	//check if reset switch was previously pressed
			{
				//initiate the timer as both the switches are pressed
				SystemResetTimerFlag = 0;
				xTimerReset(SystemResetTimer, 0);
			}
			drv_gpio_config_interrupt(DRV_GPIO_PIN_AC_SW1, DRV_GPIO_INTERRUPT_HIGH_EDGE);	//Record pin pressed; configure interrupt for Rising edge
			recordSwToggle = TRUE;
		}
		else if((recordSwToggle == TRUE) || (SystemResetTimerFlag == 1))
		{
			recordSwSet = FALSE;
			xTimerStop(SystemResetTimer, 0);
			drv_gpio_config_interrupt(DRV_GPIO_PIN_AC_SW1, DRV_GPIO_INTERRUPT_LOW_EDGE);	//Record pin released; configure interrupt for Falling edge
			recordSwToggle = FALSE;
			if (SystemResetTimerFlag == 1)
			{
				debugPrintString("System reset triggered\r\n");
				vTaskDelay(200); //wait for the log to be written
				rstc_start_software_reset(RSTC);
			}
			else
			{
				task_stateMachine_EnqueueEvent(SYS_EVENT_RECORD_SWITCH,0);
				debugPrintString("Record switch pressed\r\n");
			}
			SystemResetTimerFlag = 0;
		}
	}	
	
	if ((drv_gpio_check_Int(DRV_GPIO_PIN_AC_SW2) == 1) || (SystemResetTimerFlag == 1))
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_RESET_SWITCH,0); 		
		if (resetSwToggle == FALSE)
		{
			resetSwSet = TRUE;	//set the flag to as the pin is pulled low
			if (recordSwSet == TRUE)	//check if record switch was previously pressed
			{
				//initiate the timer as both the switches are pressed
				SystemResetTimerFlag = 0;
				xTimerReset(SystemResetTimer, 0);
			}
			drv_gpio_config_interrupt(DRV_GPIO_PIN_AC_SW2, DRV_GPIO_INTERRUPT_HIGH_EDGE);	//Reset pin pressed; configure interrupt for Rising edge
			resetSwToggle = TRUE;
		}
		else if((resetSwToggle == TRUE) || (SystemResetTimerFlag == 1))
		{
			resetSwSet = FALSE;
			xTimerStop(SystemResetTimer, 0);
			drv_gpio_config_interrupt(DRV_GPIO_PIN_AC_SW2, DRV_GPIO_INTERRUPT_LOW_EDGE);	//Reset pin released; configure interrupt for Falling edge
			resetSwToggle = FALSE;
			if (SystemResetTimerFlag == 1)
			{
				debugPrintString("System reset triggered\r\n");
				rstc_start_software_reset(RSTC);
			}
			else
			{
				task_stateMachine_EnqueueEvent(SYS_EVENT_RESET_SWITCH,0);
				debugPrintString("Reset switch pressed\r\n");
			}
			SystemResetTimerFlag = 0;
		}
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_OC1) == 1)
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_OVER_CURRENT,1);
		debugPrintString("Jack 1 Over current\r\n");
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_OC2) == 1)
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_OVER_CURRENT,2);
		debugPrintString("Jack 2 Over current\r\n");
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_DC1) == 1)
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_JACK_DETECT,1);
		debugPrintString("Jack 1 inserted\r\n");
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_DC2) == 1)
	{
		//task_stateMachine_EnqueueEvent(SYS_EVENT_JACK_DETECT,2);
		debugPrintString("Jack 2 inserted\r\n");
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_LBO) == 1)
	{
		debugPrintString("Battery Low\r\n");
		task_stateMachine_EnqueueEvent(SYS_EVENT_LOW_BATTERY,0);
	}
	//no idea what to do with this one...	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_STAT) == 1)
	{
		debugPrintString("STAT detected\r\n");
		vTaskDelay(1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_SD_CD) == 1)
	{
		drv_gpio_pin_state_t sdCdPinState;
		drv_gpio_getPinState(DRV_GPIO_PIN_SD_CD, &sdCdPinState);
		if (sdCdPinState == DRV_GPIO_PIN_STATE_LOW)
		{
			debugPrintString("SD-card removed\r\n");
			//SD card not present, set the respective event
			task_stateMachine_EnqueueEvent(SYS_EVENT_SD_FILE_ERROR,0);
			//reconfigure the SD-card interrupt to look for insertion of card
			drv_gpio_config_interrupt(DRV_GPIO_PIN_SD_CD, DRV_GPIO_INTERRUPT_HIGH_EDGE);
		}
		else if (sdCdPinState == DRV_GPIO_PIN_STATE_HIGH)
		{
			debugPrintString("SD-card inserted\r\n");
			//SD card present or inserted, set the respective event
			task_stateMachine_EnqueueEvent(SYS_EVENT_SD_CARD_DETECT,0);
		}
	}
}

/***********************************************************************************************
 * checkRtosStack(void)
 * @brief Check for Stack overflow for all Tasks on kernel
 * @param void
 * @return void
 ***********************************************************************************************/
static void checkRtosStack(int loopCount)
{
	unsigned portBASE_TYPE vHighWaterMark;
 	switch (loopCount)
 	{
	 	case 0:
	 		vHighWaterMark = uxTaskGetStackHighWaterMark(quinticConfig[0].taskHandle);
	 		//if (vHighWaterMark < 100)
	 		//{
				 if (vHighWaterMark < vTaskStackSize[0])
				 {
					 debugPrintStringInt("Quintic task Q0 stack new high water mark\r\n", vHighWaterMark);
					 vTaskStackSize[0] = vHighWaterMark;
				 }
	 		//}
	 	break;
	 	case 1:
		 #ifdef USE_ALL_QUINTICS
		 	vHighWaterMark = uxTaskGetStackHighWaterMark(quinticConfig[1].taskHandle);
		 	//if (vHighWaterMark < 100)
		 	//{
			 	 if (vHighWaterMark < vTaskStackSize[1])
			 	 {
				 	 debugPrintStringInt("Quintic task Q1 stack new high water mark\r\n", vHighWaterMark);
				 	 vTaskStackSize[1] = vHighWaterMark;
			 	 }
		 	//}
		#endif
	 	break;
		case 2:
			vHighWaterMark = uxTaskGetStackHighWaterMark(quinticConfig[2].taskHandle);
			//if (vHighWaterMark < 100)
			//{
				if (vHighWaterMark < vTaskStackSize[2])
				{
					debugPrintStringInt("Quintic task Q2 stack new high water mark\r\n", vHighWaterMark);
					vTaskStackSize[2] = vHighWaterMark;
				}
			//}
		break;
		case 3:
		 	vHighWaterMark = uxTaskGetStackHighWaterMark(fabSenseTaskHandle);
		 	//if (vHighWaterMark < 100)
		 	//{
				if (vHighWaterMark < vTaskStackSize[3])
				{
			 		debugPrintStringInt("Fabric Sense task stack new high water mark\r\n", vHighWaterMark);
					vTaskStackSize[3] = vHighWaterMark;
				}
		 	//}
	 	break;
		case 4:
			vHighWaterMark = uxTaskGetStackHighWaterMark(cmdHandlerTaskHandle);
			//if (vHighWaterMark < 100)
			//{
				if (vHighWaterMark < vTaskStackSize[4])
				{
					debugPrintStringInt("Command-Handler task stack new high water mark\r\n", vHighWaterMark);
					vTaskStackSize[4] = vHighWaterMark;
				}
			//}
		break;
		case 5:
			vHighWaterMark = uxTaskGetStackHighWaterMark(dataHandlerTaskHandle);
			//if (vHighWaterMark < 100)
			//{
				if (vHighWaterMark < vTaskStackSize[5])
				{
					debugPrintStringInt("Data-Handler task stack new high water mark\r\n", vHighWaterMark);
					vTaskStackSize[5] = vHighWaterMark;
				}
			//}
		break;
		case 6:
			vHighWaterMark = uxTaskGetStackHighWaterMark(sdCardTaskHandle);
			//if (vHighWaterMark < 100)
			//{
				if (vHighWaterMark < vTaskStackSize[6])
				{
					debugPrintStringInt("SD-card task stack new high water mark\r\n", vHighWaterMark);
					vTaskStackSize[6] = vHighWaterMark;
				}
			//}
		break;
		case 7:
			vHighWaterMark = uxTaskGetStackHighWaterMark(stateMachineTaskHandle);
			//if (vHighWaterMark < 100)
			//{
				if (vHighWaterMark < vTaskStackSize[7])
				{
					debugPrintStringInt("State-Machine task stack new high water mark\r\n", vHighWaterMark);
					vTaskStackSize[7] = vHighWaterMark;
				}
			//}
		break;
		default:
		break;
 	}
}