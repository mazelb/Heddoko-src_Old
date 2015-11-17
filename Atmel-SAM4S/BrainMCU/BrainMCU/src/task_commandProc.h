/*
 * task_commandProc.h
 *
 * Created: 10/28/2015 9:47:21 AM
 *  Author: sean
 */ 


#ifndef TASK_COMMANDPROC_H_
#define TASK_COMMANDPROC_H_

#include "drv_uart.h"

typedef struct  
{
	drv_uart_config_t* uart; 	
}commandProcConfig_t;

void task_commandHandler(void *pvParameters); 
void printString(char* str);

#endif /* TASK_COMMANDPROC_H_ */