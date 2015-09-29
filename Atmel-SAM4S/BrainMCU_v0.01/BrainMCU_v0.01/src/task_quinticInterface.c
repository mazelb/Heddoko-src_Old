/*
 * task_quinticInterface.c
 *
 * Created: 9/18/2015 10:49:35 AM
 *  Author: sean
 */
/**
 * @file  task_quinticInterface.c
 * @brief handler task and associated functions for management of the Quintic modules
 * @author Heddoko
 * @note 
 *
 * @copy Heddoko (2015)
 */ 
#include "task_quinticInterface.h"
#include "task_dataProcessor.h"
#include <string.h>

//#define DEBUG_DUMMY_DATA 

extern xQueueHandle queue_dataHandler;
//static declarations
#define DUMMY_PACKET "YY%d1PPP1RRR1YY%d2PPP2RRR2YY%d3PPP3RRR3YY%d4PPP4RRR4YY%d5PPP5RRR5YY%d6PPP6RRR6YY%d7PPP7RRR7YY%d8PPP8RRR8YY%d9PPP9RRR0YY%d0PPP0RRR0"
static status_t sendString(drv_uart_config_t* uartConfig, char* cmd);
static status_t getAck(drv_uart_config_t* uartConfig);
static status_t initializeImus(quinticConfiguration_t* qConfig); 
/***********************************************************************************************
 * task_quinticHandler(void *pvParameters)
 * @brief The main task for a quintic module, associate UART has to be initialized before calling this
 *	function. 
 * @param pvParameters, void pointer to structure containing quintic and imu configuration. 
 * @return void
 ***********************************************************************************************/
void task_quinticHandler(void *pvParameters)
{
	//cast void pointer to quintic config type. 
	quinticConfiguration_t *qConfig = (quinticConfiguration_t*)pvParameters; 
	//initialize all structures ,check everything is alright
	int i = 0;
	//initialize all buffers and pointers to zero
	for(i = 0 ; i < QUINTIC_MAX_NUMBER_OF_IMUS ; i++)
	{
		qConfig->imuArray[i]->bufferEnd = 0;
		qConfig->imuArray[i]->bufferHead = 0;
		memset(qConfig->imuArray[i]->packetBuffer, 0 , IMU_BUFFER_SIZE*IMU_PACKET_LENGTH); 	
	}
	if(drv_uart_isInit(qConfig->uartDevice) != STATUS_PASS)
	{
		//the UART driver has not been initialized, this should be done before starting this task!
		return; 
	}
	//cycle power on quintic module
	//Has specific IO for each quintic BLE_RST1 - BLE_RST3 TODO add this to configuration structure. 
	//Cycle power for all the NODs? This is only done when the start command is received. 
	//send all the initialization garbage
	#ifndef DEBUG_DUMMY_DATA
	initializeImus(qConfig);
	#endif 
	dataPacket_t packet; 
	//main loop of task, this is where we request information and store it. 
	char buf[CMD_RESPONSE_BUF_SIZE] = {0}; 
	while(1)
	{
		//for now just get a line and return one... just to see if things are working
		//if(drv_uart_getline(qConfig->uartDevice, buf, CMD_RESPONSE_BUF_SIZE) == STATUS_PASS)
		//{
			//sendString(qConfig->uartDevice, buf); 
		//}
		//
		
		#ifdef DEBUG_DUMMY_DATA
		int packetNumber = 0; 
		snprintf(&packet.data, 120+1, DUMMY_PACKET,qConfig->imuArray[0]->imuId,qConfig->imuArray[0]->imuId,qConfig->imuArray[0]->imuId,
		qConfig->imuArray[0]->imuId,qConfig->imuArray[0]->imuId,qConfig->imuArray[0]->imuId,qConfig->imuArray[0]->imuId,qConfig->imuArray[0]->imuId,
		qConfig->imuArray[0]->imuId,qConfig->imuArray[0]->imuId); 
		packet.imuId = qConfig->imuArray[0]->imuId; 
		packet.type = DATA_PACKET_TYPE_IMU;  
		if(queue_dataHandler != NULL)
		{		
			if(xQueueSendToBack( queue_dataHandler,( void * ) &packet,10 ) != TRUE)
			{
				//error failed to read the packet. 	
			}
		}

		vTaskDelay(10); //let the other processes do stuff	
		snprintf(&packet.data, 120+1, DUMMY_PACKET,qConfig->imuArray[1]->imuId,qConfig->imuArray[1]->imuId,qConfig->imuArray[1]->imuId,
		qConfig->imuArray[1]->imuId,qConfig->imuArray[1]->imuId,qConfig->imuArray[1]->imuId,qConfig->imuArray[1]->imuId,qConfig->imuArray[1]->imuId,
		qConfig->imuArray[1]->imuId,qConfig->imuArray[1]->imuId); 
		packet.imuId = qConfig->imuArray[1]->imuId; 
		packet.type = DATA_PACKET_TYPE_IMU;  
		if(queue_dataHandler != NULL)
		{		
			if(xQueueSendToBack( queue_dataHandler,( void * ) &packet,10 ) != TRUE)
			{
				//error failed to read the packet. 	
			}
		}
		vTaskDelay(10); //let the other processes do stuff
		snprintf(&packet.data, 120+1, DUMMY_PACKET,qConfig->imuArray[2]->imuId,qConfig->imuArray[2]->imuId,qConfig->imuArray[2]->imuId,
		qConfig->imuArray[2]->imuId,qConfig->imuArray[2]->imuId,qConfig->imuArray[2]->imuId,qConfig->imuArray[2]->imuId,qConfig->imuArray[2]->imuId,
		qConfig->imuArray[2]->imuId,qConfig->imuArray[2]->imuId); 
		packet.imuId = qConfig->imuArray[2]->imuId;
		packet.type = DATA_PACKET_TYPE_IMU;
		if(queue_dataHandler != NULL)
		{
			if(xQueueSendToBack( queue_dataHandler,( void * ) &packet,10 ) != TRUE)
			{
				//error failed to read the packet.
			}
		}
		#endif
		vTaskDelay(10); //let the other processes do stuff				
		
	}	
}



