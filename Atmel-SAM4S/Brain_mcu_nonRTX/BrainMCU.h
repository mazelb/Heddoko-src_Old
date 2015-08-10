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
#define bufsize 2
#define elmntnb 50
#define datasize 20

static unsigned int q1i=0, q1j=0, q1k=0;
static unsigned int q2i=0, q2j=0, q2k=0;
static unsigned int q3i=0, q3j=0, q3k=0;

struct id_tag{	/* The data structure to stores the address and data of NODs	*/
	
	//nod address
	char addr[20];
	
	//nod data
	char data[bufsize][elmntnb][datasize];	//two data structure buffers with 10 elements of 20 bytes of data
};

struct NOD{		/* The data structure to store the ID of NODs	*/ 
	
	//	nod ID
	struct id_tag id[9];
};

struct QN{

	struct id_tag id[3];
};

static struct QN qn;
static struct NOD nod;
static struct id_tag id;

#endif