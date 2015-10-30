/*
 * task_stateMachine.c
 *
 * Created: 10/19/2015 9:09:03 AM
 *  Author: Sean Cloghesy
 */ 

#include <asf.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task_stateMachine.h"
#include "task_quinticInterface.h"
#include "task_fabricSense.h"
#include "task_dataProcessor.h"
#include "task_sdCardWrite.h"
#include "drv_gpio.h"
#include "Board_Init.h"
#include "drv_led.h"
#include "settings.h"

#define LED_LEVEL_OFF	DRV_GPIO_PIN_STATE_HIGH
#define LED_LEVEL_ON	DRV_GPIO_PIN_STATE_LOW
//queue to store events
xQueueHandle queue_stateMachineEvents = NULL;
systemStates_t currentSystemState = SYS_STATE_OFF;
//external variables
extern quinticConfiguration_t quinticConfig[];
extern fabricSenseConfig_t fsConfig;
extern unsigned long sgSysTickCount;
drv_gpio_pin_state_t pwSwState;
uint8_t ResetStatus; //of what???????
uint8_t QResetCount;
extern drv_uart_config_t uart0Config;
extern brainSettings_t brainSettings;
//Reset task handle
xTaskHandle ResetHandle = NULL;

//static function forward declarations
void processEvent(eventMessage_t eventMsg);
void stateEntry_PowerDown();
void stateEntry_Reset();
void stateExit_Reset();
void stateEntry_Idle();
void stateExit_Idle();
void stateEntry_Recording();
void stateExit_Recording();
void stateEntry_Error();
void stateExit_Error();
static void CheckInitQuintic();
static void PreSleepProcess();
static void PostSleepProcess();
status_t reloadConfigSettings();

uint32_t stateEntryTime = 0;
xTimerHandle TimeOutTimer, sdTimeOutTimer;
bool sdInsertWaitTimeoutFlag = FALSE;

void vTimeOutTimerCallback( xTimerHandle xTimer )
{
	task_stateMachine_EnqueueEvent(SYS_EVENT_POWER_SWITCH, 0x00);
}

void vSdTimeOutTimerCallback( xTimerHandle xTimer )
{
	sdInsertWaitTimeoutFlag = TRUE;
}

//task to handle the events
void task_stateMachineHandler(void *pvParameters)
{
	//Start Idle-time out timer 
	TimeOutTimer = xTimerCreate("Time Out Timer", (MAX_IDLE_TIMEOUT/portTICK_RATE_MS), pdFALSE, NULL, vTimeOutTimerCallback);
	if (TimeOutTimer == NULL)
	{
		printf("Failed to create timer task code %d\r\n", TimeOutTimer);
	}
	
	queue_stateMachineEvents = xQueueCreate( 10, sizeof(eventMessage_t));
	if(queue_stateMachineEvents == 0)
	{
		// Queue was not created this is an error!
		printf("an error has occurred, state machine queue creation failed. \r\n");
		return;
	}	
	eventMessage_t eventMessage = {.sysEvent = SYS_EVENT_POWER_SWITCH, .data = 0x0000}; 
	//from start immediately put the system into power down mode. 
	xQueueSendToBack( queue_stateMachineEvents,( void * ) &eventMessage,5); 
	while(1)
	{
		if(xQueueReceive(queue_stateMachineEvents, &(eventMessage), 100) == TRUE)
		{
			processEvent(eventMessage); 
		}
		vTaskDelay(50); 
		
	}		
}

