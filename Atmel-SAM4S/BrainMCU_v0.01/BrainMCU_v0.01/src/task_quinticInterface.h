/*
 * task_quinticInterface.h
 *
 * Created: 9/18/2015 10:50:03 AM
 *  Author: Sean Cloghesy
 */ 


#ifndef TASK_QUINTIC_INTERFACE_H_
#define TASK_QUINTIC_INTERFACE_H_

#include "common.h"
#include "drv_uart.h"

#define NOD_MAC_ADDRESS_LENGTH 12+1 //plus 1 for termination 
#define NOD_PACKET_LENGTH 64
#define NOD_BUFFER_SIZE 100
#define QUINTIC_NUMBER_OF

#define QUINTIC_MAX_NUMBER_OF_NODS 3

#define TASK_QUINTIC_STACK_SIZE                (1024/sizeof(portSTACK_TYPE))
#define TASK_QUINTIC_STACK_PRIORITY            (tskIDLE_PRIORITY + 1)
#define CMD_RESPONSE_BUF_SIZE						  255

//Commands
#define QCMD_QN_ACK "QnAck\r\n"
#define QCMD_ACK "Ack\r\n"
#define QCMD_BEGIN "begin\r\n"
#define QCMD_SCAN "scan\r\n"
#define QCMD_SCAN_ACK "QnScanAck\r\n"
#define QCMD_CONN_ACK "QnConAck\r\n"
#define QCMD_CONNECT "connect\r\n"
#define QCMD_START "start\r\n"
#define QCMD_STOP "stop\r\n"



typedef struct
{
	int nodId; 
	char macAddress[NOD_MAC_ADDRESS_LENGTH]; //stored as ASCII
	char packetBuffer[NOD_BUFFER_SIZE][NOD_PACKET_LENGTH]; 
	int bufferHead;
	int bufferEnd; 
	int nodValid; //indicates whether current nod should be used. 
	xSemaphoreHandle semaphor; //the semaphore for the specific nod. 	 		
}nodConfiguration_t;

typedef struct  
{
	nodConfiguration_t* nodArray[QUINTIC_MAX_NUMBER_OF_NODS]; //array that stores the configuration/data for each NOD
	drv_uart_config_t* uartDevice; //pointer to uart driver that's used by the interface, must already be initialized
	int expectedNumberOfNods; 	
	int isinit; 
}quinticConfiguration_t;

//function declarations
void task_quinticHandler(void *pvParameters); 



#endif /* TASK_QUINTIC_INTERFACE_H_ */