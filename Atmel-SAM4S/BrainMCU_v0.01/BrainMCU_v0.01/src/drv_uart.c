/*
 * drv_uart.c
 *
 * Created: 9/21/2015 8:27:54 AM
 *  Author: Sean Cloghesy 
 */ 
/**
 * @file  drv_uart.c
 * @brief Low level uart driver with circular buffer on receive
 * @author Heddoko
 * @note This driver can be used on UART0, UART1, USART0 and USART1
 *
 * @copy Heddoko (2015)
 */
#include "drv_uart.h"
#include <interrupt.h>
#include <string.h>
#include <stdio.h>
typedef struct
{
	sw_fifo_typedef rx_fifo; 
	uint8_t isinit; 
	uint8_t uart_rx_fifo_not_empty_flag; // this flag is automatically set and cleared by the software buffer
	uint8_t uart_rx_fifo_full_flag;      // this flag is automatically set and cleared by the software buffer
	uint8_t uart_rx_fifo_ovf_flag;       // this flag is not automatically cleared by the software buffer		
}drv_uart_memory_buf_t;
//global variables
volatile drv_uart_memory_buf_t uartMemBuf[4]; //4 UARTS, 4 buffers
//static function declarations
static int uart_get_byte(drv_uart_memory_buf_t* memBuf, char* c); 
static void uart_process_byte(Usart *p_usart, drv_uart_memory_buf_t* memBuf);

/***********************************************************************************************
 * drv_uart_init(drv_uart_config_t* uartConfig)
 * @brief initialize uart driver and circular buffer
 * @param uartConfig, the configuration structure for the uart, contains settings.  
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error
 ***********************************************************************************************/	
status_t drv_uart_init(drv_uart_config_t* uartConfig)
{
	status_t status = STATUS_PASS; 
	
	if(uartConfig->p_usart == UART0)
	{
		uartConfig->mem_index = 0;
	}
	else if(uartConfig->p_usart == UART1)
	{
		uartConfig->mem_index = 1;
	}
	else if(uartConfig->p_usart == USART0)
	{
		uartConfig->mem_index = 2;
	}
	else if(uartConfig->p_usart == USART1)
	{
		uartConfig->mem_index = 3;
	}
	else
	{
		//ERROR! The config settings have an invalid UART pointer
		return STATUS_FAIL;
	}

	//initialize the circular buffers. 
	if(uartConfig->mem_index < 0 || uartConfig->mem_index > 4)
	{
		//ERROR! an incorrect buffer index has been used.  
		return STATUS_FAIL; 
	}
	//make sure the buffer isn't already initialized. 
	if(uartMemBuf[uartConfig->mem_index].isinit == true)
	{
		//buffer already in use, call drv_uart_deinit() first
		return STATUS_FAIL; 
	}
	//clear the buffer
	memset(uartMemBuf[uartConfig->mem_index].rx_fifo.data_buf, 0,FIFO_BUFFER_SIZE);
	uartMemBuf[uartConfig->mem_index].rx_fifo.i_first = 0;
	uartMemBuf[uartConfig->mem_index].rx_fifo.i_last = 0;
	uartMemBuf[uartConfig->mem_index].uart_rx_fifo_full_flag = 0;
	uartMemBuf[uartConfig->mem_index].uart_rx_fifo_not_empty_flag = 0;
	uartMemBuf[uartConfig->mem_index].uart_rx_fifo_ovf_flag = 0;
	//initialize the UART hardware 
	usart_serial_init(uartConfig->p_usart, &uartConfig->uart_options);
	
	//setup interrupts for the UART
	/* Disable all the interrupts. */
	usart_disable_interrupt(uartConfig->p_usart, ALL_INTERRUPT_MASK);
	/* Configure and enable interrupt of USART. */
	if(uartConfig->p_usart == UART0)
	{
		NVIC_EnableIRQ(UART0_IRQn);
	}
	else if(uartConfig->p_usart == UART1)
	{
		NVIC_EnableIRQ(UART1_IRQn);
	}
	else if(uartConfig->p_usart == USART0)
	{
		NVIC_EnableIRQ(USART1_IRQn);
	}
	else if(uartConfig->p_usart == USART1)
	{
		NVIC_EnableIRQ(USART1_IRQn);
	}
	else
	{
		//ERROR! The config settings have an invalid UART pointer
		return STATUS_FAIL;
	}
	usart_enable_interrupt(uartConfig->p_usart, 0x01); //enable RXRDY interrupt	
	uartMemBuf[uartConfig->mem_index].isinit = true;
	return status; 
}
/***********************************************************************************************
 * drv_uart_putChar(drv_uart_config_t* uartConfig, char c)
 * @brief sends a char through the configured uart
 * @param uartConfig, the configuration structure for the uart, used to identify where to send 
 * the character
 * @param c, the character that will be sent through the uart. 
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error
 ***********************************************************************************************/	
