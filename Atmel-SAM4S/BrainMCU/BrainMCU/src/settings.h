/*
 * settings.h
 *
 * Created: 9/16/2015 5:20:42 PM
 *  Author: Hriday Mehta
 */ 

#include <asf.h>
#include "common.h"

#define MAX_NUMBER_OF_IMUS 10
#define MAC_ADDRESS_LENGTH 15

#ifndef CONFIG_SETTINGS_H_
#define CONFIG_SETTINGS_H_
#define SETTINGS_FILENAME "0:settings.dat"

#define MAX_CONFIG_FILE_SIZE 300
#define MAX_SUIT_NUMBER_LENGTH 50 

typedef struct
{
	int isLoaded; 
	//imuSettings_t imuSettings[MAX_NUMBER_OF_IMUS];
	int numberOfImus;
	char suitNumber[MAX_SUIT_NUMBER_LENGTH];  
	bool debugPackets;		//toggle zeroing of repeated IMU data. (default off)  
	bool autoTurnOff;		//toggle auto off when in idle. (default on)
	bool debugPrintsEnabled; 
	char channelmap[20];	//BLE channel map for Quintics
}brainSettings_t;


extern status_t loadSettings(char* filename);

#endif /* CONFIG_SETTINGS_H_ */