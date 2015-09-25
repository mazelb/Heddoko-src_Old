/*
 * drv_gpio.c
 *
 * Created: 9/25/2015 9:55:00 AM
 *  Author: Sean Cloghesy
 */ 
#include "drv_gpio.h"

drv_gpio_config_t gpioConfig[] = 
{
	/*  PIN ID              PIN MODE                INITIAL PIN STATE       INTERRUPT MODE       INTERRUPT HANDLER  PULL UP EN  DEBOUNCE EN */
 	{DRV_GPIO_PIN_SW0, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},		
	{DRV_GPIO_PIN_PW_SW, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_AC_SW1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_AC_SW2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_JC_OC1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_JC_OC2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_JC_DC1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_JC_DC2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_BLE_RST1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},		
	{DRV_GPIO_PIN_BLE_RST2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},		
	{DRV_GPIO_PIN_BLE_RST3, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},		
	{DRV_GPIO_PIN_GREEN_LED, DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},	  
	{DRV_GPIO_PIN_BLUE_LED, DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},	  
	{DRV_GPIO_PIN_RED_LED, DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},	  	
	{DRV_GPIO_PIN_LBO, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_STAT, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_PIN_SD_CD, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE}
	 
};

status_t drv_gpio_initializeAll(void)
{
	status_t status = STATUS_PASS; 
	int numGpio = sizeof(gpioConfig)/sizeof(drv_gpio_config_t); 
	int i = 0; 
	
	for(i = 0; i < numGpio ; i++)
	{
		if(drv_gpio_config(&gpioConfig[i]) != STATUS_PASS)
		{
			printf("failed to initialize GPIO at index %d", i); 
			status |= STATUS_FAIL; 			
		}
	}	
	return status; 
}
status_t drv_gpio_config(drv_gpio_config_t* gpioConfig)
{
	status_t status = STATUS_PASS;
	if(gpioConfig->pinMode == DRV_GPIO_PIN_MODE_INPUT)
	{
		
	}
	else
	{
		
	}
	
	
	return status;
}
status_t drv_gpio_setPinState(uint32_t pinId, drv_gpio_pin_state_t state)
{
	status_t status = STATUS_PASS;
	
	return status;
}
status_t drv_gpio_getPinState(uint32_t pinId, drv_gpio_pin_state_t* state)
{
	status_t status = STATUS_PASS;
	
	return status;
}
status_t drv_gpio_togglePin(uint32_t pinId)
{
	status_t status = STATUS_PASS;
	
	return status;
}