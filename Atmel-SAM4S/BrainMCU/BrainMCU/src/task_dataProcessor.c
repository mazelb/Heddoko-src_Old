/*
 * task_dataProcessor.c
 *
 * Created: 9/28/2015 11:18:25 AM
 *  Author: Sean Cloghesy
 */ 
#include <asf.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "task_dataProcessor.h"
#include "task_quinticInterface.h"
#include "task_sdCardWrite.h"
#include "drv_uart.h"
//extern definitions
#define NUMBER_OF_SENSORS 10

extern imuConfiguration_t imuConfig[];
extern drv_uart_config_t uart0Config;
extern uint32_t sgSysTickCount;
xQueueHandle queue_dataHandler = NULL;


dataPacket_t packetBuffer[NUMBER_OF_SENSORS]; //store 10 packets, one for each sensor, when all loaded process it.
uint16_t packetReceivedFlags = 0x0000;  //a flag for each slot, when all are 1 then process it.
uint16_t packetReceivedMask = 0x000;	//0x01FF; //this mask of which flags have to be set to save the files to disk.
volatile uint32_t totalFramesWritten = 0; //the total bytes written to the file

#define MAX_BUFFERED_DATA_FRAMES 3

//dataFrame_t dataFrames[MAX_BUFFERED_DATA_FRAMES]; 
//uint32_t dataFrameHead = 0;
//uint32_t dataFrameTail = 0; 

extern xSemaphoreHandle semaphore_sdCardWrite;

//static function declarations

static status_t processPackets(); 

/***********************************************************************************************
 * task_dataHandler(void *pvParameters)
 * @brief Handles the incoming data from IMUs
 * @param void *pvParameters
 * @return 
 ***********************************************************************************************/
void task_dataHandler(void *pvParameters)
{

	//setup the queue
	queue_dataHandler = xQueueCreate( 50, sizeof(dataPacket_t));
	if(queue_dataHandler == 0)
	{
		// Queue was not created this is an error!
		printf("an error has occurred, data handler queue failure\r\n"); 
		return; 
	}
	int timerId = 0;


	
	//open file to read. 
	dataPacket_t packet; 
	int i =0; 
	//int index = dataFrameTail; 
	while(1)
	{
		
		if(xQueueReceive( queue_dataHandler, &( packet ), 1000) == TRUE)
		{			
			//handle packet
			if(packet.type == DATA_PACKET_TYPE_IMU)
			{
				//start at the end of the packets. 
				//index = dataFrameTail; 
				//for(i = 0; i<MAX_BUFFERED_DATA_FRAMES; i++)
				//{
					////check to see if this IMU has checked in. 
					//if((dataFrames[index].flag & (uint16_t)(1 << packet.imuId)) == 0)
					//{
						////This IMU does not yet have data, copy it over
						//memcpy(dataFrames[index].imuData[packet.imuId], packet.data, 12); 
						//dataFrames[index].flag |= (uint16_t)(1 << packet.imuId); 						
						////have all sensors checked in for this frame, send it out. 
						//if(dataFrames[index].flag == packetReceivedMask)
						//{
							////process the packet. 
							//
							////clear the flags
							//dataFrames[index].flag = 0x0000; 
							////move up the tail. 
							//dataFrameTail++; 
							//if(dataFrameTail > MAX_BUFFERED_DATA_FRAMES)
							//{
								//dataFrameTail = 0;
							//}
						//}
						////if we're saving in the head of the buffer
						//if(index == dataFrameHead)
						//{
							//dataFrameHead++; 
							//if(dataFrameHead > MAX_BUFFERED_DATA_FRAMES)
							//{
								//dataFrameHead = 0;
							//}
						//}
						////break the loop, we've stored our data
						//break;
					//}
					//index++ ;
					//if(index > MAX_BUFFERED_DATA_FRAMES)
					//{
						//index = 0; 
					//}
															//
				//}
				if(packetReceivedFlags & (uint16_t)(1 << packet.imuId))
				{
					//we've already received data for this sensor, copy over it...
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
		vTaskDelay(1);
	}
	
}

#define NUMBER_OF_PACKETS_PER_MESSAGE 10
#define IMU_PACKET_LENGTH 3  //in number of data sizes. 
#define IMU_PACKET_DATA_SIZE 4 
#define FS_PACKET_LENGTH 5  //There are 5 channels on the fabric sense
#define FS_PACKET_DATA_SIZE 4 //each data point is 4 characters
char entryBuffer[200] = {0};

/***********************************************************************************************
 * processPackets()
 * @brief process the incoming packets and create a full frame. 
 * @param 
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error 
 ***********************************************************************************************/	
static status_t processPackets()
{
	status_t status = STATUS_PASS; 
	int i, j, k;	  
	int entryBufferPtr = 0; 
	int res = 0; 
	int numberBytes = 0; 
	//char* entryBuffer = NULL; 	
	
	entryBufferPtr = snprintf(entryBuffer, 12, "%010d,", sgSysTickCount);
	for(i = 0; i < 10; i++) //sensor reading
	{
		//if IMU, process the data this way
		if(packetBuffer[i].type == DATA_PACKET_TYPE_IMU)
		{			
			for(j=0; j < IMU_PACKET_LENGTH; j++) //reading value
			{
				for(k = 0; k < IMU_PACKET_DATA_SIZE; k++) //reading bytes
				{				
					//copy the asci data to the entry buffer
					if(packetBuffer[i].data != NULL)
					{
						entryBuffer[entryBufferPtr++] = packetBuffer[i].data[(j*4)+k];	
					}
					else
					{
						entryBuffer[entryBufferPtr++] = 0;
					}
						
					if(entryBuffer[entryBufferPtr-1] == 0)
					{
						//if that entry is empty/NULL, fill it in with '0's
						entryBuffer[entryBufferPtr-1] = '0'; 
					}
				}
				if(j != IMU_PACKET_LENGTH-1)
				{
					entryBuffer[entryBufferPtr++] = ';';	
				}			
			}
		}
		else //if stretch sense data, add it differently.
		{
			for(j=0; j < FS_PACKET_LENGTH; j++) //reading value
			{
				for(k = 0; k < FS_PACKET_DATA_SIZE; k++) //reading bytes
				{
					if(packetBuffer[i].data != NULL)
					{
						//copy the asci data to the entry buffer
						entryBuffer[entryBufferPtr++] = packetBuffer[i].data[(j*4)+k];							
					}
					else
					{
						//there is no data, set to zero. 
						entryBuffer[entryBufferPtr++] = 0;
					}
					if(entryBuffer[entryBufferPtr-1] == 0)
					{
						//if that entry is empty/NULL, fill it in with '0's
						entryBuffer[entryBufferPtr-1] = '0';
					}
				}
				if(j != FS_PACKET_LENGTH-1)
				{
					entryBuffer[entryBufferPtr++] = ';';
				}
			}				
		}			 			
		entryBuffer[entryBufferPtr++] = ',';		
	}	
	entryBuffer[entryBufferPtr++] = '\r';
	entryBuffer[entryBufferPtr++] = '\n';
	entryBuffer[entryBufferPtr] = 0; //terminate the string
		
	drv_uart_putString(&uart0Config, entryBuffer);	
	totalFramesWritten++;	
	//write the entry to file
	task_sdCardWriteEntry(entryBuffer,entryBufferPtr);
	entryBufferPtr = 0; //reset pointer.		
	//clear flag at the end 
	packetReceivedFlags = 0x0000; 		
	return status; 
}


