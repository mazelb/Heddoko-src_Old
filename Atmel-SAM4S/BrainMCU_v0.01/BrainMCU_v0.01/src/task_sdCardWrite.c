/*
 * task_sdCardWrite.c
 *
 * Created: 10/19/2015 8:20:39 AM
 *  Author: Sean Cloghesy
 */ 
#include <asf.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task_sdCardWrite.h"
#include "BrainMCU.h"


xSemaphoreHandle semaphore_sdCardWrite = NULL;
volatile char sdCardBuffer[SD_CARD_BUFFER_SIZE] = {0};
volatile uint32_t sdCardBufferPointer = 0;
volatile uint32_t totalBytesWritten = 0; //the total bytes written to the file
volatile char tempBuf[SD_CARD_BUFFER_SIZE] = {0};
FIL dataLogFile_obj;
volatile Bool dataLogFileOpen = false; 
uint8_t closeLogFileFlag = 0; 
static char dataLogFileName[SD_CARD_FILENAME_LENGTH] = {0};
void task_sdCardHandler(void *pvParameters)
{
	
	uint32_t numBytesToWrite = 0, numBytesWritten = 0;
	uint32_t numBytes = 0;
	semaphore_sdCardWrite = xSemaphoreCreateMutex();
	static FRESULT res = FR_OK;
	dataLogFileName[0] = LUN_ID_SD_MMC_0_MEM + '0';
	//res = f_open(&dataLogFile_obj, (char const *)dataLogFileName, FA_OPEN_ALWAYS | FA_WRITE);
	//
	//if (res == FR_OK)
	//{
		//printf("log open\r\n");
	//}
	//else
	//{
		//printf("log failed to open\r\n");
	//}
	//
	//res = f_lseek(&dataLogFile_obj, dataLogFile_obj.fsize);	
	
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
		//if the close file flag is set, close the file. 
		if(closeLogFileFlag == 1)
		{
			f_close(&dataLogFile_obj); 
			dataLogFileOpen = false; 
			//clear the flag. 
			closeLogFileFlag = 0;
		}
		//if the data file is open, then write to log
		if(dataLogFileOpen)
		{	
			if(numBytesToWrite > 0)
			{
				numBytesWritten = 0;			
				while(numBytesToWrite > 0)
				{
					if(dataLogFileOpen)
					{				
						res = f_write(&dataLogFile_obj, tempBuf+numBytesWritten, numBytesToWrite, &numBytes);
					}
				
					numBytesToWrite -= numBytes;
					numBytesWritten += numBytes;
					totalBytesWritten += numBytes;
					vTaskDelay(1);

				}
				res = f_sync(&dataLogFile_obj); //sync the file
				if(res != FR_OK)
				{
					printf("file sync failed with code %d\r\n", res);
				}
				vTaskDelay(1);
			}
		}
		vTaskDelay(10);
	}
}
status_t task_sdCardWriteEntry(char* entry, size_t length)
{
	status_t status = STATUS_PASS; 
	//take the semaphore
	if(xSemaphoreTake(semaphore_sdCardWrite,5) == true)
	{
		//copy data to sdCard buffer, make sure we have room first
		if(sdCardBufferPointer + length < SD_CARD_BUFFER_SIZE)
		{
			memcpy(sdCardBuffer+sdCardBufferPointer,entry,length);
			sdCardBufferPointer += length;
		}
		else
		{
			status = STATUS_FAIL; 
		}
		xSemaphoreGive(semaphore_sdCardWrite);
	}
	else
	{
		status = STATUS_FAIL; 
	}	
	
	return status; 
}
status_t task_sdCard_OpenNewFile()
{
	uint8_t data_buffer[100];
	char fileIndexLog[] = "0:logIndex.dat"; 
	char logFileName[SD_CARD_FILENAME_LENGTH] = {0}; 
	uint32_t byte_to_read, byte_read, bytes_written;
	uint8_t result = SUCCESS;
	FRESULT res;
	uint32_t fileIndexNumber = 0; 
	FIL indexFile_obj;
	//if the log file is open, then return an error
	if(dataLogFileOpen == true)
	{
		return STATUS_FAIL; 
	}
	
	//get the file index for the newly created file
	//open the file that contains the index numbers
	res = f_open(&indexFile_obj, (char const *)fileIndexLog, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	if (res != FR_OK)
	{		
		return STATUS_FAIL;
	}
	//if the filesize is 0, it means it's never been created, set index to 1. 	
	if(indexFile_obj.fsize == 0)
	{
		fileIndexNumber = 1; 
	}
	else
	{
		res = f_read(&indexFile_obj, data_buffer, 100, &byte_read);
		if(res != FR_OK)
		{
			return STATUS_FAIL; 
		}
		sscanf(data_buffer,"%d\r\n",&fileIndexNumber); 	
		fileIndexNumber++; 		
	}
	//write the update index back to the file. 
	sprintf(data_buffer, "%05d\r\n", fileIndexNumber); 
	f_lseek(&indexFile_obj,0); 
	res = f_write(&indexFile_obj,data_buffer,strlen(data_buffer),&bytes_written); 
	if(res != FR_OK)
	{
		return STATUS_FAIL; 
	}
	else
	{
		f_close(&indexFile_obj);
	}
	
	//create the filename
	snprintf(dataLogFileName, SD_CARD_FILENAME_LENGTH, "0:MovementLog%05d.csv",fileIndexNumber); 	
	if(xSemaphoreTake(semaphore_sdCardWrite,100) == true)
	{
		res = f_open(&dataLogFile_obj, (char const *)dataLogFileName, FA_OPEN_ALWAYS | FA_WRITE);		
		if (res == FR_OK)
		{
			printf("log open\r\n");
		}
		else
		{
			printf("log failed to open\r\n");
		}		
		res = f_lseek(&dataLogFile_obj, dataLogFile_obj.fsize);
		dataLogFileOpen = true; 
		xSemaphoreGive(semaphore_sdCardWrite);	
	}
	
}
status_t task_sdCard_CloseFile()
{
	//if the filelog is not open, return and error
	if(dataLogFileOpen == false)
	{
		return STATUS_FAIL;
	}		
	if(xSemaphoreTake(semaphore_sdCardWrite,100) == true)
	{
		//set the flag to have the main sd card thread close the file. 
		//we don't want to close the file in the middle of a write. 
		closeLogFileFlag = 1; 
		xSemaphoreGive(semaphore_sdCardWrite);
	}	
}

