/*
 * GPIO.c
 *
 * Created: 9/9/2015 12:39:25 PM
 *  Author: Hriday Mehta
 */ 

#include "GPIO.h"

/**
 * ButtonInit(void)
 * @brief Initialize button functionality
 * Power button		: PA0
 * Action Switch 1: PA1
 * Action Switch 2: PA20
 */
void ButtonInit(void) {
  	pmc_enable_periph_clk(PIN_SW0_GPIO);
	gpio_configure_pin(PIN_SW0_GPIO, PIN_SW0_FLAGS);
	pio_enable_interrupt(PIN_SW0_GPIO,PIO_PA2);
	
	/*	Power button	*/     
	pmc_enable_periph_clk(PIN_PW_SW_GPIO);
	gpio_configure_pin(PIN_PW_SW_GPIO, PIN_PW_SW_FLAGS);
	pio_enable_interrupt(PIN_PW_SW_GPIO, PIO_PA0);

	/*	Action Switch 1	*/ 
	pmc_enable_periph_clk(PIN_AC_SW1_GPIO);
	gpio_configure_pin(PIN_AC_SW1_GPIO, PIN_AC_SW1_FLAGS);
	pio_enable_interrupt(PIN_AC_SW1_GPIO, PIO_PA1);

	/*	Action Switch 2	*/
	pmc_enable_periph_clk(PIN_AC_SW2_GPIO);
	gpio_configure_pin(PIN_AC_SW2_GPIO, PIN_AC_SW2_FLAGS);
	pio_enable_interrupt(PIN_AC_SW2_GPIO, PIO_PA20);

	/*	Jack OC 1	*/
	pmc_enable_periph_clk(PIN_JC_OC2_GPIO);
	gpio_configure_pin(PIN_JC_OC2_GPIO, PIN_JC_OC1_FLAGS);
	pio_enable_interrupt(PIN_JC_OC2_GPIO, PIO_PA16);

	/*	Jack OC 2	*/
	pmc_enable_periph_clk(PIN_JC_OC2_GPIO);
	gpio_configure_pin(PIN_JC_OC2_GPIO, PIN_JC_OC2_FLAGS);
	pio_enable_interrupt(PIN_JC_OC2_GPIO, PIO_PA15);
	
	/*	Jack detect 1	*/
	pmc_enable_periph_clk(PIN_JC_DC1_GPIO);
	gpio_configure_pin(PIN_JC_DC1_GPIO, PIN_JC_DC1_FLAGS);
	pio_enable_interrupt(PIN_JC_DC1_GPIO, PIO_PA24);

	/*	Jack detect 2	*/
	pmc_enable_periph_clk(PIN_JC_DC2_GPIO);
	gpio_configure_pin(PIN_JC_DC2_GPIO, PIN_JC_DC2_FLAGS);
	pio_enable_interrupt(PIN_JC_DC2_GPIO, PIO_PA25);

	/*	Jack EN 1	*/
	pmc_enable_periph_clk(PIN_JC_EN1_GPIO);
	gpio_configure_pin(PIN_JC_EN1_GPIO, PIN_JC_EN1_FLAGS);
	pio_enable_interrupt(PIN_JC_EN1_GPIO, PIO_PA19);

	/*	Jack EN 2	*/
	pmc_enable_periph_clk(PIN_JC_EN2_GPIO);
	gpio_configure_pin(PIN_JC_EN2_GPIO, PIN_JC_EN2_FLAGS);
	pio_enable_interrupt(PIN_JC_EN2_GPIO, PIO_PA8);

	/*	BLE RST 1	*/
	pmc_enable_periph_clk(PIN_BLE_RST1_GPIO);
	gpio_configure_pin(PIN_BLE_RST1_GPIO, PIN_BLE_RST1_FLAGS);
	pio_enable_interrupt(PIN_BLE_RST1_GPIO, PIO_PB10);

	/*	BLE RST 2	*/
	pmc_enable_periph_clk(PIN_BLE_RST2_GPIO);
	gpio_configure_pin(PIN_BLE_RST2_GPIO, PIN_BLE_RST2_FLAGS);
	pio_enable_interrupt(PIN_BLE_RST2_GPIO, PIO_PA17);

	/*	BLE RST 3	*/
	pmc_enable_periph_clk(PIN_BLE_RST3_GPIO);
	gpio_configure_pin(PIN_BLE_RST3_GPIO, PIN_BLE_RST3_FLAGS);
	pio_enable_interrupt(PIN_BLE_RST3_GPIO, PIO_PA18);

	/*	Green LED	*/
	pmc_enable_periph_clk(PIN_GREEN_LED_GPIO);
	gpio_configure_pin(PIN_GREEN_LED_GPIO, PIN_GREEN_LED_FLAGS);
	pio_enable_interrupt(PIN_GREEN_LED_GPIO, PIO_PB13);

	/*	Red LED	*/
	pmc_enable_periph_clk(PIN_RED_LED_GPIO);
	gpio_configure_pin(PIN_RED_LED_GPIO, PIN_RED_LED_FLAGS);
	pio_enable_interrupt(PIN_RED_LED_GPIO, PIO_PB1);

	/*	Blue LED	*/
	pmc_enable_periph_clk(PIN_BLUE_LED_GPIO);
	gpio_configure_pin(PIN_BLUE_LED_GPIO, PIN_BLUE_LED_FLAGS);
	pio_enable_interrupt(PIN_BLUE_LED_GPIO, PIO_PB0);

	/*	LBO	*/
	pmc_enable_periph_clk(PIN_LBO_GPIO);
	gpio_configure_pin(PIN_LBO_GPIO, PIN_LBO_FLAGS);
	pio_enable_interrupt(PIN_LBO_GPIO, PIO_PA11);

	/*	STAT	*/
	pmc_enable_periph_clk(PIN_STAT_GPIO);
	gpio_configure_pin(PIN_STAT_GPIO, PIN_STAT_FLAGS);
	pio_enable_interrupt(PIN_STAT_GPIO, PIO_PA2);

	/*	CD	*/
	pmc_enable_periph_clk(PIN_SD_CD_GPIO);
	gpio_configure_pin(PIN_SD_CD_GPIO, PIN_SD_CD_FLAGS);
	pio_enable_interrupt(PIN_SD_CD_GPIO, PIO_PB11);
	
	/*	TIMER CONFIG	*/
	
}


/**
 * button_get(void)
 * @brief Return button states
 * @return Unsigned Int for button state, else 0 for not pressed
 */
unsigned int button_get(void) {
  
  return (~(PIOA->PIO_PDSR) & (PIO_PA2));
}