status_t drv_uart_putChar(drv_uart_config_t* uartConfig, char c)
{
	status_t status = STATUS_PASS;
	//will return 0 if byte has been sent (make sure to check return)
	//TODO possibly change this to blocking, or buffered output. 
	if(uart_write(uartConfig->p_usart, c) != 0) 
	{
		status = STATUS_FAIL; 
	}
	return status;	
}
/***********************************************************************************************
 * drv_uart_getChar(drv_uart_config_t* uartConfig, char* c)
 * @brief reads a char from the buffer
 * @param uartConfig, the configuration structure for the uart, used to identify where to read 
 * the character from
 * @param *c, the return character
 * @return STATUS_PASS if successful, STATUS_EOF if there were no bytes to read
 *	STATUS_FAIL if there is an error
 ***********************************************************************************************/	
status_t drv_uart_getChar(drv_uart_config_t* uartConfig, char* c)
{
	status_t status = STATUS_PASS;
	usart_disable_interrupt(uartConfig->p_usart, ALL_INTERRUPT_MASK);
	if(uartMemBuf[uartConfig->mem_index].uart_rx_fifo_not_empty_flag == 1) //check if the buffer has information in it
	{
		//disable the interrupts so we don't fuck up the pointers		
		status = uart_get_byte(&(uartMemBuf[uartConfig->mem_index]), c); //get the byte from the buffer	
	}
	else
	{
		status = STATUS_EOF; //there's no data return End Of File status code.
	}
	//re-enable the interrupts
	usart_enable_interrupt(uartConfig->p_usart, 0x01);			
	return status;	
}
/***********************************************************************************************
 * drv_uart_deInit(drv_uart_config_t* uartConfig)
 * @brief puts the uart driver in a deinitialized state
 * @param uartConfig, the configuration structure for the uart, 
 * @return STATUS_PASS if successful,	STATUS_FAIL if there is an error
 ***********************************************************************************************/	
status_t drv_uart_deInit(drv_uart_config_t* uartConfig)
{
	status_t status = STATUS_PASS;
	/* Disable all the interrupts. */
	usart_disable_interrupt(uartConfig->p_usart, ALL_INTERRUPT_MASK);
	uartMemBuf[uartConfig->mem_index].isinit = false;	
	return status;	
}
/***********************************************************************************************
 * drv_uart_isInit(drv_uart_config_t* uartConfig)
 * @brief returns the initialized status of the uart
 * @param uartConfig, the configuration structure for the uart
 * @return STATUS_PASS if the driver is initialized,	STATUS_FAIL if not
 ***********************************************************************************************/	
status_t drv_uart_isInit(drv_uart_config_t* uartConfig)
{
	if(uartMemBuf[uartConfig->mem_index].isinit)
	{
		return STATUS_PASS;
	}
	else
	{
		return STATUS_FAIL; 
	}
}

