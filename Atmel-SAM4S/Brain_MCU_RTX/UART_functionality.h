/**
 * @file UART_functionality.h
 * @author Ron Brash - Heddoko 
 * @date July 22nd, 2015
 * @brief Handle all UART/USART functionality
 *
 */

#ifndef __UART_H
#define __UART_H

void UART_USART_init(void);
int read_uart0	(char idx);
int read_usart0	(char idx);
int read_usart1	(char idx);

#endif
