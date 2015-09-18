/*
 * quintic_interface_task.h
 *
 * Created: 9/18/2015 10:50:03 AM
 *  Author: Sean Cloghesy
 */ 


#ifndef QUINTIC_INTERFACE_TASK_H_
#define QUINTIC_INTERFACE_TASK_H_

#define NOD_MAC_ADDRESS_LENGTH 12 
#define NOD_PACKET_LENGTH 64
#define NOD_BUFFER_SIZE 100
#define QUINTIC_NUMBER_OF

#define QUINTIC_MAX_NUMBER_OF_NODS 3

typedef struct
{
	int nodId; 
	char macAddress[NOD_MAC_ADDRESS_LENGTH]; //stored as ASCII
	char packetBuffer[NOD_BUFFER_SIZE][NOD_PACKET_LENGTH]; 
	int bufferHead;
	int bufferEnd; 	 	
	
}nodConfiguration_t;


typedef struct  
{
	nodConfiguration_t nodArray[QUINTIC_MAX_NUMBER_OF_NODS]; 
	drvUart_t device; 	
}quiticConfiguration_t;

//UART device driver
#define FIFO_BUFFER_SIZE 255
typedef struct
{
	uint8_t data_buf[FIFO_BUFFER_SIZE];
	uint16_t i_first;
	uint16_t i_last;
	uint16_t num_bytes;
}sw_fifo_typedef;


typedef struct  
{
	
	void* getchar; 
	void* putchar; 
	sw_fifo_typedef rx_fifo; //fifo for uart receive
	uint8_t rx_fifo_not_empty_flag; // this flag is automatically set and cleared by the software buffer
	uint8_t rx_fifo_full_flag;      // this flag is automatically set and cleared by the software buffer
	uint8_t rx_fifo_ovf_flag;       // this flag is not automatically cleared by the software buffer
	
}drvUart_t;

#endif /* QUINTIC_INTERFACE_TASK_H_ */