/**
 * @file UART_functionality.c
 * @author Ron Brash - Heddoko 
 * @date July 22nd, 2015
 * @brief Handle all UART/USART functionality
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <rtl.h>
#include <SAM4S.h>

#include "Serial.h"
#include "UART_functionality.h"
#include "Tasks.h"
#include "LED.h"
#include "Button.h"

/**
 * UART_USART_init(void)
 * @brief Initialize UART/USARTs
 */
void UART_USART_init(void) {
	
	//	Enable interrupts on UART0
	UART0->UART_IER=UART_IER_RXBUFF;
	UART0->UART_IER=UART_IER_ENDRX;
	UART0->UART_IER=UART_IER_RXRDY;
	
	//	Enable interrupts on USART1
	USART1->US_IER=US_IER_RXBUFF;
	USART1->US_IER=US_IER_ENDRX;
	USART1->US_IER=US_IER_RXRDY;
	
}

/* 
 * read_uart0()
 * @brief Interrupt Handler for reading UART0
 */
void read_uart0(void){

	uint8_t i=0;
	if((UART0->UART_SR & UART_SR_RXRDY)&&(UART0->UART_SR & UART_SR_RXBUFF)){
			UART0->UART_IER=UART_IDR_RXBUFF;
			i=UART0->UART_RHR;
			serial_put_char_ur0(i);
			i=0;
			UART0->UART_IER=UART_IER_RXBUFF;
		}
}

/*
 * read_usart1()
 * @brief Interrupt Handler for USART1
 */
void read_usart1(void){
	
	uint8_t i=0;
	if((USART1->US_CSR & US_CSR_RXRDY)&&(USART1->US_CSR & US_CSR_RXBUFF)){
		USART1->US_IER=US_IDR_RXBUFF;
		i=USART1->US_RHR;
		serial_put_char_ur0(i);
		i=0;
		USART1->US_IER=US_IER_RXBUFF;
	}
}
