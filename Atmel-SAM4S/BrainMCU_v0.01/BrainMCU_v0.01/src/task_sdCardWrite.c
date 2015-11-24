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
#include "task_commandProc.h"
#include "settings.h"
#include "drv_uart.h"

extern brainSettings_t brainSettings; 
extern drv_uart_config_t uart0Config;

xSemaphoreHandle semaphore_sdCardWrite = NULL;
volatile char sdCardBuffer[SD_CARD_BUFFER_SIZE] = {0}, debugLogBuffer[SD_CARD_BUFFER_SIZE] = {0};
volatile uint32_t sdCardBufferPointer = 0, debugLogBufferPointer = 0;
volatile uint32_t totalBytesWritten = 0, debugLogTotalBytesWritten = 0; //the total bytes written to the file
volatile char tempBuf[SD_CARD_BUFFER_SIZE] = {0}, debugLogTempBuf[SD_CARD_BUFFER_SIZE] = {0};
FIL dataLogFile_obj, debugLogFile_Obj;
volatile Bool dataLogFileOpen = false, debugLogFileOpen = false;  
uint8_t closeLogFileFlag = 0, closeDebugLogFileFlag = 0; 
static char dataLogFileName[SD_CARD_FILENAME_LENGTH] = {0};
	
void task_sdCardHandler(void *pvParameters)
{
	
	uint32_t numBytesToWrite = 0, numBytesWritten = 0;
	uint32_t debugNumBytesToWrite = 0, debugNumBytesWritten = 0;
	uint32_t numBytes = 0, debugNumBytes = 0;
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
			
			if (debugLogBufferPointer > 0 && debugLogBufferPointer <= SD_CARD_BUFFER_SIZE)
			{
				memcpy(debugLogTempBuf, debugLogBuffer, debugLogBufferPointer);
				debugNumBytesToWrite = debugLogBufferPointer;
				debugLogBufferPointer = 0;
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
		if (closeDebugLogFileFlag == 1)
		{
			f_close(&debugLogFile_Obj);
			debugLogFileOpen = false;
			//clear the flag
			closeDebugLogFileFlag = 0;
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
						res = f_write(&dataLogFile_obj,  (void*)(tempBuf+numBytesWritten), numBytesToWrite, &numBytes);
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
		if(debugLogFileOpen)
		{
			if(debugNumBytesToWrite > 0)
			{
				debugNumBytesWritten = 0;
				while(debugNumBytesToWrite > 0)
				{
					if(debugLogFileOpen)
					{
						res = f_write(&debugLogFile_Obj,  (void*)(debugLogTempBuf+debugNumBytesWritten), debugNumBytesToWrite, &debugNumBytes);
					}
					
					debugNumBytesToWrite -= debugNumBytes;
					debugNumBytesWritten += debugNumBytes;
					debugLogTotalBytesWritten += debugNumBytes;
					vTaskDelay(1);

				}
				res = f_sync(&debugLogFile_Obj); //sync the file
				if(res != FR_OK)
				{
					printf("file sync failed with code %d\r\n", res);
				}
				vTaskDelay(1);
			}
		}
		vTaskDelay(100);
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
			debugPrintString("Write failed: Buffer full\r\n");
		}
		xSemaphoreGive(semaphore_sdCardWrite);
	}
	else
	{
		status = STATUS_FAIL; 
	}	
	
	return status; 
}

