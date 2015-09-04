#ifndef __COMMANDS_H
#define __COMMANDS_H

#include <SAM4S.h>
#include <stdlib.h>
#include "Serial.h"

//volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */

extern int Q1_init(void);
extern int Q2_init(void);
extern int Q3_init(void);
extern int Qn_start(void);
extern int Qn_stop(void);
extern void Delay	(uint32_t dlyTicks);
//void SysTick_Handler	(void);

#endif