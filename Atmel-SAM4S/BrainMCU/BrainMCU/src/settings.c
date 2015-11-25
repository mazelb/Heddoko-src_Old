/*
 * Config_Settings.c
 *
 * Created: 9/16/2015 5:21:04 PM
 *  Author: Hriday Mehta
 */ 
#include "settings.h"
#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "conf_board.h"
#include "common.h"
#include "DebugLog.h"
#include "task_quinticInterface.h"
#include "limits.h"
#include "task_fabricSense.h"
#include "task_commandProc.h"

extern drv_uart_config_t uart0Config;
extern drv_uart_config_t uart1Config;
extern drv_uart_config_t usart0Config;
extern drv_uart_config_t usart1Config;
extern uint16_t packetReceivedMask; 
//global variable of settings structure
brainSettings_t brainSettings = {.isLoaded = 0, .debugPackets = false, .autoTurnOff = true, .debugPrintsEnabled = false}; 


//imuConfiguration array, defined here for now
uint8_t rotl32 (uint8_t value, unsigned int count);
uint8_t rotr32 (uint8_t value, unsigned int count);
void decryptBuf(uint8_t* buffer, uint16_t length);
void encryptBuf(uint8_t* buffer, uint16_t length);
//has maximum amount of NODs possible is 10
imuConfiguration_t imuConfig[] =
{
	{.macAddress = "1ABBCCDDEEFF", .imuId = 0},
	{.macAddress = "2ABBCCDDEEFF", .imuId = 1},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 2},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 3},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 4},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 5},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 6},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 7},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 8},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 9}
	
};