status_t task_debugLogWriteEntry(char* entry, size_t length)
{
	status_t status = STATUS_PASS;
	//take the semaphore
	if(xSemaphoreTake(semaphore_sdCardWrite,5) == true)
	{
		//copy data to sdCard buffer, make sure we have room first
		if(debugLogBufferPointer + length < SD_CARD_BUFFER_SIZE)
		{
			memcpy(debugLogBuffer+debugLogBufferPointer,entry,length);
			debugLogBufferPointer += length;
		}
		else
		{
			status = STATUS_FAIL;
			debugPrintString("Write failed: Debug Log Buffer full\r\n");
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
		res = f_read(&indexFile_obj, (void*)data_buffer, 100, &byte_read);
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
	res = f_write(&indexFile_obj, (void*)data_buffer,strlen(data_buffer),&bytes_written); 
	if(res != FR_OK)
	{
		return STATUS_FAIL; 
	}
	else
	{
		f_close(&indexFile_obj);
	}
	
	//create the filename
	snprintf(dataLogFileName, SD_CARD_FILENAME_LENGTH, "0:%s_MovementLog%05d.csv",brainSettings.suitNumber,fileIndexNumber); 	
	if(xSemaphoreTake(semaphore_sdCardWrite,100) == true)
	{
		res = f_open(&dataLogFile_obj, (char const *)dataLogFileName, FA_OPEN_ALWAYS | FA_WRITE);		
		if (res == FR_OK)
		{
			debugPrintString("log open\r\n");
		}
		else
		{
			debugPrintString("log failed to open\r\n");
		}		
		res = f_lseek(&dataLogFile_obj, dataLogFile_obj.fsize);
		dataLogFileOpen = true; 
		xSemaphoreGive(semaphore_sdCardWrite);	
	}
	return STATUS_PASS;	//it only reaches here if everything is good.
}

status_t task_debugLog_OpenFile()
{
	char debugLogFileName[] = {0}, vLogFileName[] = "0:DebugLog.dat";
	char debugLogNewFileName[] = "0:DebugLog.txt", debugLogOldFileName[] = "0:DebugLog_old.txt";
	uint8_t vDebugLogFileIndex = 0;
	FRESULT res;
	FILINFO vDebugLogFileInfo;
	bool vSeekFile = false;
	
	//if the log file is open, then return an error
	if(debugLogFileOpen == true)
	{
		return STATUS_FAIL;
	}
	
	if(xSemaphoreTake(semaphore_sdCardWrite,100) == true)
	{
		res = f_stat(debugLogNewFileName, &vDebugLogFileInfo);
		if (res == FR_OK)
		{
			//file exist, do a check on size
			if (vDebugLogFileInfo.fsize >= DEBUG_LOG_MAX_FILE_SIZE)
			{
				//the file size exceeds the limit, change the log file
				//check if the old file exits
				res = f_stat(debugLogOldFileName, &vDebugLogFileInfo);
				if (res == FR_OK)
				{
					//delete the old file before renaming the current file
					f_unlink(debugLogOldFileName);
				}
				//rename the current file to the name of old file
				res = f_rename(&debugLogNewFileName[2], &debugLogOldFileName[2]);
				if (res != FR_OK)
				{
					debugPrintString("Rename of Debug Log file failed\r\n");
				}
			}
		}
		else if (res == FR_NO_FILE)
		{
			debugPrintString("File does not exist, creating new file\r\n");
		}
		else
		{
			debugPrintString("DebugLog file open failed\r\n");
		}
		//Open the Log file DebugLog.txt
		res = f_open(&debugLogFile_Obj, (char const *)debugLogNewFileName, FA_OPEN_ALWAYS | FA_WRITE);
		if (res == FR_OK)
		{
			debugPrintString("DebugLog open\r\n");
		}
		else
		{
			debugPrintString("DebugLog failed to open\r\n");
		}
		res = f_lseek(&debugLogFile_Obj, debugLogFile_Obj.fsize);
		debugLogFileOpen = true;
		xSemaphoreGive(semaphore_sdCardWrite);
	}
	return STATUS_PASS;	//it only reaches here if everything is good.
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

status_t task_debugLog_CloseFile()
{
	//if DebugLog file is not open, return error
	if (debugLogFileOpen == false)
	{
		return STATUS_FAIL;
	}
	if (xSemaphoreTake(semaphore_sdCardWrite, 100) == true)
	{
		//set the flag to have the main sd card thread close the file
		//we don't want to close the file in the middle of a write
		closeDebugLogFileFlag = 1;
		xSemaphoreGive(semaphore_sdCardWrite);
	}
}