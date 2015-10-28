/*
 * Config_Settings.c
 *
 * Created: 9/16/2015 5:21:04 PM
 *  Author: Hriday Mehta
 */ 
#include "Config_Settings.h"
#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "conf_board.h"
#include "BrainMCU.h"
#include "common.h"
#include "DebugLog.h"
#include "task_quinticInterface.h"

//global variable of settings structure
brainSettings_t brainSettings = {.isLoaded = 0}; 
extern quinticConfiguration_t quinticConfig[]; 	
extern imuConfiguration_t imuConfig[]; 
/*	SD Card FAT-FS variables	*/
static char file_name[] = "0:Heddoko.txt";
static FIL file_object;
//static function declarations

//file parsing helper functions
status_t getLineFromBuf(char* bufPtr, char* result, size_t resultSize);

/**
 * ReadConfigSD(void)
 * @brief Read configuration settings from SD card
 */
extern int ReadConfigSD	(void)
{	
	UINT byte_to_read, byte_read;
	uint8_t result = SUCCESS;
	static FRESULT res;
	printf("Opening SD Card to read\r\n");
	
	file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
	res = f_open(&file_object, (char const *)file_name, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK)
	{
		result = CANNOT_OPEN;
		printf("Error: Cannot Open file\r\n");
		return result;
	}	
	printf("Reading from SD\r\n");
	
	memset(data_buffer, 0, DATA_SIZE);
	byte_to_read = file_object.fsize;

	for (int i = 0; i < byte_to_read; i += DATA_SIZE)
	{
		res = f_read(&file_object, data_buffer, DATA_SIZE, &byte_read);
		if (res != FR_OK)
		{
			result = READ_FAILED;
			printf("Error: Cannot Open file\r\n");
			return result;
		}
	}
	
	printf("Closing the file\r\n");
	res = f_close(&file_object);
	if (res != FR_OK)
	{
		result = CANNOT_CLOSE;
		printf("Error: Cannot Open file\r\n");
		return result;
	}	
}

/**
 * loadSettings(char* filename)
 * @brief Load configuration settings to buffers
 */
status_t loadSettings(char* filename)
{
	
	uint8_t result = SUCCESS;
	static FIL configFileObj;
	//printf("Opening SD Card to read\r\n");
	DebugLogBufPrint("Opening SD Card to read\r\n");
	
	filename[0] = LUN_ID_SD_MMC_0_MEM + '0'; //is this necessary? 
	FRESULT res = f_open(&configFileObj, (char const *)filename, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK)
	{
		result = CANNOT_OPEN;
		//printf("Error: Cannot Open file\r\n");
		DebugLogBufPrint("Error: Cannot Open file\r\n");
		return STATUS_FAIL;
	}
	//read the whole file into a buffer
	//printf("Reading from SD\r\n");
	DebugLogBufPrint("Reading from SD\r\n");
	//char buf[MAX_CONFIG_FILE_SIZE] = {0}; 	 
	UINT bytes_read = 0, total_bytes_read = 0;	
	while(total_bytes_read < configFileObj.fsize && res == FR_OK)
	{
		res = f_read(&configFileObj, data_buffer+total_bytes_read, MAX_CONFIG_FILE_SIZE - total_bytes_read, &bytes_read);
		total_bytes_read += bytes_read; 
	}
	
	//now parse the file and 
	status_t step_status = STATUS_PASS;
	char* bufPtr = data_buffer; //set pointer to start of buffer
	//char line[50] = {0}; 
	int NumberOfNods = 0;	
	if(getLineFromBuf(bufPtr, line, sizeof(line)) == PASS)
	{
		if(sscanf(line, "%d,\r\n",&NumberOfNods) < 1)
		{
			printf("failed to read settings\r\n");
			DebugLogBufPrint("failed to read settings\r\n");
			return STATUS_FAIL; 
		}
		bufPtr += strlen(line); 		
	}
	brainSettings.numberOfImus = NumberOfNods; 
	//initialize the expectedNumberOfNods
	quinticConfig[0].expectedNumberOfNods = 0;
	quinticConfig[1].expectedNumberOfNods = 0;
	quinticConfig[2].expectedNumberOfNods = 0;	
	int i = 0; 
	int quinticIndex = 0; 
	int imuId = 0;
	char tempMACAddress[20] = {0}; 
	for(i = 0; i < NumberOfNods; i++)
	{
		if(getLineFromBuf(bufPtr, line,sizeof(line)) == STATUS_PASS)
		{			
			if(sscanf(line,"%d,%d,%s\r\n",&quinticIndex, &imuId, tempMACAddress) < 2)
			{
				printf("failed to parse IMU settings\r\n"); 
				DebugLogBufPrint("failed to parse IMU settings\r\n");
				break;
			}
			else
			{
				if(imuId < 0 || imuId > 9)
				{
					printf("received incorrect imuId%d\r\n",imuId); 
					break;	
				}
				imuConfig[imuId].imuId = imuId; 
				snprintf(imuConfig[imuId].macAddress,20, "%s\r\n",tempMACAddress); 
				imuConfig[imuId].imuValid = true;
				if(quinticIndex < 0 || quinticIndex > 2)
				{
					printf("failed to assign IMU%d to quintic %d",i,quinticIndex); 
					break;
				}
				if(quinticConfig[quinticIndex].expectedNumberOfNods >= MAX_NUMBER_OF_IMUS)
				{
					printf("failed to assign IMU%d to quintic %d: too many IMUs",imuId,quinticIndex);
					break;
				}
				quinticConfig[quinticIndex].imuArray[quinticConfig[quinticIndex].expectedNumberOfNods++] = &imuConfig[imuId]; 
			}
			printf("loaded settings for IMU %d on Q%d, %s",imuConfig[imuId].imuId,quinticIndex, imuConfig[imuId].macAddress);
			bufPtr += strlen(line); 
		}
		else
		{
			break; 
		}		
	}	
	printf("Closing the file\r\n");
	DebugLogBufPrint("Closing the file\r\n");
	res = f_close(&configFileObj);
	if (res != FR_OK)
	{		
		printf("Error: Cannot Open file\r\n");
		DebugLogBufPrint("Error: Cannot Open file\r\n");
		return STATUS_FAIL;
	}
	brainSettings.isLoaded = 1; 	
	return STATUS_PASS; 	
	
}


//Static functions

/**
 * getLineFromBuf(char* bufPtr, char* resp, size_t respSize)
 * @brief Get one line from the buffer
 */
status_t getLineFromBuf(char* bufPtr, char* resp, size_t respSize)
{
	status_t result = STATUS_FAIL; 
	int bufLen = strlen(bufPtr); 
	int i = 0;
	if(bufLen == 0)
	{
		return STATUS_FAIL; 
	}
	for(i = 0; i < bufLen && i<respSize; i++)
	{
		resp[i] = bufPtr[i]; 
		if(resp[i] == '\n')
		{
			result = STATUS_PASS;
			resp[i+1] = 0; //terminate the string! 
			break; 			
		}		
	}
	if(result == STATUS_PASS)
	{
		bufPtr += i+1; 
	}
	return result; 
}