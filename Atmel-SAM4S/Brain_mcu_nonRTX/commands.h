#ifndef __COMMANDS_H
#define __COMMANDS_H

#include <SAM4S.h>
#include <stdlib.h>
#include "Serial.h"

extern int Q1_init(void);
extern int Q2_init(void);
extern int Q3_init(void);
extern void Qn_start(void);
extern void Delay	(uint32_t dlyTicks);
void SysTick_Handler	(void);

#endif