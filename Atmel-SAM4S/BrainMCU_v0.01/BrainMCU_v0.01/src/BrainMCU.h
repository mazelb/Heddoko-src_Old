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
#include <ctrl_access.h>

#define Q1 1
#define Q2 2
#define Q3 3
#define SS 4
//#define QN_MAX_CONN 3
//#define IMU_MAX_CNT 9
//#define gBufSize 2	//Buffer Size
//#define gElementNum 50	//Elements in Buffer
//#define gDataSize 20	//Number of characters
#define __UART
#define DATA_SIZE 2048

/*	Error Codes	*/
#define SUCCESS			0
#define INVALID_DRIVE	1
#define CANNOT_OPEN		2
#define WRITE_FAILED	3
#define READ_FAILED		4
#define CANNOT_CLOSE	5
#define UNKNOWN_ERROR	255



static unsigned int cnt=0;

/*	RTOS variables, semaphores, Mutexes, functions	*/
xSemaphoreHandle DebugLogSemaphore;



static Ctrl_status status;





//struct id_tag{	/* The data structure to stores the address and data of NODs	*/
	//
	////imu address
	//char Addr[20];
	//
	////imu data
	//char Data[gBufSize][gElementNum][gDataSize];	//two data structure buffers with 10 elements of 20 bytes of data
	//
	//unsigned int DataCount, ElementCount, BufCount;
//};
//
//struct IMU{		/* The data structure to store the ID of NODs	*/
	//
	////	imu ID
	//struct id_tag id[IMU_MAX_CNT];
//};
//
//struct QN{
//
	//struct id_tag id[QN_MAX_CONN];
//};
//
//extern struct DEBUGLOG DebugLog;
//extern struct IMU Imu;
//extern struct id_tag id;

#endif /* BRAINMCU_H_ */