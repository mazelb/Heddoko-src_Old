/*
 * task_dataProcessor.c
 *
 * Created: 9/28/2015 11:18:25 AM
 *  Author: Sean Cloghesy
 */ 
#include <asf.h>
#include <string.h>
#include "queue.h"
#include "task_dataProcessor.h"
#include "task_quinticInterface.h"
#include "drv_uart.h"
//extern definitions
extern imuConfiguration_t imuConfig[];
extern drv_uart_config_t uart1Config;

xQueueHandle queue_dataHandler = NULL;
static FIL log_file_object; 
static char dataLogFileName[] = "0:dataLog.csv";

#define NUMBER_OF_SENSORS 10

dataPacket_t packetBuffer[NUMBER_OF_SENSORS]; //store 10 packets, one for each sensor, when all loaded process it. 
uint16_t packetReceivedFlags = 0x0000;  //a flag for each slot, when all are 1 then process it. 
uint16_t packetReceivedMask = 0x000;	//0x01FF; //this mask of which flags have to be set to save the files to disk. 
volatile uint32_t totalBytesWritten = 0; //the total bytes written to the file
volatile uint32_t totalFramesWritten = 0; //the total bytes written to the file
//static function declarations
static status_t processPackets(); 
	 
void task_dataHandler(void *pvParameters)
{
	
	dataLogFileName[0] = LUN_ID_SD_MMC_0_MEM + '0';
	res = f_open(&log_file_object, (char const *)dataLogFileName, FA_OPEN_ALWAYS | FA_WRITE);
	if (res == FR_OK)
	{
		printf("log open\r\n");
	}
	else
	{
		printf("log failed to open\r\n");
	}
	
	res = f_lseek(&log_file_object, log_file_object.fsize);	
	//setup the queue
	queue_dataHandler = xQueueCreate( 100, sizeof(dataPacket_t));
	if(queue_dataHandler == 0)
	{
		// Queue was not created this is an error!
		printf("an error has occurred, data handler queue failure\r\n"); 
		return; 
	}
	//open file to read. 
	dataPacket_t packet; 
	while(1)
	{
		if(xQueueReceive( queue_dataHandler, &( packet ), 1000) == TRUE)
		{
			//handle packet
			if(packet.type == DATA_PACKET_TYPE_IMU)
			{
				if(packetReceivedFlags & (uint16_t)(1 << packet.imuId))
				{
					//we've already received data for this sensor, process all the bytes as is. 
					imuConfig[packet.imuIndex].stats.droppedPackets++; //we're dropping a packet. 	TODO must fix this case									
				}
				else
				{
					//make sure the index is alright. 
					if(packet.imuId < NUMBER_OF_SENSORS)
					{
						memcpy(&packetBuffer[packet.imuId],&packet, sizeof(dataPacket_t));
						packetReceivedFlags |= (1 << packet.imuId); //set flag
					}
				}	
			}
			else
			{
				if(packetReceivedFlags & (uint16_t)(1 << NUMBER_OF_SENSORS -1))
				{
					//we've already received data for this sensor, process all the bytes as is.					
				}
				else
				{
					//FabSense data always stored at end of buffer
					memcpy(&packetBuffer[NUMBER_OF_SENSORS -1],&packet, sizeof(dataPacket_t));
					packetReceivedFlags |= (1 << NUMBER_OF_SENSORS -1); //set flag
				}				
			}
			
			if(packetReceivedFlags == packetReceivedMask)
			{
				if(packetReceivedMask != 0x0000)
				{
					processPackets(); 	
				}				
			}	
		}
		//vTaskDelay(1); 
	}
	
}

#define NUMBER_OF_PACKETS_PER_MESSAGE 10
#define PACKET_LENGTH 3  //in number of data sizes. 
#define PACKET_DATA_SIZE 4 
static status_t processPackets()
{
	status_t status = STATUS_PASS; 
	int packetIndex = 0, i, j, k, sensorIndex;
	char entryBuffer[500] = {0};  
	int entryBufferPtr = 0; 
	int res = 0; 
	int numberBytes = 0; 
	for(packetIndex = 0; packetIndex < NUMBER_OF_PACKETS_PER_MESSAGE; packetIndex++)
	{	
		for(i = 0; i < 10; i++) //sensor reading
		{
			for(j=0; j < PACKET_LENGTH; j++) //reading value
			{
				for(k = 0; k < PACKET_DATA_SIZE; k++) //reading bytes
				{				
					//copy the asci data to the entry buffer
					entryBuffer[entryBufferPtr++] = packetBuffer[i].data[(packetIndex*12)+(j*4)+k];
					if(entryBuffer[entryBufferPtr-1] == 0)
					{
						//if that entry is empty/NULL, fill it in with '0's
						entryBuffer[entryBufferPtr-1] = '0'; 
					}
				}
				if(j != 2)
				{
					entryBuffer[entryBufferPtr++] = ';';	
				}			
			}
			entryBuffer[entryBufferPtr++] = ',';		
		}
		entryBuffer[entryBufferPtr++] = '\r';
		entryBuffer[entryBufferPtr++] = '\n';
		entryBuffer[entryBufferPtr++] = 0; //terminate the string
		
		//printf("%s", entryBuffer); 
		res = f_write(&log_file_object, entryBuffer, entryBufferPtr -2, &numberBytes);
		//drv_uart_putString(&uart1Config, entryBuffer);
		totalFramesWritten++;	
		totalBytesWritten += entryBufferPtr -2;
		entryBufferPtr = 0; //reset pointer. 
			
	}
	//clear flag at the end 
	packetReceivedFlags = 0x0000; 
	res = f_sync(&log_file_object); //sync the file
	
	return status; 
}

