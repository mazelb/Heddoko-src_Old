/*
 *****************************************************************************************
 * @brief Includes the pin configuration and connection details
 * @author Hriday, Heddoko Inc.
 * @copy Heddoko Inc. 2015.
 * @param[in] None
 * @description 
 *
 * This file specifies the connection of each Quintic to the respective UART/USART ports.
 * Q1 is connected to USART1 (PA22 = TX, PA21 = RX)
 * Q2 is connected to UART0  (PA10 = TX, PA9 = RX)
 * Q3 is connected to USART0 (PA6 = TX, PA5 = RX)
 *****************************************************************************************
 */
#ifndef __BRAINMCU_H
#define __BRAINMCU_H

#include <SAM4S.h>

#define Q1 1
#define Q2 2
#define Q3 3
#define QN_MAX_CONN 3

//struct id{	/* The data structure to stores the address and data of NODs	*/
//	
//	//nod address
//	uint8_t addr[6];
//	
//	//nod data
//	uint8_t data[6];
//};

//struct nod{		/* The data structure to store the ID of NODs	*/ 
//	
//	//	nod ID
//	struct id idx;
//};

//struct nod NOD;

#endif