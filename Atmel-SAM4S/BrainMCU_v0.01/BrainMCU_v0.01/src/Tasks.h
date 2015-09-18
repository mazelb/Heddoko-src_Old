/*
 * Tasks.h
 *
 * Created: 9/9/2015 12:24:06 PM
 *  Author: Hriday Mehta
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "BrainMCU.h"
#include "Serial.h"
#include "UART_functionality.h"
#include <asf.h>
#include "GPIO.h"

#define TASK_MONITOR_STACK_SIZE            (2048/sizeof(portSTACK_TYPE))
#define TASK_MONITOR_STACK_PRIORITY        (tskIDLE_PRIORITY)
#define TASK_SERIAL_RECEIVE_STACK_SIZE                (1024/sizeof(portSTACK_TYPE))
#define TASK_SERIAL_RECEIVE_STACK_PRIORITY            (tskIDLE_PRIORITY)

static bool svStartStopToggle=0, svPowerSwIrqFlag, svActionSw1IrqFlag, svActionSw2IrqFlag, svSwBtnIrqFlag, svInitCmpFlag=0, svMainTaskCmpFlag=0;
static uint8_t svMenuLevel=0;
static uint32_t svCurrentTicks, svOldTicks, svPioaIsrVal=0;

int StartTimer (void);

#endif /* TASKS_H_ */