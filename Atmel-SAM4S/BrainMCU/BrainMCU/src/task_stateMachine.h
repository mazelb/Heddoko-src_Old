/*
 * task_stateMachine.h
 *
 * Created: 10/19/2015 9:09:32 AM
 *  Author: Sean Cloghesy
 */ 


#ifndef TASK_STATEMACHINE_H_
#define TASK_STATEMACHINE_H_
#include "common.h"
//enumeration of states
typedef enum 
{
	SYS_STATE_OFF,  //initial state of the system
	SYS_STATE_POWER_DOWN,
	SYS_STATE_RESET, 
	SYS_STATE_IDLE, 
	SYS_STATE_RECORDING, 
	SYS_STATE_ERROR 
}systemStates_t;
//enumeration of events
typedef enum
{
	SYS_EVENT_POWER_SWITCH,
	SYS_EVENT_RESET_SWITCH,
	SYS_EVENT_RECORD_SWITCH,
	SYS_EVENT_OVER_CURRENT,
	SYS_EVENT_SD_CARD_DETECT,
	SYS_EVENT_IMU_DISCONNECT,
	SYS_EVENT_BLE_ERROR,
	SYS_EVENT_JACK_DETECT,
	SYS_EVENT_LOW_BATTERY,
	SYS_EVENT_RESET_COMPLETE,
	SYS_EVENT_RESET_FAILED,
	SYS_EVENT_POWER_UP_COMPLETE
}systemEvents_t;

typedef enum
{
	LED_STATE_OFF, 
	LED_STATE_RED_SOLID,
	LED_STATE_RED_FLASH, 
	LED_STATE_BLUE_SOLID,
	LED_STATE_BLUE_FLASH,
	LED_STATE_GREEN_SOLID,
	LED_STATE_GREEN_FLASH, 
	LED_STATE_YELLOW_SOLID, 
	LED_STATE_YELLOW_FLASH	
}led_states_t;

//structure of event message
typedef struct  
{
	systemEvents_t sysEvent;
	uint16_t data;   	
}eventMessage_t;

void task_stateMachineHandler(void *pvParameters);
status_t task_stateMachine_EnqueueEvent(systemEvents_t eventType, uint16_t data);
systemStates_t getCurrentState();


#endif /* TASK_STATEMACHINE_H_ */