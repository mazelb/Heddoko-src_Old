/*
 * UART_functionality.h
 * @brief Handle all UART/USART functionality
 * Created: 9/9/2015 11:39:42 AM
 *  Author: Hriday Mehta
 */ 


#ifndef UART_FUNCTIONALITY_H_
#define UART_FUNCTIONALITY_H_


void UartUsartInit	(void);
int ReadUart0		(char idx);
int ReadUsart0		(char idx);
int ReadUsart1		(char idx);


#endif /* UART_FUNCTIONALITY_H_ */