//static functions

static status_t sendString(drv_uart_config_t* uartConfig, char* cmd)
{
	int len = strlen(cmd); 
	int i = 0; 
	for(;i<len;)
	{
		if(drv_uart_putChar(uartConfig,cmd[i]) == STATUS_PASS) //returns pass only when char sent
		{
			i++;
		}
	}
}


static status_t getAck(drv_uart_config_t* uartConfig)
{
	status_t result = STATUS_PASS; 
	char buf[CMD_RESPONSE_BUF_SIZE] = {0}; //should move to static buffer for each quintic?
	result = drv_uart_getline(uartConfig, buf,CMD_RESPONSE_BUF_SIZE);
	if(result == STATUS_PASS)
	{
		if(strcmp(buf,QCMD_QN_ACK) != 0)
		{
			result = STATUS_FAIL;
		}
	}
	return result; 
}
static status_t getResponse(drv_uart_config_t* uartConfig, char* expectedResponse)
{
	status_t result = STATUS_PASS;
	char buf[CMD_RESPONSE_BUF_SIZE] = {0}; //should move to static buffer for each quintic?
	if(drv_uart_getline(uartConfig, buf,CMD_RESPONSE_BUF_SIZE) == STATUS_PASS)
	{
		if(strcmp(buf,expectedResponse) != 0)
		{
			result = STATUS_FAIL;
		}
	}
	return result;
}
static status_t initializeImus(quinticConfiguration_t* qConfig)
{
	status_t result = STATUS_PASS; 
	char vScanSuccess=0, vConSuccess=0, vScanLoopCount=0;
	//wait for first ACK	
	result = getAck(qConfig->uartDevice); 

	//get quintic ready to receive the 	
	sendString(qConfig->uartDevice,QCMD_BEGIN); 
	result |= getAck(qConfig->uartDevice); 

	//send MAC addresses for each NOD
	int i = 0;
	for(i=0;i<qConfig->expectedNumberOfNods; i++)
	{
		sendString(qConfig->uartDevice,qConfig->imuArray[i]->macAddress); 
		result |= getAck(qConfig->uartDevice);	
	}
	
	do
	{
		sendString(qConfig->uartDevice,QCMD_SCAN);
		if(getResponse(qConfig->uartDevice, QCMD_SCAN_ACK) == STATUS_PASS)
		{
			vScanSuccess=1;
			break;						
		}
		vScanLoopCount++; 
	}while(vScanLoopCount<=QN_MAX_CONN);
	
	if(vScanSuccess == 1)
	{
		sendString(qConfig->uartDevice,QCMD_CONNECT);
		if(getResponse(qConfig->uartDevice, QCMD_SCAN_ACK) == STATUS_PASS)
		{
			vConSuccess = 1; 	
		}

	}
	if(vScanSuccess == 1 && vScanSuccess == 1)
	{
		qConfig->isinit = true; 	
	}	
	//return the result; 
	return result; 

}