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
#include <string.h>

//static declarations

static status_t sendString(drv_uart_config_t* uartConfig, char* cmd);
static status_t getLine(drv_uart_config_t* uartConfig, char* str, size_t str_size); 
static status_t getAck(drv_uart_config_t* uartConfig);
static status_t initializeNods(quinticConfiguration_t* qConfig); 
/***********************************************************************************************
 * task_quinticHandler(void *pvParameters)
 * @brief The main task for a quintic module, associate UART has to be initialized before calling this
 *	function. 
 * @param pvParameters, void pointer to structure containing quintic and nod configuration. 
 * @return void
 ***********************************************************************************************/
void task_quinticHandler(void *pvParameters)
{
	//cast void pointer to quintic config type. 
	quinticConfiguration_t *qConfig = (quinticConfiguration_t*)pvParameters; 
	//initialize all structures ,check everything is alright
	int i = 0;
	//initialize all buffers and pointers to zero
	for(i = 0 ; i < QUINTIC_MAX_NUMBER_OF_NODS ; i++)
	{
		qConfig->nodArray[i]->bufferEnd = 0;
		qConfig->nodArray[i]->bufferHead = 0;
		memset(qConfig->nodArray[i]->packetBuffer, 0 , NOD_BUFFER_SIZE*NOD_PACKET_LENGTH); 	
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
	initializeNods(qConfig); 
	
	//main loop of task, this is where we request information and store it. 
	char buf[CMD_RESPONSE_BUF_SIZE] = {0}; 
	while(1)
	{
		//for now just get a line and return one... just to see if things are working
		if(getLine(qConfig->uartDevice, buf, CMD_RESPONSE_BUF_SIZE) == STATUS_PASS)
		{
			sendString(qConfig->uartDevice, buf); 
		}
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
static status_t getLine(drv_uart_config_t* uartConfig, char* str, size_t strSize)
{
	status_t result = STATUS_PASS; 
	char val; 
	int pointer = 0;
	while(1) //TODO add timeout
	{	
		result = drv_uart_getChar(uartConfig,&val);
		if(result != STATUS_EOF && val != NULL)
		{
			if(pointer < strSize)
			{
				str[pointer++] = val; //add the result; 
				if(val == '\n')
				{
					str[pointer] = NULL; //terminate the string
					result = STATUS_PASS; 
					pointer = 0; //reset the pointer.
					break;
				}
			}
			else
			{
				//we overwrote the buffer 
				result = STATUS_FAIL;
				str[strSize - 1] = NULL; //terminate what's in the buffer. 
				pointer = 0;
				break;
			}
		}
		vTaskDelay(10); //let the other processes do stuff	
	}	
}

static status_t getAck(drv_uart_config_t* uartConfig)
{
	status_t result = STATUS_PASS; 
	char buf[CMD_RESPONSE_BUF_SIZE] = {0}; //should move to static buffer for each quintic?
	result = getLine(uartConfig, buf,CMD_RESPONSE_BUF_SIZE);
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
	if(getLine(uartConfig, buf,CMD_RESPONSE_BUF_SIZE) == STATUS_PASS)
	{
		if(strcmp(buf,expectedResponse) != 0)
		{
			result = STATUS_FAIL;
		}
	}
	return result;
}
static status_t initializeNods(quinticConfiguration_t* qConfig)
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
		sendString(qConfig->uartDevice,qConfig->nodArray[i]->macAddress); 
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