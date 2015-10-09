/*
 * task_dataProcessor.h
 *
 * Created: 9/28/2015 11:19:10 AM
 *  Author: Sean Cloghesy
 */ 


#ifndef TASK_DATAPROCESSOR_H_
#define TASK_DATAPROCESSOR_H_


#define TASK_DATA_HANDLER_STACK_SIZE                (4096/sizeof(portSTACK_TYPE))
#define TASK_DATA_HANDLER_PRIORITY            (tskIDLE_PRIORITY + 7)
#define MAX_DATA_PACKET_SIZE 32

typedef enum
{
	DATA_PACKET_TYPE_IMU,
	DATA_PACKET_TYPE_SS,
	DATA_PACKET_TYPE_CMD	
}dataPacket_type_t;

typedef struct  
{
	dataPacket_type_t type; 
	uint8_t imuId; 
	uint8_t imuIndex; 
	char data[MAX_DATA_PACKET_SIZE];
	 
}dataPacket_t;

void task_dataHandler(void *pvParameters);

#endif /* TASK_DATAPROCESSOR_H_ */