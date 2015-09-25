/*
 * drv_gpio.c
 *
 * Created: 9/25/2015 9:55:00 AM
 *  Author: Sean Cloghesy
 */ 
#include "drv_gpio.h"

drv_gpio_config_t gpioConfig[] = 
{
	/*  PIN ID					PIN MASK		PIN MODE                INITIAL PIN STATE       INTERRUPT MODE       INTERRUPT HANDLER  PULL UP EN  DEBOUNCE EN */
 	{DRV_GPIO_ID_PIN_SW0, DRV_GPIO_PIN_SW0, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},		
	{DRV_GPIO_ID_PIN_PW_SW, DRV_GPIO_PIN_PW_SW, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_AC_SW1, DRV_GPIO_PIN_AC_SW1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_AC_SW2, DRV_GPIO_PIN_AC_SW2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_JC_OC1, DRV_GPIO_PIN_JC_OC1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_JC_OC2, DRV_GPIO_PIN_JC_OC2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_JC_DC1, DRV_GPIO_PIN_JC_DC1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_JC_DC2, DRV_GPIO_PIN_JC_DC2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_JC_EN1, DRV_GPIO_PIN_JC_EN1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_JC_EN2, DRV_GPIO_PIN_JC_EN2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_BLE_RST1, DRV_GPIO_PIN_BLE_RST1, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},		
	{DRV_GPIO_ID_PIN_BLE_RST2, DRV_GPIO_PIN_BLE_RST2, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},		
	{DRV_GPIO_ID_PIN_BLE_RST3, DRV_GPIO_PIN_BLE_RST3, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},		
	{DRV_GPIO_ID_PIN_GREEN_LED, DRV_GPIO_PIN_GREEN_LED, DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},	  
	{DRV_GPIO_ID_PIN_BLUE_LED, DRV_GPIO_PIN_BLUE_LED, DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},	  
	{DRV_GPIO_ID_PIN_RED_LED, DRV_GPIO_PIN_RED_LED, DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},	  	
	{DRV_GPIO_ID_PIN_LBO, DRV_GPIO_PIN_LBO, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_STAT, DRV_GPIO_PIN_STAT, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE},			
	{DRV_GPIO_ID_PIN_SD_CD, DRV_GPIO_PIN_SD_CD, DRV_GPIO_PIN_MODE_INPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_NONE, NULL                , TRUE    , TRUE}
	 
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
	unsigned long PinFlag = 0;
	if(gpioConfig->pinMode == DRV_GPIO_PIN_MODE_INPUT)	//Check if input or output
	{
		PinFlag |= PIO_TYPE_PIO_INPUT;	//Set as input
		if (gpioConfig->pullUpEnabled == TRUE)
		{
			PinFlag |= PIO_PULLUP;	//enable the pull up
		}
		
		if (gpioConfig->debounceEnabled == TRUE)
		{
			PinFlag |= PIO_DEBOUNCE;	//enable debounce filter
		}
		
		pmc_enable_periph_clk(gpioConfig->pinId);	
		pio_configure_pin(gpioConfig->pinId, PinFlag);
		
		switch(gpioConfig->interruptType)	//set interrupt configuration flag
		{
			case DRV_GPIO_INTERRUPT_HIGH_EDGE :
				PinFlag |= PIO_IT_RISE_EDGE;
				break;
			case DRV_GPIO_INTERRUPT_LOW_EDGE :
				PinFlag |= PIO_IT_FALL_EDGE;
				break;
			case DRV_GPIO_INTERRUPT_HIGH_LVL :
				PinFlag |= PIO_IT_HIGH_LEVEL;
				break;
			case DRV_GPIO_INTERRUPT_LOW_LVL :
				PinFlag |= PIO_IT_LOW_LEVEL;
				break;
			case DRV_GPIO_INTERRUPT_NONE :
			default:
				break;
		}
		PinFlag |= PIO_IT_AIME;
		
		if (PinFlag & (PIO_IT_RISE_EDGE | PIO_IT_FALL_EDGE | PIO_IT_HIGH_LEVEL | PIO_IT_LOW_LEVEL))	//if set as interrupt, enable and configure it
		{
			Pio *p_pio = pio_get_pin_group(gpioConfig->pinId);
			pio_enable_interrupt(p_pio, gpioConfig->pinMask);
			pio_configure_interrupt(p_pio, gpioConfig->pinMask, PinFlag);
		}
		
	}
	else	//The pin is ouput
	{
		if (gpioConfig->initialPinState == DRV_GPIO_PIN_STATE_HIGH)
		{
			PinFlag |= PIO_TYPE_PIO_OUTPUT_1;	//set default pin state
		} 
		else
		{
			PinFlag |= PIO_TYPE_PIO_OUTPUT_0;	//set default pin state
		}
		
		pmc_enable_periph_clk(gpioConfig->pinId);
		pio_configure_pin(gpioConfig->pinId, PinFlag);
	}
	
	return status;
}

status_t drv_gpio_setPinState(uint32_t pinId, drv_gpio_pin_state_t state)
{
	status_t status = STATUS_PASS;
	if (state == DRV_GPIO_PIN_STATE_HIGH)
	{
		pio_set_pin_high(pinId);
	} 
	else
	{
		pio_set_pin_low(pinId);
	}
	return status;
}

status_t drv_gpio_getPinState(uint32_t pinId, drv_gpio_pin_state_t* state)
{
	status_t status = STATUS_PASS;
	state = pio_get_pin_value(pinId);
	return status;
}

status_t drv_gpio_togglePin(uint32_t pinId)
{
	status_t status = STATUS_PASS;
	pio_toggle_pin(pinId);
	return status;
}