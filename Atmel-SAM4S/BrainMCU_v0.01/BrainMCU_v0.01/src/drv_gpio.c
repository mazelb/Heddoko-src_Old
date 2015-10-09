/*
 * drv_gpio.c
 *
 * Created: 9/25/2015 9:55:00 AM
 *  Author: Sean Cloghesy
 */ 
#include "drv_gpio.h"

/*	Interrupt Handlers Declarations	*/
static void drv_gpio_int_sw0(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_pw(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_sw1(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_sw2(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_oc1(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_oc2(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_dc1(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_dc2(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_en1(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_en2(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_lbo(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_stat(uint32_t ul_id, uint32_t ul_mask);
static void drv_gpio_int_cd(uint32_t ul_id, uint32_t ul_mask);

drv_gpio_config_t gpioConfig[] = 
{
	/*  PIN ID					PIN MODE				  INITIAL PIN STATE       INTERRUPT MODE				INTERRUPT HANDLER  PULL UP EN  DEBOUNCE EN	setFlag */
 	{DRV_GPIO_ID_PIN_SW0,		DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_sw0,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_PW_SW,		DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_pw,		TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_AC_SW1,	DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_sw1,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_AC_SW2,	DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_sw2,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_JC_OC1,	DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_oc1,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_JC_OC2,	DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_oc2,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_JC_DC1,	DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_dc1,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_JC_DC2,	DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_dc2,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_JC_EN1,	DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_en1,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_JC_EN2,	DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_en2,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_BLE_RST1,	DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, NULL,				TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_BLE_RST2,	DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, NULL,				TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_BLE_RST3,	DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, NULL,				TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_GREEN_LED, DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, NULL,				TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_BLUE_LED,	DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, NULL,				TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_RED_LED,	DRV_GPIO_PIN_MODE_OUTPUT, DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, NULL,				TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_LBO,		DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_lbo,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_STAT,		DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_stat,	TRUE,	TRUE,			0},
	{DRV_GPIO_ID_PIN_SD_CD,		DRV_GPIO_PIN_MODE_INPUT,  DRV_GPIO_PIN_STATE_LOW, DRV_GPIO_INTERRUPT_LOW_EDGE, drv_gpio_int_cd,		TRUE,	TRUE,			0}
	 
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
		
		PinFlag = 0;	//Reset the PinFlag to use it to configure interrupt
		switch(gpioConfig->interruptType)	//set interrupt configuration flag
		{
			case DRV_GPIO_INTERRUPT_HIGH_EDGE :
				PinFlag |= PIO_IT_RISE_EDGE | PIO_IT_AIME;
				break;
			case DRV_GPIO_INTERRUPT_LOW_EDGE :
				PinFlag |= PIO_IT_FALL_EDGE | PIO_IT_AIME;
				break;
			case DRV_GPIO_INTERRUPT_HIGH_LVL :
				PinFlag |= PIO_IT_HIGH_LEVEL | PIO_IT_AIME;
				break;
			case DRV_GPIO_INTERRUPT_LOW_LVL :
				PinFlag |= PIO_IT_LOW_LEVEL | PIO_IT_AIME;
				break;
			case DRV_GPIO_INTERRUPT_NONE :
			default:
				break;
		}
		
		if (PinFlag & (PIO_IT_RISE_EDGE | PIO_IT_FALL_EDGE | PIO_IT_HIGH_LEVEL | PIO_IT_LOW_LEVEL))	//if set as interrupt, enable and configure it
		{
			Pio *p_pio = pio_get_pin_group(gpioConfig->pinId);	//peripheral ID
			uint32_t PinMask = pio_get_pin_group_mask(gpioConfig->pinId);	//PinMask
			uint32_t PinPio = pio_get_pin_group_id(gpioConfig->pinId);	//Pin ID
			pio_handler_set(p_pio, PinPio, PinMask, PinFlag, gpioConfig->interruptHandler);
			pio_enable_interrupt(p_pio, PinMask);
			NVIC_EnableIRQ(PinPio);
		}
		
	}
	else	//The pin is output
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

status_t drv_gpio_setPinState(drv_gpio_pins_t pin, drv_gpio_pin_state_t state)
{
	status_t status = STATUS_PASS;
	if(gpioConfig[pin].pinMode != DRV_GPIO_PIN_MODE_OUTPUT)
	{
		return STATUS_FAIL; 
	}
	if (state == DRV_GPIO_PIN_STATE_HIGH)
	{
		pio_set_pin_high(gpioConfig[pin].pinId);
	} 
	else
	{
		pio_set_pin_low(gpioConfig[pin].pinId);
	}
	return status;
}

status_t drv_gpio_getPinState(drv_gpio_pins_t pin, drv_gpio_pin_state_t* state)
{
	status_t status = STATUS_PASS;
	state = pio_get_pin_value(gpioConfig[pin].pinId);
	return status;
}

status_t drv_gpio_togglePin(drv_gpio_pins_t pin)
{
	status_t status = STATUS_PASS;
	pio_toggle_pin(gpioConfig[pin].pinId);
	return status;
}

bool drv_gpio_check_Int(drv_gpio_pins_t pin)
{
	bool returnVal;
	returnVal = gpioConfig[pin].gpioSetFlag;
	gpioConfig[pin].gpioSetFlag = 0;
	return	returnVal;
}

/*	Interrupt Handlers definitions for GPIOs	*/
static void drv_gpio_int_sw0(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_SW0].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = pio_get_interrupt_status(PIOA);
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_SW0].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_pw(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_PW_SW].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_PW_SW].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_sw1(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_AC_SW1].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_AC_SW1].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_sw2(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_AC_SW2].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_AC_SW2].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_oc1(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_JC_OC1].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_JC_OC1].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_oc2(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_JC_OC2].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_JC_OC2].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_dc1(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_JC_DC1].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_JC_DC1].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_dc2(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_JC_DC2].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_JC_DC2].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_en1(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_JC_EN1].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_JC_EN1].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_en2(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_JC_EN2].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_JC_EN2].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_lbo(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_LBO].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_LBO].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_stat(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_STAT].pinId);
	pio_disable_interrupt(PIOA, PinMask);
	uint32_t ReadIsr = PIOA->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_STAT].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOA, PinMask);
}

static void drv_gpio_int_cd(uint32_t ul_id, uint32_t ul_mask)
{
	uint32_t PinMask = pio_get_pin_group_mask(gpioConfig[DRV_GPIO_PIN_SD_CD].pinId);
	pio_disable_interrupt(PIOB, PinMask);
	uint32_t ReadIsr = PIOB->PIO_ISR;
	if (PinMask == ul_mask)
	{
		gpioConfig[DRV_GPIO_PIN_SD_CD].gpioSetFlag = 1;
	}
	pio_enable_interrupt(PIOB, PinMask);
}

