/*
 * brd_board.c
 *
 * Created: 2/29/2016 2:38:45 PM
 *  Author: sean
 */ 
#include "brd_board.h"
#include "drv_gpio.h"
#include "drv_uart.h"
#include "drv_led.h"

drv_uart_config_t uart0Config =
{
	.p_usart = UART0,
	.mem_index = 0,
	.uart_options =
	{
		.baudrate   = CONF_BAUDRATE,
		.charlength = CONF_CHARLENGTH,
		.paritytype = CONF_PARITY,
		.stopbits   = CONF_STOPBITS
	}
};
drv_uart_config_t uart1Config =
{
	.p_usart = UART1,
	.mem_index = 1,
	.uart_options =
	{
		.baudrate   = CONF_BAUDRATE,
		.charlength = CONF_CHARLENGTH,
		.paritytype = CONF_PARITY,
		.stopbits   = CONF_STOPBITS
	}
};

drv_led_config_t ledConfiguration =
{
	.redLed = DRV_GPIO_PIN_LED_RED,
	.greenLed = DRV_GPIO_PIN_LED_GREEN,
	.blueLed = DRV_GPIO_PIN_LED_BLUE
};

/**
 * \brief Configure the console UART for using printf.
 */
static void configure_console(void)
{
	const usart_serial_options_t usart_serial_options = 
	{
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS,
	};

	/* Configure console UART. */
	stdio_serial_init(UART1, &usart_serial_options);
	/* Specify that stdout should not be buffered. */
	#if defined(__GNUC__)
		setbuf(stdout, NULL);
	#else
	/* Already the case in IAR's Normal DLIB default configuration: printf()
	 * emits one character at a time.
	 */
	#endif
}

void brd_board_init()
{
	//configure the interrupt handlers first. 
	
	//configure the gpio
	drv_gpio_initializeAll();
	//configure LED driver
	drv_led_init(&ledConfiguration);
	//Set LED white to indicate power on
	drv_led_set(DRV_LED_WHITE,DRV_LED_SOLID);
	vTaskDelay(200);
	pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);
	while (!pmc_osc_is_ready_32kxtal());
	rtc_set_hour_mode(RTC, 0);
	rtc_clear_date_alarm(RTC);
	rtc_clear_time_alarm(RTC);
	drv_led_set(DRV_LED_OFF,DRV_LED_SOLID);	
	brd_initAllUarts();
	configure_console();
	//try to configure the USB
	udc_start(); 
	
}


void brd_initAllUarts()
{
	if(drv_uart_init(&uart0Config) != STATUS_PASS)
	{
		while(1); //spin here
	}
	if(drv_uart_init(&uart1Config) != STATUS_PASS)
	{
		while(1); //spin here
	}
}

void brd_deInitAllUarts()
{
	drv_uart_deInit(&uart0Config);
	drv_uart_deInit(&uart1Config);
}
