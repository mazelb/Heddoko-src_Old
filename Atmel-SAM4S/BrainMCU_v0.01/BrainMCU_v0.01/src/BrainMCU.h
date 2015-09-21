/*
 * BrainMCU.h
 *
 * Created: 9/9/2015 10:37:58 AM
 *  Author: Hriday Mehta
 */ 

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


#ifndef BRAINMCU_H_
#define BRAINMCU_H_

#include <asf.h>
#include "sam4s_xplained_pro/sam4s_xplained_pro.h"

#define Q1 USART1
#define Q2 UART0
#define Q3 USART0
#define SS UART1
#define QN_MAX_CONN 3
#define NOD_MAX_CNT 9
#define gBufSize 2	//Buffer Size --> swap out buffer
#define gElementNum 50	//Elements in Buffer --> number of frames saved
#define gDataSize 20	//Number of characters --> frame size (eventually parse frame with \r\n)
#define __UART




static unsigned int sgQ1DataCount=0, sgQ1ElementCount=0, sgQ1BufCount=0;
static unsigned int sgQ2DataCount=0, sgQ2ElementCount=0, sgQ2BufCount=0;
static unsigned int sgQ3DataCount=0, sgQ3ElementCount=0, sgQ3BufCount=0;
static unsigned int cnt=0;

struct id_tag{	/* The data structure to stores the address and data of NODs	*/
	
	//nod address
	char Addr[20];
	
	//nod data
	char Data[gBufSize][gElementNum][gDataSize];	//two data structure buffers with 10 elements of 20 bytes of data
	
	unsigned int DataCount, ElementCount, BufCount;
};

struct NOD{		/* The data structure to store the ID of NODs	*/
	
	//	nod ID
	struct id_tag id[NOD_MAX_CNT];
};

struct QN{

	struct id_tag id[QN_MAX_CONN];
};

extern struct QN Qn;
extern struct NOD Nod;
extern struct id_tag id;

#endif /* BRAINMCU_H_ */