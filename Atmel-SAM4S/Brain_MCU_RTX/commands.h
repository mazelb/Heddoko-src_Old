#ifndef __COMMANDS_H
#define __COMMANDS_H

#include <SAM4S.h>
#include <stdlib.h>
#include "Serial.h"

//volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */

extern int Q1Init(void);
extern int Q2Init(void);
extern int Q3Init(void);
extern int QnStart(void);
extern int QnStop(void);
extern void Delay	(uint32_t dlyTicks);
//void SysTick_Handler	(void);

#endif