status_t task_stateMachine_EnqueueEvent(systemEvents_t eventType, uint16_t data)
{
	status_t status = STATUS_PASS; 
	eventMessage_t msg; 
	msg.sysEvent = eventType; 
	msg.data = data; 
	if(queue_stateMachineEvents != NULL)
	{
		if(xQueueSendToBack( queue_stateMachineEvents,( void * ) &msg,5) != TRUE)
		{
			status = STATUS_FAIL; 
		}
	}
	return status; 
}
systemStates_t getCurrentState()
{
	return currentSystemState;
}
//static functions
//event processor
void processEvent(eventMessage_t eventMsg)
{
	switch(eventMsg.sysEvent)
	{
		case SYS_EVENT_POWER_SWITCH:
		{
			if(currentSystemState == SYS_STATE_RECORDING)
			{
				//stop recording, then go to the off state. 
				stateExit_Recording(); 
			}
			else if (currentSystemState == SYS_STATE_RESET)
			{				
				stateExit_Reset();
			}
			else if (currentSystemState == SYS_STATE_IDLE)
			{
				stateExit_Idle();
			}
			else if (currentSystemState == SYS_STATE_ERROR)
			{
				stateExit_Error();
			}
			else if(currentSystemState == SYS_STATE_POWER_DOWN)
			{
				//we should already be off, ignore
				break; 
			}
			//go to the power down state. 
			stateEntry_PowerDown(); 	
		}
		break;
		case SYS_EVENT_RECORD_SWITCH:
		{
			if(currentSystemState == SYS_STATE_IDLE)
			{
				//start recording
				stateExit_Idle();
				stateEntry_Recording();
				//go to recording state
			}
			else if(currentSystemState == SYS_STATE_RECORDING)
			{
				//stop recording
				stateExit_Recording();
				//go to IDLE state
				stateEntry_Idle(); 
			}
		}
		break;
		case SYS_EVENT_RESET_SWITCH:
		{
			if(currentSystemState == SYS_STATE_RECORDING)
			{
				//stop recording
				stateExit_Recording();
			}
			else if(currentSystemState == SYS_STATE_RESET)
			{
				//do nothing, the user is impatient. 
				break; 
			}
			else if (currentSystemState == SYS_STATE_IDLE)
			{
				stateExit_Idle();
			}
			else if (currentSystemState == SYS_STATE_ERROR)
			{
				stateExit_Error();
			}
						 
			//start the reset process... 
			if (brainSettings.isLoaded == 1)
			{
				stateEntry_Reset();
			}
			else
			{
				stateEntry_Error();
			}
			 
		}
		break;
		case SYS_EVENT_IMU_DISCONNECT:
		{
			if(currentSystemState == SYS_STATE_POWER_DOWN)
			{
				//do nothing, this is expected
				break;
			}
		}		
		case SYS_EVENT_OVER_CURRENT:
		case SYS_EVENT_BLE_ERROR:
		case SYS_EVENT_JACK_DETECT:
		case SYS_EVENT_RESET_FAILED:
		{
			if(currentSystemState == SYS_STATE_RECORDING)
			{
				//stop recording.
				stateExit_Recording();
			}
			stateEntry_Error(); 			
		}
		break;
		case SYS_EVENT_LOW_BATTERY:
		{
			if(currentSystemState == SYS_STATE_RECORDING)
			{
				//stop recording
				stateExit_Recording();
			}
			else if (currentSystemState == SYS_STATE_IDLE)
			{
				stateExit_Idle();
			}
			else if (currentSystemState == SYS_STATE_ERROR)
			{
				stateExit_Error();
			}
			//go to the power down state. 	
			stateEntry_PowerDown(); 		
		}
		break;
		case SYS_EVENT_RESET_COMPLETE:
		{
			if(currentSystemState != SYS_STATE_RESET)
			{
				//do nothing, this is weird, should not get here. 
				break;
			}
			QResetCount++;	//Check if all three Quintics are past the initialization process
			int z;
			for (z=0; z<3; z++)
			{
				if (eventMsg.data == z)	//check for which Quintic is successfully initialized
				{
					ResetStatus |= (1u<<(z));	//Save the result to a result flag
					
				}
			}
			//go to the idle state
			if (QResetCount < 3)	//If not all Quintic were initialized,
			{
				CheckInitQuintic();	// pass the init command to the next one
			}
			else
			{
				if (ResetStatus == 0x05)	//Check if all of them were initialized
				{
					//go to the idle state
					stateEntry_Idle();
				}
				else
				{
					task_stateMachine_EnqueueEvent(SYS_EVENT_RESET_FAILED, 0);	//Assert Reset failed as one or more failed to initialize
				}
			}
			
		}
		break;
		case SYS_EVENT_POWER_UP_COMPLETE:
		{
			if(currentSystemState != SYS_STATE_POWER_DOWN)
			{
				//we should not have gotten this event here
				break;
			}
			//first thing to do after the power up is reload config settings 
			if(reloadConfigSettings() == STATUS_PASS)
			{	
				//perform reset only if loading the settings was successful
				stateEntry_Reset(); 			
			}
			else
			{
				stateEntry_Error();
			}
		}
		break; 		
		default:
		//do nothing, should never reach here
		break; 										
	}
}

