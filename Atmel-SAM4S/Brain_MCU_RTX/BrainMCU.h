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
#define NOD_MAX_CNT 9
#define gBufSize 2	//Buffer Size
#define gElementNum 50	//Elements in Buffer
#define gDataSize 20	//Number of characters

static char sgaNodAddr[9][20]={"A0E5E900133B\r\n",
															"A0E5E90016DD\r\n",
															"A0E5E90012CD\r\n",
															"A0E5E900139B\r\n",
															"A0E5E9001717\r\n",
															"A0E5E90011FD\r\n",
															"A0E5E90016E6\r\n",
															"A0E5E9001398\r\n",
															"A0E5E9000387\r\n"
															};

static unsigned int sgQ1DataCount=0, sgQ1ElementCount=0, sgQ1BufCount=0;
static unsigned int sgQ2DataCount=0, sgQ2ElementCount=0, sgQ2BufCount=0;
static unsigned int sgQ3DataCount=0, sgQ3ElementCount=0, sgQ3BufCount=0;													
static unsigned int cnt=0;
															
struct id_tag			/* The data structure to stores the address and data of NODs	*/
{		
	//nod address
	char Addr[20];
	
	//nod data
	char Data[gBufSize][gElementNum][gDataSize];	//two data structure buffers with 10 elements of 20 bytes of data
	
	unsigned int DataCount, ElementCount, BufCount;
};

struct NOD			/* The data structure to store the ID of NODs	*/ 
{	
	//	nod ID
	struct id_tag id[NOD_MAX_CNT];
};

struct QN
{
	struct id_tag id[3];
};

extern struct QN Qn;
extern struct NOD Nod;
extern struct id_tag id;

#endif