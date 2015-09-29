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
#define QN_MAX_CONN 3
#define IMU_MAX_CNT 9
#define gBufSize 2	//Buffer Size
#define gElementNum 50	//Elements in Buffer
#define gDataSize 20	//Number of characters
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

static char sgaImuAddr[9][20]={"A0E5E900133B\r\n",
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

/*	RTOS variables, semaphores, Mutexes, functions	*/
xSemaphoreHandle DebugLogSemaphore;

/*	SD Card FAT-FS variables	*/
static char file_name[] = "0:Heddoko.txt";
static char test_file_name[] = "0:sd_mmc_test.txt";

static Ctrl_status status;
static FRESULT res;
static FATFS fs;
static FIL file_object;
static bool sgSDWriteLockBit = true, sgDebugLogAutoSave = false;

/*	Global buffer to store Debug Log	*/
struct DEBUGLOG
{
	char DebugLogBuf[DATA_SIZE];
	unsigned int DebugLogBufTail, DebugLogBufHead;
	bool DebugLockBit;
};

struct id_tag{	/* The data structure to stores the address and data of NODs	*/
	
	//imu address
	char Addr[20];
	
	//imu data
	char Data[gBufSize][gElementNum][gDataSize];	//two data structure buffers with 10 elements of 20 bytes of data
	
	unsigned int DataCount, ElementCount, BufCount;
};

struct IMU{		/* The data structure to store the ID of NODs	*/
	
	//	imu ID
	struct id_tag id[IMU_MAX_CNT];
};

struct QN{

	struct id_tag id[QN_MAX_CONN];
};

extern struct DEBUGLOG DebugLog;
extern struct IMU Imu;
extern struct id_tag id;

#endif /* BRAINMCU_H_ */