#ifdef USE_Q1_Q2
#define PRINTF_UART UART1
quinticConfiguration_t quinticConfig[] =
{
	{
		.qId = 0,
		.imuArray = {&imuConfig[3],&imuConfig[4],&imuConfig[5]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice = &usart0Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST2,
		.imuMask = "11110000"
	},	
	{
		.qId = 1,
		.imuArray =	{&imuConfig[0],&imuConfig[1],&imuConfig[2]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice =  &uart1Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST1,
		.imuMask = "11110000"
	},
	{
		.qId = 2,
		.imuArray = {&imuConfig[6],&imuConfig[7],&imuConfig[8]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice =&usart1Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST3,
		.imuMask = "11110000"
	}
};
commandProcConfig_t cmdConfig =
{
	.uart = &uart1Config
};
#else
#define PRINTF_UART UART0
quinticConfiguration_t quinticConfig[] =
{
	{
		.qId = 0,
		.imuArray =	{&imuConfig[0],&imuConfig[1],&imuConfig[2]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice =  &uart1Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST1,
		.imuMask = "11110000"
	},
	{
		.qId = 1,
		.imuArray = {&imuConfig[3],&imuConfig[4],&imuConfig[5]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice = &usart0Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST2,
		.imuMask = "11110000"
	},	
	{
		.qId = 2,
		.imuArray = {&imuConfig[6],&imuConfig[7],&imuConfig[8]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice =&usart1Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST3,
		.imuMask = "11110000"
	}
};
commandProcConfig_t cmdConfig =
{
	.uart = &uart0Config
};
#endif

fabricSenseConfig_t fsConfig =
{
	.samplePeriod_ms = 20,
	.numAverages = 4,
	.uartDevice = &uart0Config
};




//static function declarations

//file parsing helper functions
status_t getLineFromBuf(char* bufPtr, char* result, size_t resultSize);

/**
 * loadSettings(char* filename)
 * @brief Load configuration settings to buffers
 */
status_t loadSettings(char* filename)
{	
	status_t result = STATUS_PASS;
	static FIL configFileObj;
	packetReceivedMask = 0;
	debugPrintString("Opening SD Card to read\r\n");
	//initialize the suitNumber
	strncpy(brainSettings.suitNumber, "S0001", 10);
	//initialize the run time settings to their defaults. 
	brainSettings.debugPackets = false; 
	brainSettings.autoTurnOff = true; 
	//brainSettings.debugPrintsEnabled = false;
	filename[0] = LUN_ID_SD_MMC_0_MEM + '0'; //is this necessary? 
	FRESULT res = f_open(&configFileObj, (char const *)filename, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK)
	{
		return STATUS_FAIL;
	}
	//read the whole file into a buffer
	char buf[MAX_CONFIG_FILE_SIZE] = {0}; 	 
	uint16_t bytes_read = 0, total_bytes_read = 0;	
	while(total_bytes_read < configFileObj.fsize && res == FR_OK)
	{
		res = f_read(&configFileObj, buf+total_bytes_read, MAX_CONFIG_FILE_SIZE - total_bytes_read, &bytes_read);
		total_bytes_read += bytes_read; 
	}
	char* bufPtr = 0;	//set pointer to start of buffer
	//Decrypt the data
	if (strncmp(buf, "ee", 2) == 0)		//check if the file is encrypted
	{
		bufPtr = buf + 2;
		decryptBuf(bufPtr, total_bytes_read);
	}
	else
	{
		bufPtr = buf;
	}
	//now parse the file and 
	status_t step_status = STATUS_PASS;
	char line[50] = {0}; 
	int NumberOfNods = 0;	
	if(getLineFromBuf(bufPtr, line, sizeof(line)) == PASS)
	{
		if(sscanf(line, "%s ,%d\r\n",brainSettings.suitNumber,&NumberOfNods) < 2)
		{
			debugPrintString("failed to read settings\r\n");
			return STATUS_FAIL; 
		}
		bufPtr += strlen(line); 		
	}
	brainSettings.numberOfImus = NumberOfNods; 
	//initialize the expectedNumberOfNods
	quinticConfig[0].expectedNumberOfNods = 0;
	quinticConfig[1].expectedNumberOfNods = 0;
	quinticConfig[2].expectedNumberOfNods = 0;	
	int i=0, j=0; 
	int quinticIndex = 0; 
	int imuId = 0;
	char tempMACAddress[20] = {0}; 
	//packetReceivedMask = 0; //reset the mask	
	for(i = 0; i < NumberOfNods; i++)
	{
		if(getLineFromBuf(bufPtr, line,sizeof(line)) == STATUS_PASS)
		{			
			if(sscanf(line,"%d,%d,%s\r\n",&quinticIndex, &imuId, tempMACAddress) < 2)
			{
				debugPrintString("failed to parse IMU settings\r\n"); 
				break;
			}
			else
			{
				if(imuId < 0 || imuId > 9)
				{
					debugPrintStringInt("received incorrect imuId",imuId); 
					break;	
				}
				packetReceivedMask |= (1<<imuId);
				imuConfig[imuId].imuId = imuId; 
				snprintf(imuConfig[imuId].macAddress,20, "%s\r\n",tempMACAddress); 
				imuConfig[imuId].imuValid = true;
				if(quinticIndex < 0 || quinticIndex > 2)
				{
					debugPrintStringInt("failed to assign IMU ",i); 
					break;
				}
				if(quinticConfig[quinticIndex].expectedNumberOfNods >= MAX_NUMBER_OF_IMUS)
				{
					debugPrintStringInt("failed to assign IMU ",imuId);
					break;
				}
				quinticConfig[quinticIndex].imuArray[quinticConfig[quinticIndex].expectedNumberOfNods++] = &imuConfig[imuId]; 
			}
			//debugPrintStringInt("loaded settings for IMU ",imuConfig[imuId].imuId);
			//debugPrintStringInt("On quintic ",quinticIndex);
			//debugPrintString(imuConfig[imuId].macAddress);
			printf("loaded settings for IMU %d on Q%d, %s",imuConfig[imuId].imuId,quinticIndex, imuConfig[imuId].macAddress);
			bufPtr += strlen(line); 
		}
		else
		{
			break; 
		}		
	}
	//populate the expected IMU mask with the loaded config. 
	//for(i=0;i<3;i++)
	//{
		//strcpy(quinticConfig[i].imuMask, "00000000"); //initialize the array
		//for(j=0;j<quinticConfig[i].expectedNumberOfNods;j++)
		//{
			//quinticConfig[i].imuMask[j] = '1'; 
		//}
	//}	
	debugPrintString("Closing the file\r\n");
	res = f_close(&configFileObj);
	if (res != FR_OK)
	{		
		debugPrintString("Error: Cannot Open file\r\n");
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

/**
 * rotl32 (uint8_t value, unsigned int count)
 * @brief Circular rotate left
 */
uint8_t rotl32 (uint8_t value, unsigned int count)
{
	const unsigned int mask = (CHAR_BIT*sizeof(value)-1);
	count &= mask;
	return (value<<count) | (value>>( (-count) & mask ));
}

/**
 * rotr32 (uint8_t value, unsigned int count)
 * @brief Circular rotate right
 */
uint8_t rotr32 (uint8_t value, unsigned int count)
{
	const unsigned int mask = (CHAR_BIT*sizeof(value)-1);
	count &= mask;
	return (value>>count) | (value<<( (-count) & mask ));
}

/**
 * decryptBuf(uint8_t* buffer, uint16_t length)
 * @brief Decrypt the input buffer and save its updated contents
 */
void decryptBuf(uint8_t* buffer, uint16_t length)
{
	uint8_t shift;
	//decryption part
	for (int i = 0; i < length; i++)
	{
		shift = i % 7;
		if (shift == 0)
		{
			shift = 3;
		}
		buffer[i] = rotr32(buffer[i], shift);
	}
	//printf("Decrypted string:\r\n%s\r\n", buffer);	//Debug prints
}

/**
 * encryptBuf(uint8_t* buffer, uint16_t length)
 * @brief Encrypt the input buffer and save its updated contents
 */
void encryptBuf(uint8_t* buffer, uint16_t length)
{
	uint8_t shift;
	//encryption part
	for (int i = 0; i < length; i++)
	{
		shift = i % 7;
		if (shift == 0)
		{
			shift = 3;
		}
		buffer[i] = rotl32(buffer[i], shift);
	}
	//printf("Encrypted string:\r\n%s\r\n", buffer);	//Debug prints
}