status_t drv_uart_getline(drv_uart_config_t* uartConfig, char* str, size_t strSize)
{
	status_t result = STATUS_PASS;
	char val;
	int pointer = 0;
	while(1) //TODO add timeout
	{
		result = drv_uart_getChar(uartConfig,&val);
		if(result != STATUS_EOF && val != NULL)
		{
			if(pointer < strSize)
			{
				str[pointer++] = val; //add the result;
				if(val == '\n')
				{
					str[pointer] = NULL; //terminate the string
					result = STATUS_PASS;
					pointer = 0; //reset the pointer.
					break;
				}
			}
			else
			{
				//we overwrote the buffer
				result = STATUS_FAIL;
				str[strSize - 1] = NULL; //terminate what's in the buffer.
				pointer = 0;
				break;
			}
		}
		vTaskDelay(10); //let the other processes do stuff
	}
	return result; 
}

// interrupt handlers
void UART0_Handler()
{	
	if(uartMemBuf[0].isinit) //only handle the interrupt if the driver is initialized. 
	{
		uart_process_byte(UART0, &(uartMemBuf[0]));	
	}
}

void UART1_Handler()
{
	if(uartMemBuf[1].isinit) //only handle the interrupt if the driver is initialized.
	{
		uart_process_byte(UART1, &(uartMemBuf[1]));
	}
}

void USART0_Handler()
{
	if(uartMemBuf[2].isinit) //only handle the interrupt if the driver is initialized.
	{
		uart_process_byte(USART0, &(uartMemBuf[2]));
	}
}

void USART1_Handler()
{
	if(uartMemBuf[3].isinit) //only handle the interrupt if the driver is initialized.
	{
		uart_process_byte(USART1, &(uartMemBuf[3]));
	}
}


//static functions
static int uart_get_byte(drv_uart_memory_buf_t* memBuf, char* c)
{
	status_t status = STATUS_PASS;
	if(memBuf->rx_fifo.num_bytes == FIFO_BUFFER_SIZE)
	{ // if the sw buffer is full
		memBuf->uart_rx_fifo_full_flag = 0;               // clear the buffer full flag because we are about to make room
	}
	if(memBuf->rx_fifo.num_bytes > 0)
	{
		// if data exists in the sw buffer
		*c = memBuf->rx_fifo.data_buf[memBuf->rx_fifo.i_first]; // grab the oldest element in the buffer
		memBuf->rx_fifo.i_first++;                        // increment the index of the oldest element
		memBuf->rx_fifo.num_bytes--;                      // decrement the bytes counter
	}
	else
	{                                      // RX sw buffer is empty
		memBuf->uart_rx_fifo_not_empty_flag = 0;          // clear the rx flag
		status = STATUS_EOF;
	}
	if(memBuf->rx_fifo.i_first == FIFO_BUFFER_SIZE)
	{   // if the index has reached the end of the buffer,
		memBuf->rx_fifo.i_first = 0;                      // roll over the index counter
	}		
	return status;
}

static void uart_process_byte(Usart *p_usart, drv_uart_memory_buf_t* memBuf)
{
	if(memBuf->rx_fifo.num_bytes == FIFO_BUFFER_SIZE) // if the sw buffer is full
	{
		memBuf->uart_rx_fifo_ovf_flag = 1;                     // set the overflow flag
	}
	else if(memBuf->rx_fifo.num_bytes < FIFO_BUFFER_SIZE)  // if there's room in the sw buffer
	{
		uint32_t val = 0;
		//while((UART0->UART_SR & UART_SR_RXRDY) == 0);
		if(usart_getchar(p_usart,&val) == STATUS_PASS)
		{		
			memBuf->rx_fifo.data_buf[memBuf->rx_fifo.i_last] = (uint8_t)val;
			memBuf->rx_fifo.i_last++;                              // increment the index of the most recently added element
			memBuf->rx_fifo.num_bytes++;                           // increment the bytes counter
		}
	}
	if(memBuf->rx_fifo.num_bytes == FIFO_BUFFER_SIZE)
	{      // if sw buffer just filled up
		memBuf->uart_rx_fifo_full_flag = 1;                    // set the RX FIFO full flag
	}
	if(memBuf->rx_fifo.i_last == FIFO_BUFFER_SIZE)
	{         // if the index has reached the end of the buffer,
		memBuf->rx_fifo.i_last = 0;                            // roll over the index counter
	}
	memBuf->uart_rx_fifo_not_empty_flag = 1;                 // set received-data flag	
}