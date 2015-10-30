/*
 * drv_led.c
 *
 * Created: 10/27/2015 10:45:28 AM
 *  Author: Hriday Mehta
 * Copyright Heddoko(TM) 2015, all rights reserved
 */ 

#include "drv_led.h"

//local structure to store led configuration
drv_led_config_t ledConfig;

xTimerHandle LedTimer;	//timer handle for the led timer task
drv_led_color_type_t vledColor;	//global variable to store led color, used in timer callback
drv_led_state_type_t vledState; //global variable to store led state, used in timer callback

//Led Timer callback function
void vLedTimerCallback( xTimerHandle xTimer )
{
	if(vledState == DRV_LED_FLASH)	//perform the action only if state is flashing
	{
		//check which led color is set and toggle the pin state
		switch(vledColor)
		{
			case DRV_LED_RED:
				drv_gpio_togglePin(DRV_GPIO_PIN_RED_LED);
				break;
			case DRV_LED_BLUE:
				drv_gpio_togglePin(DRV_GPIO_PIN_BLUE_LED);
				break;
			case DRV_LED_GREEN:
				drv_gpio_togglePin(DRV_GPIO_PIN_GREEN_LED);
				break;
			case DRV_LED_YELLOW:
				drv_gpio_togglePin(DRV_GPIO_PIN_RED_LED);
				drv_gpio_togglePin(DRV_GPIO_PIN_GREEN_LED);
				break;
			case DRV_LED_WHITE:
				drv_gpio_togglePin(DRV_GPIO_PIN_RED_LED);
				drv_gpio_togglePin(DRV_GPIO_PIN_GREEN_LED);
				drv_gpio_togglePin(DRV_GPIO_PIN_BLUE_LED);
				break;
			default:
				break;
			
		}
	}
	else
	{
		//usually shouldn't end up here
		//if it does the timer was not stopped while initializing led to SOLID state
		xTimerStop(LedTimer, 0);
	}
}

//store the led configuration
status_t drv_led_init(drv_led_config_t* configLed)
{
	status_t result = STATUS_PASS;
	//store the led configuration
	ledConfig.redLed = configLed->redLed;
	ledConfig.blueLed = configLed->blueLed;
	ledConfig.greenLed = configLed->greenLed;
	
	//create LedTimer task
	LedTimer = xTimerCreate("Led Timer", (LED_BLINK_RATE/portTICK_RATE_MS), pdTRUE, NULL, vLedTimerCallback);
	if (LedTimer == NULL)
	{
		printf("Failed to create Led timer task code %d\r\n", LedTimer);
		result = STATUS_FAIL;
	}
	return result;
}

status_t drv_led_set(drv_led_color_type_t ledColor, drv_led_state_type_t ledState)
{
	status_t result = STATUS_PASS;
	if (xTimerIsTimerActive(LedTimer) != pdFALSE)	//check if timer is active or dormant
	{
		//timer is active, stop it and reset it
		xTimerStop(LedTimer, 0);
		xTimerReset(LedTimer, 0);
	}
	//switch off all LEDs
	drv_gpio_setPinState(DRV_GPIO_PIN_RED_LED, DRV_GPIO_PIN_STATE_HIGH);
	drv_gpio_setPinState(DRV_GPIO_PIN_BLUE_LED, DRV_GPIO_PIN_STATE_HIGH);
	drv_gpio_setPinState(DRV_GPIO_PIN_GREEN_LED, DRV_GPIO_PIN_STATE_HIGH);
	
	vledColor = ledColor;	//copy the color to a global variable used in timer callback
	vledState = ledState;	//copy the state to a global variable used in timer callback
	switch (ledColor)
	{
		case DRV_LED_RED:
			drv_gpio_setPinState(DRV_GPIO_PIN_RED_LED, DRV_GPIO_PIN_STATE_LOW);
			break;
		case DRV_LED_BLUE:
			drv_gpio_setPinState(DRV_GPIO_PIN_BLUE_LED, DRV_GPIO_PIN_STATE_LOW);
			break;
		case DRV_LED_GREEN:
			drv_gpio_setPinState(DRV_GPIO_PIN_GREEN_LED, DRV_GPIO_PIN_STATE_LOW);
			break;
		case DRV_LED_YELLOW:
			drv_gpio_setPinState(DRV_GPIO_PIN_RED_LED, DRV_GPIO_PIN_STATE_LOW);
			drv_gpio_setPinState(DRV_GPIO_PIN_GREEN_LED, DRV_GPIO_PIN_STATE_LOW);
			break;
		case DRV_LED_WHITE:
			drv_gpio_setPinState(DRV_GPIO_PIN_RED_LED, DRV_GPIO_PIN_STATE_LOW);
			drv_gpio_setPinState(DRV_GPIO_PIN_GREEN_LED, DRV_GPIO_PIN_STATE_LOW);
			drv_gpio_setPinState(DRV_GPIO_PIN_BLUE_LED, DRV_GPIO_PIN_STATE_LOW);
			break;
		case DRV_LED_OFF:
			drv_gpio_setPinState(DRV_GPIO_PIN_RED_LED, DRV_GPIO_PIN_STATE_HIGH);
			drv_gpio_setPinState(DRV_GPIO_PIN_GREEN_LED, DRV_GPIO_PIN_STATE_HIGH);
			drv_gpio_setPinState(DRV_GPIO_PIN_BLUE_LED, DRV_GPIO_PIN_STATE_HIGH);
			break;
		default:
			break;
	}
	
	if ((ledState == DRV_LED_FLASH) & (ledColor != DRV_LED_OFF))
	{
		xTimerStart(LedTimer, 0);
	}
	else
	{
		//stop the timer
		if (xTimerIsTimerActive(LedTimer) != pdFALSE)	//check if timer is active or dormant
		{
			xTimerStop(LedTimer, 0);
		}
	}
	return result;
}

