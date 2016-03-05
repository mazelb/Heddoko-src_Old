/*
 * chrg_chargeMonitor.c
 *
 * Created: 2/29/2016 3:50:03 PM
 *  Author: sean
  * charger state
  CHARGE CYCLE STATE 			STAT1 		STAT2 		PG
  Shutdown (VDD = VBAT) 		Hi-Z 		Hi-Z		 Hi-Z   0x07
  Shutdown (VDD = IN) 			Hi-Z 		Hi-Z		 L		0x06
  Shutdown (CE = L) 			Hi-Z 		Hi-Z		 L      0x06 
  Preconditioning 				L			Hi-Z 		 L      0x02
  Constant Current				L 			Hi-Z 		 L		0x02
  Constant Voltage 				L 			Hi-Z 		 L		0x02
  Charge Complete - Standby		Hi-Z 		L 			 L		0x04
  Temperature Fault 			L 			L 			 L		0x00
  Timer Fault					L 			L			 L		0x00
  Low Battery Output			L 			Hi-Z 		 Hi-Z	0x03
  No Battery Present 			Hi-Z		Hi-Z		 L		0x01
  No Input Power Present 		Hi-Z 		Hi-Z 		 Hi-Z	0x00

0x00 Fault
0x01 No Battery
0x02 Charging
0x03 Low Battery
0x04 Charge Complete
0x05 Invalid code
0x06 Shutdown_VIN
0x07 Shutdown_VBAT
 */ 


#include "chrg_chargeMonitor.h"
#include "mgr_managerTask.h"
volatile chrg_chargerState_t chrg_currentChargerState = CHRG_CHARGER_STATE_INVALID_CODE; 

extern xQueueHandle mgr_eventQueue;
//static function forward declarations
chrg_chargerState_t getChargerState(chrg_chargeMonitorConfig_t* chargerConfig);  


/***********************************************************************************************
 * chrg_chargeMonitor(void *pvParameters)
 * @brief This task will monitor the current charger status and battery charge level.  
 * @param pvParameters, void pointer to structure containing data router configuration. 
 * @return void
 ***********************************************************************************************/
void chrg_task_chargeMonitor(void *pvParameters)
{
	chrg_chargeMonitorConfig_t* chargeMonitorConfig;  	
	chrg_chargerState_t newChargerState = CHRG_CHARGER_STATE_INVALID_CODE; 
	mgr_eventMessage_t eventMessage; 
	while(1)
	{
		newChargerState = getChargerState(chargeMonitorConfig); 	
		//check if the state is new
		if(newChargerState != chrg_currentChargerState)
		{
			if(newChargerState == CHRG_CHARGER_STATE_LOW_BATTERY)
			{
				eventMessage.sysEvent = SYS_EVENT_LOW_BATTERY; 
				if(mgr_eventQueue != NULL)
				{
					if(xQueueSendToBack(mgr_eventQueue,( void * ) &eventMessage,5) != TRUE)
					{
						//this is an error, we should log it.
					}				
				}
			}
			chrg_currentChargerState = newChargerState; 			
		}	
		vTaskDelay(500); 
	}	
}


//static functions

//get charge status
chrg_chargerState_t getChargerState(chrg_chargeMonitorConfig_t* chargerConfig)
{
	chrg_chargerState_t chargerState = 0x00; 
	drv_gpio_pin_state_t tempPinState = DRV_GPIO_PIN_STATE_LOW; 
	drv_gpio_getPinState(chargerConfig->pin_pg, &tempPinState); 
	chargerState |= (tempPinState | 0x01); 
	drv_gpio_getPinState(chargerConfig->pin_stat1, &tempPinState);
	chargerState |= ((tempPinState<<1) | 0x01);
	drv_gpio_getPinState(chargerConfig->pin_stat2, &tempPinState);
	chargerState |= ((tempPinState<<2) | 0x01);		
	return chargerState; 		
}