/*
 *****************************************************************************************
 * @brief Includes the pin configuration and connection details
 * @author Hriday, Heddoko Inc.
 * @copy Heddoko Inc. 2015.
 * @param[in] None
 * @description 
 *,p
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
#define bufsize 2
#define elmntnb 50
#define datasize 20

static char nod_addr[9][20]={"A0E5E900133B\r\n",
															"A0E5E90016DD\r\n",
															"A0E5E90012CD\r\n",
															"A0E5E900139B\r\n",
															"A0E5E9001717\r\n",
															"A0E5E90011FD\r\n",
															"A0E5E90016E6\r\n",
															"A0E5E9001398\r\n",
															"A0E5E9000387\r\n"
															};

static unsigned int q1i=0, q1j=0, q1k=0;
static unsigned int q2i=0, q2j=0, q2k=0;
static unsigned int q3i=0, q3j=0, q3k=0;													
static unsigned int cnt=0;
															
struct id_tag{	/* The data structure to stores the address and data of NODs	*/
	
	//nod address
	char addr[20];
	
	//nod data
	char data[bufsize][elmntnb][datasize];	//two data structure buffers with 10 elements of 20 bytes of data
	
	unsigned int data_cnt, elmnt_cnt, buf_cnt;
};

struct NOD{		/* The data structure to store the ID of NODs	*/ 
	
	//	nod ID
	struct id_tag id[NOD_MAX_CNT];
};

struct QN{

	struct id_tag id[3];
};

extern struct QN qn;
extern struct NOD nod;
extern struct id_tag id;

#endif