/***********************************************************************************************
 * setLED(led_states_t ledState)
 * @brief Set the led to ON or oFF as per the requirement
 * @param led_states_t ledState
 * @return void
 ***********************************************************************************************/
//need to add timer to handle flashing. 
void setLED(led_states_t ledState)
{
	//put LEDs in an initial state
	drv_gpio_setPinState(DRV_GPIO_PIN_RED_LED, LED_LEVEL_OFF); 
	drv_gpio_setPinState(DRV_GPIO_PIN_BLUE_LED, LED_LEVEL_OFF); 
	drv_gpio_setPinState(DRV_GPIO_PIN_GREEN_LED, LED_LEVEL_OFF); 
	switch(ledState)
	{
		case LED_STATE_OFF:
		//do nothing. 
		break; 
		case LED_STATE_RED_SOLID:
			drv_gpio_setPinState(DRV_GPIO_PIN_RED_LED, LED_LEVEL_ON); 
		break; 
		case LED_STATE_RED_FLASH:
		break;
		case LED_STATE_BLUE_SOLID:
			drv_gpio_setPinState(DRV_GPIO_PIN_BLUE_LED, LED_LEVEL_ON);
		break;
		case LED_STATE_BLUE_FLASH:
		break;
		case LED_STATE_GREEN_SOLID:
			drv_gpio_setPinState(DRV_GPIO_PIN_GREEN_LED, LED_LEVEL_ON);
		break;
		case LED_STATE_GREEN_FLASH:
		break;
		case LED_STATE_YELLOW_SOLID:
			drv_gpio_setPinState(DRV_GPIO_PIN_GREEN_LED, LED_LEVEL_ON);
			drv_gpio_setPinState(DRV_GPIO_PIN_RED_LED, LED_LEVEL_ON); 
		break;
		case LED_STATE_YELLOW_FLASH:
		break;				
	}
}

/***********************************************************************************************
 * stateEntry_PowerDown()
 * @brief Put the processor to sleep and wait for interrupt on Power pin
 * @param void
 * @return void
 ***********************************************************************************************/
