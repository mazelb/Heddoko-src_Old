/*
 * task_sdCardWrite.h
 *
 * Created: 10/19/2015 8:21:00 AM
 *  Author: sean
 */ 


#ifndef TASK_SDCARDWRITE_H_
#define TASK_SDCARDWRITE_H_
#include "common.h"
#define SD_CARD_BUFFER_SIZE 2136
#define SD_CARD_FILENAME_LENGTH 150


void task_sdCardHandler(void *pvParameters);
status_t task_sdCardWriteEntry(char* entry, size_t length); 
status_t task_sdCard_OpenNewFile();
status_t task_sdCard_CloseFile();
#endif /* TASK_SDCARDWRITE_H_ */