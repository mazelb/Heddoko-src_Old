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
#include "BrainMCU.h"
#include "Serial.h"
#include "UART_functionality.h"
#include "Tasks.h"
#include "LED.h"
#include "Button.h"

static uint8_t i=0;

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
	
	//	Enable interrupts on USART0
	USART0->US_IER=US_IER_RXBUFF;
	USART0->US_IER=US_IER_ENDRX;
	USART0->US_IER=US_IER_RXRDY;
	
}

//Interrupt Handler for reading UART0
int read_uart0(char idx){

	uint8_t i=0;
	if((UART0->UART_SR & UART_SR_RXRDY)&&(UART0->UART_SR & UART_SR_RXBUFF)){
			//printf("IF loop");
			UART0->UART_IER=UART_IDR_RXBUFF;
//			qn.id[1].data[q2i][q2j][q2k]=UART0->UART_RHR;
			i=UART0->UART_RHR;
			ser_putchar_ur1(i);
			i=0;
			UART0->UART_IER=UART_IER_RXBUFF;
		}
}

	//Interrupt Handler for USART1
int read_usart1(char idx){
	
	if((USART1->US_CSR & US_CSR_RXRDY)&&(USART1->US_CSR & US_CSR_RXBUFF)){
		//printf("IF loop");
		USART1->US_IER=US_IDR_RXBUFF;
//		qn.id[0].data[q1i][q1j][q1k]=USART1->US_RHR;
		nod.id[idx].data[nod.id[idx].buf_cnt][nod.id[idx].elmnt_cnt][nod.id[idx].data_cnt]=USART1->US_RHR;
		ser_putchar_ur1(nod.id[idx].data[nod.id[idx].buf_cnt][nod.id[idx].elmnt_cnt][nod.id[idx].data_cnt]);
		if(nod.id[idx].data[nod.id[idx].buf_cnt][nod.id[idx].elmnt_cnt][nod.id[idx].data_cnt]!='\0'){		//Heddoko: To increment the buffer only if a character is received
			nod.id[idx].data_cnt++;
//			cnt++;
		}
//		i=USART1->US_RHR;	
//		ser_putchar_ur1(i);
//		i=0;
		USART1->US_IER=US_IER_RXBUFF;
	}
}

//Interrupt Handler for USART0
int read_usart0(char idx){
	
	uint8_t i=0;
	if((USART0->US_CSR & US_CSR_RXRDY)&&(USART0->US_CSR & US_CSR_RXBUFF)){
		//printf("IF loop");
		USART0->US_IER=US_IDR_RXBUFF;
//		qn.id[2].data[q3i][q3j][q3k]=USART0->US_RHR;
		i=USART0->US_RHR;
		ser_putchar_ur1(i);
		i=0;
		USART0->US_IER=US_IER_RXBUFF;
	}
}