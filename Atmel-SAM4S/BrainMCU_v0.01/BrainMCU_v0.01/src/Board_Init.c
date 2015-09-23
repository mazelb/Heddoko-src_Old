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
#include "UART_functionality.h"
#include "BrainMCU.h"
#include "Config_Settings.h"
#include "drv_uart.h"

//configuration structures


/** Baudrate setting : 115200 */
#define CONF_BAUDRATE   115200
/** Char setting     : 8-bit character length (don't care for UART) */
#define CONF_CHARLENGTH 0
/** Parity setting   : No parity check */
#define CONF_PARITY     UART_MR_PAR_NO
/** Stopbit setting  : No extra stopbit, i.e., use 1 (don't care for UART) */
#define CONF_STOPBITS   false


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
 * powerOnInit(void)
 * @brief Initialize the board after power up. 
 */
void powerOnInit(void) 
{
		//Initialize system clock and peripherals
		sysclk_init();
		board_init();
		
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
		
		//Initialize GPIOs
		//ButtonInit();		
		
		//Initialize SD card
		
		sd_mmc_init();
		
		/* Wait card present and ready */
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
		
		//load the settings
		if(loadSettings(SETTINGS_FILENAME) != STATUS_PASS)
		{
			printf("failed to get read settings\r\n"); 
		} 
}