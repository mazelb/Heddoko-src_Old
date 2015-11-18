/*
 * task_fabricSense.c
 *
 * Created: 10/5/2015 9:19:27 AM
 *  Author: Sean Cloghesy
 */ 

#include "task_fabricSense.h"
#include "task_dataProcessor.h"
#include "task_commandProc.h"
#include "task_stateMachine.h"
#include "task_quinticInterface.h"
#include <string.h>
//#define CREATE_DUMMY_PACKETS 
extern xQueueHandle queue_dataHandler;
extern uint32_t sgSysTickCount;
extern bool enableRecording; 
extern uint16_t packetReceivedMask; 
extern drv_uart_config_t uart1Config;
//static function foward declarations

void createDummyFabSensePacket(char* dataPacket, size_t maxPacketSize, uint32_t seqNum);

/***********************************************************************************************
 * task_fabSenseHandler(void *pvParameters)
 * @brief The main task for a fabric sense module, associated UART has to be initialized before calling this
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
	//if(task_fabSense_init(fsConfig) != STATUS_PASS)
	//{
		////this is an error, we should do something here!
		//return; 
	//} 
	dataPacket_t packet;
	packet.type = DATA_PACKET_TYPE_SS;
	//main loop of task, this is where we request information and store it.
	char buf[FS_RESPONSE_BUF_SIZE] = {0};
	uint8_t numPacketsReceived = 0; 
	uint8_t dataSize = 0;
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
			if (getCurrentState() != SYS_STATE_ERROR)		//Remove when working without Quintics
			{
				if (drv_uart_getlineTimedSized(fsConfig->uartDevice, buf, CMD_RESPONSE_BUF_SIZE, 400, &dataSize) == STATUS_PASS)
				{
					if(strncmp(buf, "!", 1) == 0)
					{
						if (dataSize != 31)
						{
							//the size does not match the expected size, package is corrupt
						}
						else
						{
							//drv_uart_putString(&uart1Config, buf);
							memcpy(packet.data, buf+1, 20);		//ignore the first character and the end time stamp
						}
					}
					else if (strncmp(buf, "@", 1) == 0)
					{
						printString("Received command feedback from FabSense\r\n");
						//do nothing the message should just be consumed
					}
					
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
			}		
		}
		else
		{
			vTaskDelay(10); 
		}
		
	}
}
/***********************************************************************************************
 * task_fabSense_init(fabricSenseConfig_t* fabSenseConfig)
 * @brief Function called to initialize the fabric sense module parameters
 * @param pvParameters, void pointer to structure containing fabric sense configuration. 
 * @return void
 ***********************************************************************************************/
status_t task_fabSense_init(fabricSenseConfig_t* fabSenseConfig)
{
	status_t result = STATUS_FAIL;
	char buf[150] = {0};
	char* bufPtr = buf;
	uint8_t initTryCount = 3;
	uint32_t vSamplePeriod = 0, vNumAvg = 0;
	char vCommandStr[20] = {0};
		
	drv_uart_getlineTimed(fabSenseConfig->uartDevice, buf, sizeof(buf), 1500);	//consume any garbage data present on the bus
	drv_uart_flushRx(fabSenseConfig->uartDevice);	//flush the uart first
	
	snprintf(vCommandStr, sizeof(vCommandStr), "#p %d\r\n", fabSenseConfig->samplePeriod_ms);
	// Send the command to change the time interval to 20ms. Do it max 3 times.
	while (initTryCount != 0)
	{
		drv_uart_putString(fabSenseConfig->uartDevice, vCommandStr);
		vTaskDelay(1);
		if (drv_uart_getlineTimed(fabSenseConfig->uartDevice, buf, sizeof(buf), 1500) == STATUS_PASS)
		{
			drv_uart_putString(&uart1Config, buf);
			if (strncmp(buf, "@Sample", 7) == 0)
			{
				bufPtr = buf + 18;
				if (strncmp(bufPtr, "20 msec", 7) == 0)
				{
					//result = STATUS_PASS;
					break;
				}
			}
		}
		initTryCount--;
	}
	
	initTryCount = 3;
	snprintf(vCommandStr, sizeof(vCommandStr), "#f %d\r\n", fabSenseConfig->numAverages);
	// Send the command to change the time interval to 20ms. Do it max 3 times.
	while (initTryCount != 0)
	{
		drv_uart_putString(fabSenseConfig->uartDevice, vCommandStr);
		vTaskDelay(1);
		if (drv_uart_getlineTimed(fabSenseConfig->uartDevice, buf, sizeof(buf), 1500) == STATUS_PASS)
		{
			drv_uart_putString(&uart1Config, buf);
			if (strncmp(buf, "@N-Filter", 9) == 0)
			{
				bufPtr = buf + 14;
				if (strncmp(bufPtr, "4samples", 8) == 0)
				{
					result = STATUS_PASS;
					break;
				}
			}
		}
		initTryCount--;
	}
	
	return result;
}
/***********************************************************************************************
 * task_fabSense_start(fabricSenseConfig_t* fabSenseConfig)
 * @brief Function called to start the fabrix sense transmission of data. 
 * @param pvParameters, void pointer to structure containing fabric sense configuration. 
 * @return void
 ***********************************************************************************************/
status_t task_fabSense_start(fabricSenseConfig_t* fabSenseConfig)
{
	drv_uart_putString(fabSenseConfig->uartDevice, "#s\r\n");
	status_t status = STATUS_PASS; 
	enableRecording = true; 
	return status; 
}
/***********************************************************************************************
 * task_fabSense_stop(fabricSenseConfig_t* fabSenseConfig)
 * @brief Function called to stop the fabric sense module  
 * @param pvParameters, void pointer to structure containing fabric sense configuration. 
 * @return void
 ***********************************************************************************************/
status_t task_fabSense_stop(fabricSenseConfig_t* fabSenseConfig)
{
	drv_uart_putString(fabSenseConfig->uartDevice, "#t\r\n");
	status_t status = STATUS_PASS;
	enableRecording = false; 
	return status;	
}


//static functions


void createDummyFabSensePacket(char* dataPacket, size_t maxPacketSize, uint32_t seqNum)
{
	int i = 0; 
	snprintf(dataPacket, maxPacketSize, "%04dBBBBCCCCDDDDEEEE%08d\r\n",seqNum%10000, sgSysTickCount); 

}