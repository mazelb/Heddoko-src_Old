/*
 * task_fabricSense.h
 *
 * Created: 10/5/2015 9:19:50 AM
 *  Author: Sean Cloghesy
 */ 
#include "common.h"
#include "drv_uart.h"

#ifndef TASK_FABRICSENSE_H_
#define TASK_FABRICSENSE_H_
#define TASK_FABSENSE_STACK_SIZE                (4096/sizeof(portSTACK_TYPE))
#define TASK_FABSENSE_STACK_PRIORITY            (tskIDLE_PRIORITY + 5)
#define FS_RESPONSE_BUF_SIZE						  255
typedef struct  
{
	uint32_t samplePeriod_ms; //time between received samples of data
	uint32_t numAverages;     //number of values used for the running average. 
	drv_uart_config_t* uartDevice; //uart connected to stretch sense module (must be initialized elsewhere)
}fabricSenseConfig_t;

void task_fabSenseHandler(void *pvParameters); 
status_t task_fabSense_start(fabricSenseConfig_t* fabSenseConfig);
status_t task_fabSense_stop(fabricSenseConfig_t* fabSenseConfig);


#endif /* TASK_FABRICSENSE_H_ */