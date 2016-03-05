/*
 * cmd_commandProc.c
 *
 * Created: 3/1/2016 9:00:18 AM
 *  Author: sean
 */ 
#include <asf.h>
#include <string.h>
#include "cmd_commandProc.h"
#include "dat_dataRouter.h"
#include "common.h"

xQueueHandle cmd_queue_commandQueue = NULL;
//static function forward declarations
static void setTimeFromString(char* dateTime);

/***********************************************************************************************
 * cmd_task_commandProcesor(void *pvParameters)
 * @brief This task receives all incoming commands to the brain pack, and responds to the ones it needs to.   
 * @param pvParameters, void pointer to structure containing configuration
 * @return void
 ***********************************************************************************************/
void cmd_task_commandProcesor(void *pvParameters)
{
	cmd_queue_commandQueue = xQueueCreate( 10, sizeof(cmd_commandPacket_t));
	cmd_commandPacket_t packet; 
	cmd_initPacketStructure(&packet);
	if(cmd_queue_commandQueue == 0)
	{
		// Queue was not created this is an error!		
		return;
	}
	while(1)
	{	
		if(xQueueReceive( cmd_queue_commandQueue, &(packet), 1000) == TRUE)
		{
			//only a small subset of commands are handled on the power board
			//send the rest to the databoard for processing.
			if(packet.packetSize > 0)
			{		
				if(strncmp(packet.packetData,"setTime",7)==0)
				{
					//handle the set time command. 
					if(strlen(packet.packetData) >= 24)
					{
						setTimeFromString(packet.packetData+7);
					}
				}		
				//forward the command to the data board. 
				dat_sendPacketToDataBoard(&packet); 
			}
		}
	}
		
}

void cmd_initPacketStructure(cmd_commandPacket_t* packet)
{
	memset(packet->packetData,0,CMD_INCOMING_CMD_SIZE_MAX);
	packet->packetSize = 0;
	packet->packetSource = CMD_COMMAND_SOURCE_DAUGHTER; 
}

char debugMessage[100] = {0};
status_t cmd_sendDebugMsgToDataBoard(char* errorMessage)
{
	cmd_commandPacket_t packet; 
	//add prepended message to databoard packet
	snprintf(packet.packetData,CMD_INCOMING_CMD_SIZE_MAX,"PwrBrdMsg:%s", errorMessage);	
	packet.packetSize = strlen(packet.packetData); 
	packet.packetSource = CMD_COMMAND_SOURCE_LOCAL;
	if(cmd_queue_commandQueue != NULL)
	{
		if(xQueueSendToBack(cmd_queue_commandQueue,( void * ) &packet,5) != TRUE)
		{
			//this is an error, we should log it.
		}	
	}
}

//static functions
static void setTimeFromString(char* dateTime)
{
	uint32_t year, month, day, dow; //dow is day of week (1-7)
	uint32_t hour, minute, second;
	if(sscanf(dateTime,"%d-%d-%d-%d-%d:%d:%d\r\n", &year, &month, &day, &dow, &hour, &minute, &second ) == 7)
	{
		// we successfully parsed the data, set the time and date
		rtc_set_time(RTC,hour,minute,second);
		rtc_set_date(RTC,year,month,day,dow);
		//printString("ACK\r\n");
	}
	else
	{
		//printString("NACK\r\n");
	}
}
