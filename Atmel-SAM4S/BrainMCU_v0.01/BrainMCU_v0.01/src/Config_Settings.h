/*
 * Config_Settings.h
 *
 * Created: 9/16/2015 5:20:42 PM
 *  Author: Hriday Mehta
 */ 

#include "BrainMCU.h"
#include <asf.h>

#ifndef CONFIG_SETTINGS_H_
#define CONFIG_SETTINGS_H_

static uint8_t data_buffer[DATA_SIZE];

extern int ReadConfigSD	(void);
extern int StoreConfig	(void);

#endif /* CONFIG_SETTINGS_H_ */