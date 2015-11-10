/*
 * Board_Init.c
 *
 * Created: 9/16/2015 5:35:27 PM
 *  Author: Hriday Mehta
 * Copyright Heddoko(TM) 2015, all rights reserved
 */ 

#include "Board_Init.h"
#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "conf_board.h"
#include "Functionality_Tests.h"
#include "settings.h"
#include "drv_uart.h"
#include "drv_gpio.h"
#include "task_main.h"
#include "DebugLog.h"
#include "common.h"
#include "drv_led.h"
#include "rtc.h"

//configuration structures

FATFS fs;

/** Baudrate setting : 115200 */
#define CONF_BAUDRATE   115200
/** Char setting     : 8-bit character length (don't care for UART) */
#define CONF_CHARLENGTH US_MR_CHRL_8_BIT
/** Parity setting   : No parity check */
#define CONF_PARITY     UART_MR_PAR_NO
/** Stopbit setting  : No extra stopbit, i.e., use 1 (don't care for UART) */
#define CONF_STOPBITS   US_MR_NBSTOP_1_BIT


void configureWatchDog();

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
drv_uart_config_t usart0Config =
{
	.p_usart = USART0,
	.mem_index = 2,
	.uart_options =
	{
		.baudrate   = CONF_BAUDRATE,
		.charlength = CONF_CHARLENGTH,
		.paritytype = CONF_PARITY,
		.stopbits   = CONF_STOPBITS
	}
};
drv_uart_config_t usart1Config =
{
	.p_usart = USART1,
	.mem_index = 3,
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
	.redLed = DRV_GPIO_PIN_RED_LED,
	.greenLed = DRV_GPIO_PIN_GREEN_LED,
	.blueLed = DRV_GPIO_PIN_BLUE_LED
};

/**
 * \brief Configure the console UART.
 */
static void configure_console(void)
{
	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_TEST_BAUDRATE,
		.charlength = CONF_TEST_CHARLENGTH,
		.paritytype = CONF_TEST_PARITY,
		.stopbits   = CONF_TEST_STOPBITS,
	};

	/* Configure console UART. */
	stdio_serial_init(UART0, &usart_serial_options);
	/* Specify that stdout should not be buffered. */
	#if defined(__GNUC__)
		setbuf(stdout, NULL);
	#else
	/* Already the case in IAR's Normal DLIB default configuration: printf()
	 * emits one character at a time.
	 */
	#endif
}
/**
 * powerOnInit(void)
 * @brief Initialize the board after power up. 
 */
void powerOnInit(void) 
{		
		static FRESULT res;
		Ctrl_status status;
		pmc_switch_sclk_to_32kxtal(PMC_OSC_XTAL);
		while (!pmc_osc_is_ready_32kxtal());
		rtc_set_hour_mode(RTC, 0);
		rtc_clear_date_alarm(RTC);
		rtc_clear_time_alarm(RTC);
		//configure the gpio
		drv_gpio_initializeAll();
		drv_led_init(&ledConfiguration);
		drv_led_set(DRV_LED_WHITE,DRV_LED_SOLID);
		vTaskDelay(200);  
		drv_led_set(DRV_LED_OFF,DRV_LED_SOLID);
		//drv_gpio_ConfigureBLEForProgramming(); 
		//configure UART1 to be used as a STDIO function
		configure_console();
		//initialize the 
		if(drv_uart_init(&uart0Config) != STATUS_PASS)
		{
			while(1); //spin here
		}		
		if(drv_uart_init(&uart1Config) != STATUS_PASS)
		{
			while(1); //spin here
		}
		if(drv_uart_init(&usart0Config) != STATUS_PASS)
		{
			while(1); //spin here
		}
		if(drv_uart_init(&usart1Config) != STATUS_PASS)
		{
			while(1); //spin here
		}
		
		////Initialize SD card
		//
		//sd_mmc_init();
		//
		///* Wait card present and ready */
		//
		////we don't want the firmware to freeze if we don't have an SD card. 
		//do
		//{
			//status = sd_mmc_test_unit_ready(0);
			//if (CTRL_FAIL == status)
			//{
				//printf("Card install FAIL\n\r");
				//printf("Please unplug and re-plug the card.\n\r");
				//while (CTRL_NO_PRESENT != sd_mmc_check(0))
				//{
				//}
			//}
		//} while (CTRL_GOOD != status);
		//
		//configureWatchDog();
		///*	Mount the SD card	*/
		//memset(&fs, 0, sizeof(FATFS));
		//res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
		//if (res == FR_INVALID_DRIVE)
		//{
			//printf("Error: Invalid Drive\r\n");
			//
		//}
		
		/*	Create a DebugLog.txt file to store Debug information	*/
		//DebugLogCreate();
		
		/*	Perform Read Write Tests	*/
		//if (SDWriteTest() == SUCCESS)
		//{
			////printf("Success: Passed Write Tests\r\n");
			//DebugLogBufPrint("Success: Passed Write Tests\r\n");
		//}
		////DebugLogSave();
		//
		//if (SDReadTest() == SUCCESS)
		//{
			////printf("Success: Passed Read Tests\r\n");
			//DebugLogBufPrint("Success: Passed Read Tests\r\n");
		//}
		//DebugLogSave();
		
		//load the settings
		//if(loadSettings(SETTINGS_FILENAME) != STATUS_PASS)
		//{
			//printf("failed to get read settings\r\n");
		//}
		//DebugLogSave();
}
/**
 *  \brief Handler for watchdog interrupt.
 */
void WDT_Handler(void)
{

	/* Clear status bit to acknowledge interrupt by dummy read. */
	wdt_get_status(WDT);

	drv_uart_putString(&uart0Config, "Restarting system!!!\r");
	//rstc_start_software_reset(RSTC);
	
}

#define WDT_PERIOD                        10000
void configureWatchDog()
{
	pmc_enable_periph_clk(ID_WDT);
	/* Get timeout value. */
	uint32_t timeout_value = wdt_get_timeout_value(WDT_PERIOD * 1000,
			BOARD_FREQ_SLCK_XTAL);
	if (timeout_value == WDT_INVALID_ARGUMENT) {
		while (1) {
			/* Invalid timeout value, error. */
		}
	}
	/* Configure WDT to trigger an interrupt (or reset). */
	uint32_t wdt_mode = WDT_MR_WDRSTEN | WDT_MR_WDFIEN |  /* Enable WDT fault interrupt. */
			WDT_MR_WDDBGHLT  |  /* WDT stops in debug state. */
			WDT_MR_WDIDLEHLT;   /* WDT stops in idle state. */
	
	//
			//WDT_MR_WDFIEN ;
			//|  /* Enable WDT fault interrupt. */
			//WDT_MR_WDDBGHLT	 |  /* WDT stops in debug state. */
			//WDT_MR_WDIDLEHLT;   /* WDT stops in idle state. */
			
			//WDT_MR_WDRPROC   |  /* WDT fault resets processor only. */
	/* Initialize WDT with the given parameters. */
	wdt_init(WDT, wdt_mode, timeout_value, timeout_value);
			
	/* Configure and enable WDT interrupt. */
	NVIC_DisableIRQ(WDT_IRQn);
	NVIC_ClearPendingIRQ(WDT_IRQn);
	NVIC_SetPriority(WDT_IRQn, 0);
	NVIC_EnableIRQ(WDT_IRQn);					
				
}