//entry and exit functions
//power down function (handles entry and exit)
void stateEntry_PowerDown()
{
	currentSystemState = SYS_STATE_POWER_DOWN;	
	//setLED(LED_STATE_OFF);
	drv_led_set(DRV_LED_OFF, DRV_LED_SOLID);
	//disable the interrupts, except for the power button
	//it is assumed that the button has already been held for 5 seconds

	DisconnectImus(&quinticConfig[0]);
	//DisconnectImus(&quinticConfig[1]);
	DisconnectImus(&quinticConfig[2]);
	
	//clear the settings loaded bit
	brainSettings.isLoaded = 0;
	
	//turn off the JACK power supplies (they're negatively asserted) 
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_STATE_HIGH);
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_STATE_HIGH);
	/* Put the processor to sleep, in this context with the systick timer
	*  dead, we will never leave, so initialization has to be done here too. 
	*  for now just stay in a loop until the power button is pressed again. 
	*/
	
	printf("Sleep mode enabled\r\n");
	PreSleepProcess();
	while (pwSwState == FALSE)	//Stay in sleep mode until wakeup
	{
		//cpu_irq_disable();
		
		pmc_enable_sleepmode(0);
		
		//Processor wakes up from sleep
		delay_ms(WAKEUP_DELAY);
		drv_gpio_getPinState(DRV_GPIO_PIN_PW_SW, &pwSwState);	//poll the power switch
		if(pwSwState == DRV_GPIO_PIN_STATE_LOW)	//check if it is a false wakeup
		{
			pwSwState = TRUE;
		}
		else
		{
			pwSwState = FALSE;
		}
	}
	
	pwSwState = FALSE;
	PostSleepProcess();
	//enable the jacks
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_STATE_LOW);
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_STATE_LOW);
	
	//TODO check which jacks are connected to determine which IMUs are there	
	
	//send power up complete event
	eventMessage_t msg = {.sysEvent = SYS_EVENT_POWER_UP_COMPLETE, .data = 0};
	if(queue_stateMachineEvents != NULL)
	{
		xQueueSendToBack(queue_stateMachineEvents, &msg,5);
	}
		
}

/***********************************************************************************************
 * stateEntry_Reset()
 * @brief This initializes the Entry to Reset state
 * @param void
 * @return void
 ***********************************************************************************************/
//reset entry
void stateEntry_Reset()
{
	status_t status = STATUS_PASS; 
	eventMessage_t msg = {.sysEvent = SYS_EVENT_RESET_COMPLETE, .data = 0};
	//set current state to reset.
	currentSystemState = SYS_STATE_RESET;
	//set LED to blue
	//setLED(LED_STATE_BLUE_SOLID); 
	drv_led_set(DRV_LED_BLUE, DRV_LED_FLASH);
	QResetCount = 0;
	//reset NOD power with JACK EN
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_STATE_HIGH); 
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_STATE_HIGH);
	vTaskDelay(100); 
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_STATE_LOW); 
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_STATE_LOW); 
	vTaskDelay(100); 
	//Reset/init Q1
	
	if(quinticConfig[0].isinit)
		{
			//status |= task_quintic_initializeImus(&quinticConfig[i]);	
			int retCode = xTaskCreate(task_quintic_initializeImus, "Qi", TASK_IMU_INIT_STACK_SIZE, (void*)&quinticConfig[0], TASK_IMU_INIT_PRIORITY, &ResetHandle );
			if (retCode != pdPASS)
			{
				printf("Failed to create Q1 task code %d\r\n", retCode);
			}
		}
	
	//initialize fabric sense module
	status |= task_fabSense_init(&fsConfig); 
	
	//if(status != STATUS_PASS)
	//{
		//msg.sysEvent = SYS_EVENT_RESET_FAILED;  		
	//}
	//if(queue_stateMachineEvents != NULL)
	//{
		//xQueueSendToBack(queue_stateMachineEvents, &msg,5); 	
	//}
	
}

/***********************************************************************************************
 * stateExit_Reset()
 * @brief This initializes the Exit from Reset state
 * @param void
 * @return void
 ***********************************************************************************************/
void stateExit_Reset()
{
	drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST1, DRV_GPIO_PIN_STATE_LOW); 
	//drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST2, DRV_GPIO_PIN_STATE_LOW);
	drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST3, DRV_GPIO_PIN_STATE_LOW);
	if (ResetHandle != NULL)
	{
		vTaskDelete(ResetHandle);
	}
}

/***********************************************************************************************
 * stateEntry_Recording()
 * @brief This initializes the entry to recording state and sends the start command
 * @param void
 * @return void
 ***********************************************************************************************/
