/*
 * task_fabricSense.c
 *
 * Created: 10/5/2015 9:19:27 AM
 *  Author: Sean Cloghesy
 */ 

#include "task_fabricSense.h"
#include "task_dataProcessor.h"
#include <string.h>
#define CREATE_DUMMY_PACKETS 
extern xQueueHandle queue_dataHandler;
extern uint32_t sgSysTickCount;
extern bool enableRecording; 
extern uint16_t packetReceivedMask; 
//static function foward declarations
status_t initializeFabSense(fabricSenseConfig_t* fabSenseConfig);
void createDummyFabSensePacket(char* dataPacket, size_t maxPacketSize, uint32_t seqNum);


/***********************************************************************************************
 * task_fabSenseHandler(void *pvParameters)
 * @brief The main task for a fabric sense module, associate UART has to be initialized before calling this
 *	function. 
 * @param pvParameters, void pointer to structure containing fabric sense configuration. 
 * @return void
 ***********************************************************************************************/
void task_fabSenseHandler(void *pvParameters)
{
	//cast void pointer to fabric sense config type.
	fabricSenseConfig_t *fsConfig = (fabricSenseConfig_t*)pvParameters;
	//check that uart device is initialized
	if(drv_uart_isInit(fsConfig->uartDevice) != STATUS_PASS)
	{
		//the UART driver has not been initialized, this should be done before starting this task!
		return;
	}
	//initialize the module, send configuration values. 
	if(initializeFabSense(fsConfig) != STATUS_PASS)
	{
		//this is an error, we should do something here!
		return; 
	} 
	dataPacket_t packet;
	packet.type = DATA_PACKET_TYPE_SS;
	//main loop of task, this is where we request information and store it.
	char buf[FS_RESPONSE_BUF_SIZE] = {0};
	uint8_t numPacketsReceived = 0; 
	uint32_t sequenceNumber = 0;
	uint32_t packetsReceived = 0; 
	packetReceivedMask |= 1<<9; //add mask for fabric sense
	while(1)
	{
		if(enableRecording)
		{		
			#ifdef CREATE_DUMMY_PACKETS
			vTaskDelay(20); 
			createDummyFabSensePacket(buf, FS_RESPONSE_BUF_SIZE, sequenceNumber++); 
			#endif
			//if this is our first packet in the set, assign memory for it. 
			memcpy(packet.data,buf,32); 
			//enqueue the packet for the data processor. 
			if(queue_dataHandler != NULL)
			{
				if(xQueueSendToBack( queue_dataHandler,( void * ) &packet,10 ) != TRUE)
				{
					//error failed to queue the packet.
					//if(packet.data != NULL)
					//{
						//free(packet.data);
						//packet.data = NULL;
					//}
					vTaskDelay(1);
				}
			}		
		}
		else
		{
			vTaskDelay(10); 
		}
		
	}
}

status_t task_fabSense_start(fabricSenseConfig_t* fabSenseConfig)
{
	status_t status = STATUS_PASS; 
	enableRecording = true; 
	return status; 
}

status_t task_fabSense_stop(fabricSenseConfig_t* fabSenseConfig)
{
	status_t status = STATUS_PASS;
	enableRecording = false; 
	return status;	
}


//static functions
status_t initializeFabSense(fabricSenseConfig_t* fabSenseConfig)
{
	return STATUS_PASS; 
}

void createDummyFabSensePacket(char* dataPacket, size_t maxPacketSize, uint32_t seqNum)
{
	int i = 0; 
	snprintf(dataPacket, maxPacketSize, "%04dBBBBCCCCDDDDEEEE%08d\r\n",seqNum%10000, sgSysTickCount); 

}