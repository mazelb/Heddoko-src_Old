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
 * BoardInit(void)
 * @brief Initialize the board
 */
void BoardInit(void) 
{
		//Initialize system clock and peripherals
		sysclk_init();
		board_init();
		
		//configure UART1 to be used as a STDIO function
		configure_console();

		//Initialize Serial peripherals
		SerialInit();
		
		//Initialize GPIOs
		ButtonInit();
		
		//Initialize Serial interrupts
		UartUsartInit();
		
		//Initialize SD card
		sd_mmc_init();
		
		/* Wait card present and ready */
		do
		{
			status = sd_mmc_test_unit_ready(0);
			if (CTRL_FAIL == status)
			{
				SerialPrint(SS, "Card install FAIL\n\r");
				SerialPrint(SS, "Please unplug and re-plug the card.\n\r");
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
}