/*
 * DebugLog.h
 *
 * Created: 9/23/2015 2:51:50 PM
 *  Author: Hriday Mehta
 */ 


#ifndef DEBUGLOG_H_
#define DEBUGLOG_H_


#include <asf.h>
#include "BrainMCU.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Serial.h"
#include "UART_functionality.h"
#include "GPIO.h"
#include "task_main.h"

#define TASK_DEBUGLOG_STACK_SIZE                (1024/sizeof(portSTACK_TYPE))
#define TASK_DEBUGLOG_STACK_PRIORITY            (tskIDLE_PRIORITY + 1)

static int size=0;
static FIL debug_file_object;

/*	Tasks Declaration	*/
extern void TaskDebugLog(void *pvParameters);
extern void DebugLogBufPrint(char* InputString);
extern void DebugLogCreate(void);
extern void DebugLogSave(void);


#endif /* DEBUGLOG_H_ */