//recording entry
void stateEntry_Recording()
{
	currentSystemState = SYS_STATE_RECORDING;
	stateEntryTime = sgSysTickCount;  
	//setLED(LED_STATE_RED_SOLID);
	drv_led_set(DRV_LED_RED, DRV_LED_FLASH);
	//open new log file
	if(task_sdCard_OpenNewFile() != STATUS_PASS)
	{
		//this is an error, we should probably do something
	}
	
	//wait for user to get into position
	vTaskDelay(3000);
	
	//setLED(LED_STATE_RED_SOLID); 	
	drv_led_set(DRV_LED_RED, DRV_LED_SOLID);
	//send start command to quintics and fabric sense
	task_quintic_startRecording(&quinticConfig[0]);
	task_quintic_startRecording(&quinticConfig[1]);
	task_quintic_startRecording(&quinticConfig[2]);
	task_fabSense_start(&fsConfig);				
}

/***********************************************************************************************
 * stateExit_Recording()
 * @brief This initializes the exit from recording state and sends the stop command
 * @param void
 * @return void
 ***********************************************************************************************/
//recording exit
void stateExit_Recording()
{
	//send stop command to quintics and fabric sense
	task_quintic_stopRecording(&quinticConfig[0]);
	task_quintic_stopRecording(&quinticConfig[1]);
	task_quintic_stopRecording(&quinticConfig[2]);
	task_fabSense_stop(&fsConfig);	
	//wait for a bit for the data to be processed.
	vTaskDelay(100);
	//close the data file for the current recording
	task_sdCard_CloseFile();				
}

/***********************************************************************************************
 * stateEntry_Idle()
 * @brief This initializes the entry to idle state
 * @param void
 * @return void
 ***********************************************************************************************/
//idle entry
void stateEntry_Idle()
{
	currentSystemState = SYS_STATE_IDLE;
	xTimerReset(TimeOutTimer, 0); 
	//setLED(LED_STATE_GREEN_SOLID);
	drv_led_set(DRV_LED_GREEN, DRV_LED_SOLID);
}

/***********************************************************************************************
 * stateExit_Idle()
 * @brief This initializes the exit from idle state
 * @param void
 * @return void
 ***********************************************************************************************/
//idle exit
void stateExit_Idle()
{
	xTimerStop(TimeOutTimer, 0); 
}

/***********************************************************************************************
 * stateEntry_Error()
 * @brief This initializes the entry to error state
 * @param void
 * @return void
 ***********************************************************************************************/
//Error state entry
void stateEntry_Error()
{
	currentSystemState = SYS_STATE_ERROR;
	xTimerReset(TimeOutTimer, 0);
	//setLED(LED_STATE_YELLOW_SOLID); 
	drv_led_set(DRV_LED_YELLOW, DRV_LED_FLASH);
}

/***********************************************************************************************
 * stateExit_Error()
 * @brief This initializes the exit from error state
 * @param void
 * @return void
 ***********************************************************************************************/
//Error state exit
void stateExit_Error()
{
	xTimerStop(TimeOutTimer, 0);
}
/***********************************************************************************************
 * CheckInitQuintic()
 * @brief This creates quintic initializing task for the next  quintic
 * @param void
 * @return void
 ***********************************************************************************************/
static void CheckInitQuintic()
{
	if (QResetCount == 1)	//temporary check to ignore BLE2 due to hardware problems
	{
		QResetCount = 2;
	}
	//pass the init command to the next Quintic
	int retCode = xTaskCreate(task_quintic_initializeImus, "Qi", TASK_IMU_INIT_STACK_SIZE, (void*)&quinticConfig[QResetCount], TASK_IMU_INIT_PRIORITY, &ResetHandle );
	if (retCode != pdPASS)
	{
		printf("Failed to create Q1 task code %d\r\n", retCode);
	}
}

/***********************************************************************************************
 * PreSleepProcess()
 * @brief This does the necessary processing before putting the processor to sleep
 * @param void
 * @return void
 ***********************************************************************************************/
