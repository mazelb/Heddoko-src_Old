/*
 * Commands.h
 *
 * Created: 9/9/2015 10:54:42 AM
 *  Author: Hriday Mehta
 */ 


#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdlib.h>
#include "BrainMCU.h"
#include "Serial.h"

//volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */

extern int Q1Init	(void);
extern int Q2Init	(void);
extern int Q3Init	(void);
extern int QnStart	(void);
extern int QnStop	(void);
extern void Delay	(uint32_t dlyTicks);
//void SysTick_Handler	(void);


#endif /* COMMANDS_H_ */