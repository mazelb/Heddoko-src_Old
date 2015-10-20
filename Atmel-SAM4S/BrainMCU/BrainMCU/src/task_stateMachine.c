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

#define LED_LEVEL_OFF	DRV_GPIO_PIN_STATE_HIGH
#define LED_LEVEL_ON	DRV_GPIO_PIN_STATE_LOW
//queue to store events
xQueueHandle queue_stateMachineEvents = NULL;
systemStates_t currentSystemState = SYS_STATE_OFF;
//external variables
extern quinticConfiguration_t quinticConfig[];
extern fabricSenseConfig_t fsConfig;
extern unsigned long sgSysTickCount;

//static function forward declarations
void processEvent(eventMessage_t eventMsg);
void stateEntry_PowerDown();
void stateEntry_Reset();
void stateEntry_Idle();
void stateEntry_Recording();
void stateExit_Recording();
void stateEntry_Error();

uint32_t stateEntryTime = 0;


//task to handle the events
void task_stateMachineHandler(void *pvParameters)
{
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
			//start the reset process... 
			stateEntry_Reset(); 
		}
		break;
		case SYS_EVENT_OVER_CURRENT:
		case SYS_EVENT_IMU_DISCONNECT:
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
			//go to the idle state
			stateEntry_Idle(); 			
		}
		break;
		case SYS_EVENT_POWER_UP_COMPLETE:
		{
			if(currentSystemState != SYS_STATE_POWER_DOWN)
			{
				//we should not have gotten this event here
				break;
			}
			//first thing to do after the power up is reset everything. 
			stateEntry_Reset(); 			
		}
		break; 		
		default:
		//do nothing, should never reach here
		break; 										
	}
}

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


//entry and exit functions
//power down function (handles entry and exit)
void stateEntry_PowerDown()
{
	currentSystemState = SYS_STATE_POWER_DOWN;	
	setLED(LED_STATE_OFF);
	//disable the interrupts, except for the power button
	//it is assumed that the button has already been held for 5 seconds
	
	//turn off the JACK power supplies (they're negatively asserted) 
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_STATE_HIGH);
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_STATE_HIGH);
	
	/* Put the processor to sleep, in this context with the systick timer
	*  dead, we will never leave, so initialization has to be done here too. 
	*  for now just stay in a loop until the power button is pressed again. 
	*/
	drv_gpio_pin_state_t powerSwitchState = DRV_GPIO_PIN_STATE_HIGH; 
	while(1)
	{
		drv_gpio_getPinState(DRV_GPIO_PIN_PW_SW, &powerSwitchState); 
		//clear the transition flag
		drv_gpio_check_Int(DRV_GPIO_PIN_PW_SW);
		if(powerSwitchState == DRV_GPIO_PIN_STATE_LOW)
		{
			break;
		}
	}
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
//reset entry
void stateEntry_Reset()
{
	status_t status = STATUS_PASS; 
	eventMessage_t msg = {.sysEvent = SYS_EVENT_RESET_COMPLETE, .data = 0};
	//set current state to reset.
	currentSystemState = SYS_STATE_RESET;
	//set LED to blue
	setLED(LED_STATE_BLUE_SOLID); 
	//reset NOD power with JACK EN
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_STATE_HIGH); 
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_STATE_HIGH);
	vTaskDelay(100); 
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_STATE_LOW); 
	drv_gpio_setPinState(DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_STATE_LOW); 
	vTaskDelay(100); 
	//Reset/init Q1
	int i = 0;
	for(i=0;i<3;i++)
	{	
		if(quinticConfig[i].isinit)
		{
			status |= task_quintic_initializeImus(&quinticConfig[i]);		
		}
	}
	//initialize fabric sense module
	status |= task_fabSense_init(&fsConfig); 
	
	if(status != STATUS_PASS)
	{
		msg.sysEvent = SYS_EVENT_RESET_FAILED;  		
	}
	if(queue_stateMachineEvents != NULL)
	{
		xQueueSendToBack(queue_stateMachineEvents, &msg,5); 	
	}
	
}
//recording entry
void stateEntry_Recording()
{
	currentSystemState = SYS_STATE_RECORDING;
	stateEntryTime = sgSysTickCount;  
	setLED(LED_STATE_RED_SOLID); 
	//open new log file
	if(task_sdCard_OpenNewFile() != STATUS_PASS)
	{
		//this is an error, we should probably do something
	}
	//blink the LED Red for 3 seconds
	//TODO do this is a timer task instead...
	int i = 0;
	for(i =0; i < 15 ; i++)
	{
		drv_gpio_togglePin(DRV_GPIO_PIN_RED_LED); 
		vTaskDelay(200); 
	}
	setLED(LED_STATE_RED_SOLID); 	
	//send start command to quintics and fabric sense
	task_quintic_startRecording(&quinticConfig[0]);
	task_quintic_startRecording(&quinticConfig[1]);
	task_quintic_startRecording(&quinticConfig[2]);
	task_fabSense_start(&fsConfig);				
}
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
//idle entry
void stateEntry_Idle()
{
	currentSystemState = SYS_STATE_IDLE; 
	setLED(LED_STATE_GREEN_SOLID); 
}
//Error state entry
void stateEntry_Error()
{
	currentSystemState = SYS_STATE_ERROR;
	setLED(LED_STATE_YELLOW_SOLID); 
}