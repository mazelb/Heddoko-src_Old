/*
 * Config_Settings.h
 *
 * Created: 9/16/2015 5:20:42 PM
 *  Author: Hriday Mehta
 */ 

#include "BrainMCU.h"
#include <asf.h>
#include "common.h"

#define MAX_NUMBER_OF_NODS 10
#define MAC_ADDRESS_LENGTH 15

#ifndef CONFIG_SETTINGS_H_
#define CONFIG_SETTINGS_H_
#define SETTINGS_FILENAME "0:settings.csv"

#define MAX_CONFIG_FILE_SIZE 2048
static char buf[MAX_CONFIG_FILE_SIZE] = {0}; 
static char line[50] = {0};	

typedef struct  
{
	char nodMacAddress[MAC_ADDRESS_LENGTH];
	int nodId;
}nodSettings_t;
typedef struct
{
	int isLoaded; 
	nodSettings_t nodSettings[MAX_NUMBER_OF_NODS];
	int numberOfNods; 
	 
}brainSettings_t;

static uint8_t data_buffer[DATA_SIZE];

extern int ReadConfigSD	(void);
extern int StoreConfig	(void);
extern status_t loadSettings(char* filename);

#endif /* CONFIG_SETTINGS_H_ */