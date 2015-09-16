/**
 * @file UART_functionality.h
 * @author Hriday Mehta - Heddoko 
 * @date July 22nd, 2015
 * @brief Handle all UART/USART functionality
 *
 */

#ifndef __UART_H
#define __UART_H

void UartUsartInit	(void);
int ReadUart0				(char idx);
int ReadUsart0			(char idx);
int ReadUsart1			(char idx);

#endif
