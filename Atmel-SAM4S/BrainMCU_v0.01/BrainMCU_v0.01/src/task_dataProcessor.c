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
#include "drv_uart.h"
//extern definitions
extern imuConfiguration_t imuConfig[];
extern drv_uart_config_t uart1Config;
extern uint32_t sgSysTickCount;
xQueueHandle queue_dataHandler = NULL;
static FIL log_file_object; 
static char dataLogFileName[] = "0:dataLog_tst.csv";

////////////////////////////////////////////////////////
//move to own file
typedef struct
{
	char* dataPointer;
	uint16_t length;
}serialDataPacket_t;

xSemaphoreHandle semaphore_sdCardWrite = NULL;
#define SD_CARD_BUFFER_SIZE 512
volatile char sdCardBuffer[SD_CARD_BUFFER_SIZE] = {0};
volatile char tempBuf[SD_CARD_BUFFER_SIZE] = {0};	
volatile uint32_t sdCardBufferPointer = 0; 	
void task_sdCardWrite(void *pvParameters)
{
	
	uint32_t numBytesToWrite = 0, numBytesWritten = 0;
	uint32_t numBytes = 0;  
	static FRESULT res = FR_OK;	
	semaphore_sdCardWrite = xSemaphoreCreateMutex(); 
	while(1)
	{		
		//take semaphore and copy data to a temporary buffer. 
		if(xSemaphoreTake(semaphore_sdCardWrite,10) == true)
		{			
			if(sdCardBufferPointer > 0 && sdCardBufferPointer <= SD_CARD_BUFFER_SIZE)
			{
				memcpy(tempBuf,sdCardBuffer,sdCardBufferPointer); 	
				numBytesToWrite = sdCardBufferPointer; 
				sdCardBufferPointer = 0;
			}			
			xSemaphoreGive(semaphore_sdCardWrite);
		}	
		if(numBytesToWrite > 0)
		{
			numBytesWritten = 0;
			while(numBytesToWrite > 0)
			{					
				res = f_write(&log_file_object, tempBuf+numBytesWritten, numBytesToWrite, &numBytes);
				if(res != FR_OK)
				{
					printf("file write failed with code %d\r\n", res);
				}
				numBytesToWrite -= numBytes;
				numBytesWritten += numBytes; 	
				vTaskDelay(1); 	

			}
			res = f_sync(&log_file_object); //sync the file
			if(res != FR_OK)
			{
				printf("file sync failed with code %d\r\n", res);
			}
			vTaskDelay(1);
		}
		vTaskDelay(10); 
	}	
}

xQueueHandle queue_serialData = NULL;
void task_SerialTransmit(void *pvParameters)
{
	serialDataPacket_t dataPacket; 
	queue_serialData = xQueueCreate( 100, sizeof(serialDataPacket_t));
	if(queue_serialData == 0)
	{
		// Queue was not created this is an error!
		printf("an error has occurred, data handler queue failure\r\n");
		return;
	}
	while(1)
	{
		if(xQueueReceive( queue_serialData, &( dataPacket ), 1000) == TRUE)
		{
			if(dataPacket.dataPointer != NULL)
			{					
				drv_uart_putString(&uart1Config, dataPacket.dataPointer); 							
				free(dataPacket.dataPointer);	
			}
		}
	}	
}
///////////////////////////////////////////////////////
#define NUMBER_OF_SENSORS 10
xTimerHandle timer_dataTransmitter = 0;  
xSemaphoreHandle semaphore_latestData = NULL; 

dataPacket_t packetBuffer[NUMBER_OF_SENSORS]; //store 10 packets, one for each sensor, when all loaded process it. 
uint16_t packetReceivedFlags = 0x0000;  //a flag for each slot, when all are 1 then process it. 
uint16_t packetReceivedMask = 0x000;	//0x01FF; //this mask of which flags have to be set to save the files to disk. 
volatile uint32_t totalBytesWritten = 0; //the total bytes written to the file
volatile uint32_t totalFramesWritten = 0; //the total bytes written to the file
//static function declarations

static status_t processPackets(); 
void timer_sendDataCallback(xTimerHandle pxTimer);

void task_dataHandler(void *pvParameters)
{
	static FRESULT res = FR_OK;
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
	int timerId = 0;

	int retCode = xTaskCreate(task_sdCardWrite, "SD", TASK_QUINTIC_STACK_SIZE, NULL, TASK_QUINTIC_STACK_PRIORITY - 1, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to sd card task code %d\r\n", retCode);
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
				ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
				if(packetReceivedMask != 0x0000)
				{
					processPackets();
				}
				ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
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
static status_t processPackets()
{
	status_t status = STATUS_PASS; 
	int packetIndex = 0, i, j, k, sensorIndex;	  
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
						entryBuffer[entryBufferPtr++] = packetBuffer[i].data[(packetIndex*12)+(j*4)+k];	
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
						entryBuffer[entryBufferPtr++] = packetBuffer[i].data[(packetIndex*30)+(j*4)+k];							
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

	totalBytesWritten += numberBytes;		
	serialDataPacket_t serialData; 
	serialData.dataPointer = entryBuffer; 
	serialData.length = entryBufferPtr; 		
	drv_uart_putString(&uart1Config, entryBuffer);	
	totalFramesWritten++;	
	if(xSemaphoreTake(semaphore_sdCardWrite,1) == true)
	{
		//copy data to sdCard buffer; 
		if(sdCardBufferPointer + entryBufferPtr < SD_CARD_BUFFER_SIZE)
		{		
			memcpy(sdCardBuffer+sdCardBufferPointer,entryBuffer,entryBufferPtr); 
			sdCardBufferPointer += entryBufferPtr;
		}
		xSemaphoreGive(semaphore_sdCardWrite);
	}
	entryBufferPtr = 0; //reset pointer.		
	//clear flag at the end 
	packetReceivedFlags = 0x0000; 		
	return status; 
}
void timer_sendDataCallback(xTimerHandle pxTimer)
{
	if(xSemaphoreTake(semaphore_latestData,10) == true)
	{
		processPackets();
		xSemaphoreGive(semaphore_latestData); 
	}	
}

