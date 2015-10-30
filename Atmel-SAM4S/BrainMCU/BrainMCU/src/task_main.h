/*
 * task_main.h
 *
 * Created: 9/23/2015 3:34:06 PM
 *  Author: Hriday Mehta
 */ 


#ifndef TASK_MAIN_H_
#define TASK_MAIN_H_


#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Serial.h"
#include "GPIO.h"
#include "DebugLog.h"

static bool svStartStopToggle=0, svPowerSwIrqFlag, svActionSw1IrqFlag, svActionSw2IrqFlag, svSwBtnIrqFlag, svInitCmpFlag=0, svMainTaskCmpFlag=0;
static uint8_t svMenuLevel=0;
static uint32_t svCurrentTicks, svOldTicks, svPioaIsrVal=0;

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);

extern void SysTickHandler(void);

/*	Tasks Declaration	*/
extern void TaskMain	(void *pvParameters);

int StartTimer (void);

#endif /* TASK_MAIN_H_ */