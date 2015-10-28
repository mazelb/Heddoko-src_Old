/*
 * Board_Init.c
 *
 * Created: 9/16/2015 5:35:27 PM
 *  Author: Hriday Mehta
 */ 

#include "Board_Init.h"
#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "conf_board.h"
#include "Functionality_Tests.h"
#include "BrainMCU.h"
#include "Config_Settings.h"
#include "drv_uart.h"
#include "drv_gpio.h"
#include "task_main.h"
#include "DebugLog.h"
#include "common.h"
#include "drv_led.h"
#include "rtc.h"

//configuration structures

static FATFS fs;

/** Baudrate setting : 115200 */
#define CONF_BAUDRATE   115200
/** Char setting     : 8-bit character length (don't care for UART) */
#define CONF_CHARLENGTH US_MR_CHRL_8_BIT
/** Parity setting   : No parity check */
#define CONF_PARITY     UART_MR_PAR_NO
/** Stopbit setting  : No extra stopbit, i.e., use 1 (don't care for UART) */
#define CONF_STOPBITS   US_MR_NBSTOP_1_BIT


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
	stdio_serial_init(CONF_UART, &usart_serial_options);
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
 * \brief Calculate week from year, month, day.
 */
static uint32_t calculateDayOfWeek(uint32_t ul_year, uint32_t ul_month, uint32_t ul_day)
{
	uint32_t ul_week;

	if (ul_month == 1 || ul_month == 2) {
		ul_month += 12;
		--ul_year;
	}

	ul_week = (ul_day + 2 * ul_month + 3 * (ul_month + 1) / 5 + ul_year + ul_year / 4 - ul_year / 100 + ul_year / 400) % 7;

	++ul_week;

	return ul_week;
}

/**
 * \brief Calculate numeric value of month from string.
 */
static void checkMonth(char* c_date, uint32_t* u_month)
{
	//Converting month from Date string to BCD format
	if (strncmp(c_date, "Jan", 3) == 0)
	{
		*u_month = 1;
	}
	else if (strncmp(c_date, "Feb", 3) == 0)
	{
		*u_month = 2;
	}
	else if (strncmp(c_date, "Mar", 3) == 0)
	{
		*u_month = 3;
	}
	else if (strncmp(c_date, "Apr", 3) == 0)
	{
		*u_month = 4;
	}
	else if (strncmp(c_date, "May", 3) == 0)
	{
		*u_month = 5;
	}
	else if (strncmp(c_date, "Jun", 3) == 0)
	{
		*u_month = 6;
	}
	else if (strncmp(c_date, "Jul", 3) == 0)
	{
		*u_month = 7;
	}
	else if (strncmp(c_date, "Aug", 3) == 0)
	{
		*u_month = 8;
	}
	else if (strncmp(c_date, "Sep", 3) == 0)
	{
		*u_month = 9;
	}
	else if (strncmp(c_date, "Oct", 3) == 0)
	{
		*u_month = 10;
	}
	else if (strncmp(c_date, "Nov", 3) == 0)
	{
		*u_month = 11;
	}
	else if (strncmp(c_date, "Dec", 3) == 0)
	{
		*u_month = 12;
	}
	
}

/**
 * powerOnInit(void)
 * @brief Initialize the board after power up. 
 */
void powerOnInit(void) 
{		
		char vMonthString[3] = {0}, vCompileDate[12] = {0};
		uint32_t hour, minute, second, date, month, year, day;
		static FRESULT res;
		
		//configure the gpio
		drv_gpio_initializeAll();
		drv_led_init(&ledConfiguration);
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
		
		/*	RTC stuff	*/
		strncpy(vCompileDate, __DATE__, 11);	//copy the compile time to a local variable
		
		//Converting Time string to BCD format
		if(sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second) !=3)
		{
			printf("Invalid Time input\r\n");
		}
		
		//converting Date string to BCD format
		checkMonth(vCompileDate, &month);	//check for the value of month
		if (sscanf(vCompileDate, "%s %d %d", vMonthString, &date, &year))	//store the value of date and year
		{
			printf("Invalid Date input\r\n");
		}
		day = calculateDayOfWeek(year, month, date);
		
		//Configuring RTC
		rtc_set_hour_mode(RTC, 0);
		if (rtc_set_time(RTC, hour, minute, second))
		{
			puts("\n\rTime not set, invalid input\r\n");
		}
		if (rtc_set_date(RTC, year, month, date, day))
		{
			puts("\n\rDate not set, invalid input\r\n");
		}
		rtc_clear_date_alarm(RTC);
		rtc_clear_time_alarm(RTC);
		
		////Initialize SD card
		//
		sd_mmc_init();
		//
		///* Wait card present and ready */
		//
		////we don't want the firmware to freeze if we don't have an SD card. 
		do
		{
			status = sd_mmc_test_unit_ready(0);
			if (CTRL_FAIL == status)
			{
				printf("Card install FAIL\n\r");
				printf("Please unplug and re-plug the card.\n\r");
				while (CTRL_NO_PRESENT != sd_mmc_check(0))
				{
				}
			}
		} while (CTRL_GOOD != status);
		
		
		/*	Mount the SD card	*/
		memset(&fs, 0, sizeof(FATFS));
		res = f_mount(LUN_ID_SD_MMC_0_MEM, &fs);
		if (res == FR_INVALID_DRIVE)
		{
			printf("Error: Invalid Drive\r\n");
			return 0;
		}
		
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
		if(loadSettings(SETTINGS_FILENAME) != STATUS_PASS)
		{
			printf("failed to get read settings\r\n");
		}
		//DebugLogSave();
}