static void PreSleepProcess()
{
	//supc_disable_brownout_detector(SUPC);	
	//unmount the SD card. 
	static FRESULT res;
	res = f_mount(LUN_ID_SD_MMC_0_MEM, NULL);
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;	//disable the systick timer
	drv_uart_deInit(quinticConfig[0].uartDevice);
	drv_uart_deInit(quinticConfig[1].uartDevice);
	drv_uart_deInit(quinticConfig[2].uartDevice);
	drv_uart_deInit(&uart0Config);
	drv_gpio_disable_interrupt_all();
	drv_gpio_enable_interrupt(DRV_GPIO_PIN_PW_SW);
	
	
}

/***********************************************************************************************
 * PostSleepProcess()
 * @brief This does the necessary processing required after waking up the processor from sleep
 * @param void
 * @return void
 ***********************************************************************************************/
static void PostSleepProcess()
{
	drv_gpio_clear_Int(DRV_GPIO_PIN_PW_SW);	//Clear the interrupt generated by power switch flag
	drv_gpio_initializeAll();
	drv_uart_init(quinticConfig[0].uartDevice);
	drv_uart_init(quinticConfig[1].uartDevice);
	drv_uart_init(quinticConfig[2].uartDevice);
	drv_uart_init(&uart0Config);
	sd_mmc_init();
	printf("Exit Sleep mode\r\n");
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;	//enable the systick timer
}

/***********************************************************************************************
 * reloadConfigSettings()
 * @brief Reload the config settings from SD card 
 * @param void
 * @return void
 ***********************************************************************************************/
status_t reloadConfigSettings()
{
	static FATFS fs;
	static FRESULT res;
	status_t result = STATUS_FAIL;
	Ctrl_status status; 
	//make non-blocking remount of sd-card
	drv_gpio_pin_state_t sdCdPinState;
	//sd_mmc_init();
	drv_gpio_getPinState(DRV_GPIO_PIN_SD_CD, &sdCdPinState);
	if (sdCdPinState != SD_MMC_0_CD_DETECT_VALUE)
	{
		sdInsertWaitTimeoutFlag = FALSE;	//clear the flag for resuse
		return result;
	}
	sdTimeOutTimer = xTimerCreate("SD insert Time Out Timer", (SD_INSERT_WAIT_TIMEOUT/portTICK_RATE_MS), pdFALSE, NULL, vSdTimeOutTimerCallback);
	if (sdTimeOutTimer == NULL)
	{
		printf("Failed to create timer task code %d\r\n", sdTimeOutTimer);
	}
	xTimerStart(sdTimeOutTimer, 0);
	do
	{
		status = sd_mmc_test_unit_ready(0);
		if (CTRL_FAIL == status)
		{
			printf("Card install FAIL\n\r");
			printf("Please unplug and re-plug the card.\n\r");
			while ((CTRL_NO_PRESENT != sd_mmc_check(0)) | (sdInsertWaitTimeoutFlag == TRUE))
			{
			}
		}
	} while ((CTRL_GOOD != status) | (sdInsertWaitTimeoutFlag == TRUE));
	
	sdInsertWaitTimeoutFlag = FALSE;	//clear the flag for resuse
	xTimerStop(sdTimeOutTimer, 0);
	xTimerDelete(sdTimeOutTimer, 0);
	
	/*	Mount the SD card	*/
	if(status == CTRL_GOOD)
	{
		memset(&fs, 0, sizeof(FATFS));
		res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
		if (res == FR_INVALID_DRIVE)
		{
			printf("Error: Invalid Drive\r\n");
			return result;
		}
		//prevent system to go in reset state on button press event after a failed config load
		result = STATUS_PASS;
		if(loadSettings(SETTINGS_FILENAME) != STATUS_PASS)
		{
			result = STATUS_FAIL;
			printf("failed to get read settings\r\n");
			return result;
		}
		brainSettings.isLoaded = 1;
	}
	return result;
}