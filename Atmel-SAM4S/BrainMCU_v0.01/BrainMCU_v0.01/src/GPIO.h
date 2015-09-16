/*
 * GPIO.h
 *
 * Created: 9/9/2015 12:40:11 PM
 *  Author: Hriday Mehta
 */ 


#include "BrainMCU.h"
#include <stdlib.h>

#ifndef GPIO_H_
#define GPIO_H_


extern void         ButtonInit(void);
extern unsigned int button_get (void);


#endif /* GPIO_H_ */