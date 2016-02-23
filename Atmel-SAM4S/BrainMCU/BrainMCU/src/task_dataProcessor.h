/*
 * task_dataProcessor.h
 *
 * Created: 9/28/2015 11:19:10 AM
 *  Author: Sean Cloghesy
 */ 


#ifndef TASK_DATAPROCESSOR_H_
#define TASK_DATAPROCESSOR_H_



#define NO_OF_IMUS_FOR_PACKET	9

#define MAX_DATA_PACKET_SIZE	32
#define NO_OF_FAB_SENSE			1


typedef enum
{
	DATA_PACKET_TYPE_IMU,
	DATA_PACKET_TYPE_SS,
	DATA_PACKET_TYPE_CMD,
	DATA_PACKET_TYPE_ACCEL,
	DATA_PACKET_TYPE_EM
}dataPacket_type_t;

typedef struct  
{
	dataPacket_type_t type; 
	uint8_t imuId; 
	uint8_t imuIndex; 
	char data[MAX_DATA_PACKET_SIZE];	 
}dataPacket_t;

typedef struct  
{
	uint16_t flag;
	char imuData[9][12]; //data for each IMU
	char fabSense[25];
	//char emData[2][12];  //data for each sensor
}dataFrame_t;

void task_dataHandler(void *pvParameters);
void task_dataProcessor_startRecording();
void task_dataProcessor_startGetAccelData(uint32_t numberOfFrames);
#endif /* TASK_DATAPROCESSOR_H_ */