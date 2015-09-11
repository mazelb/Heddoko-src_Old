/**
 * @file UART_functionality.c
 * @author Hriday Mehta - Heddoko 
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

/**
 * UartUsartInit(void)
 * @brief Initialize UART/USARTs
 */
void UartUsartInit(void) 
{	
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
int ReadUart0(char idx)
{
	uint8_t i=0;
	if((UART0->UART_SR & UART_SR_RXRDY)&&(UART0->UART_SR & UART_SR_RXBUFF))
	{
		UART0->UART_IER=UART_IDR_RXBUFF;
//			qn.id[1].Data[sgQ2Datacount][sgQ2ElementCount][sgQ2BufCount]=UART0->UART_RHR;			// Store the received data in Qn buffer
		i=UART0->UART_RHR;
		SerialPutCharUart1(i);																																	//Print the data received directly on the serial terminal
		i=0;
		UART0->UART_IER=UART_IER_RXBUFF;
	}
}

	//Interrupt Handler for USART1
int ReadUsart1(char idx)
{
	static uint8_t i=0;
	if((USART1->US_CSR & US_CSR_RXRDY)&&(USART1->US_CSR & US_CSR_RXBUFF))
	{		
		USART1->US_IER=US_IDR_RXBUFF;
//		qn.id[0].Data[sgQ1Datacount][sgQ1ElementCount][sgQ1BufCount]=USART1->US_RHR;			// Store the received data in Qn buffer
		Nod.id[idx].Data[Nod.id[idx].BufCount][Nod.id[idx].ElementCount][Nod.id[idx].DataCount]=USART1->US_RHR;
		SerialPutCharUart1(Nod.id[idx].Data[Nod.id[idx].BufCount][Nod.id[idx].ElementCount][Nod.id[idx].DataCount]);
		if(Nod.id[idx].Data[Nod.id[idx].BufCount][Nod.id[idx].ElementCount][Nod.id[idx].DataCount]!='\0')					//Heddoko: To increment the buffer only if a character is received
		{		
			Nod.id[idx].DataCount++;
		}
//		i=USART1->US_RHR;	
//		SerialPutCharUart1(i);																																	//Print the data received directly on the serial terminal
//		i=0;
		USART1->US_IER=US_IER_RXBUFF;
	}
}

//Interrupt Handler for USART0
int ReadUsart0(char idx)
{	
	uint8_t i=0;
	if((USART0->US_CSR & US_CSR_RXRDY)&&(USART0->US_CSR & US_CSR_RXBUFF))
	{
		USART0->US_IER=US_IDR_RXBUFF;
//		qn.id[2].Data[sgQ3Datacount][sgQ3ElementCount][sgQ3BufCount]=USART0->US_RHR;			// Store the received data in Qn buffer
		i=USART0->US_RHR;
		SerialPutCharUart1(i);																																	//Print the data received directly on the serial terminal
		i=0;
		USART0->US_IER=US_IER_RXBUFF;
	}
}