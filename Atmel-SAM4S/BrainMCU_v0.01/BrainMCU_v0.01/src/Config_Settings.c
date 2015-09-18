/*
 * Config_Settings.c
 *
 * Created: 9/16/2015 5:21:04 PM
 *  Author: Hriday Mehta
 */ 
#include "Config_Settings.h"
#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "conf_board.h"
#include "BrainMCU.h"

/**
 * ReadConfigSD(void)
 * @brief Read configuration settings from SD card
 */
extern int ReadConfigSD	(void)
{
	
	UINT byte_to_read, byte_read;
	uint8_t result = SUCCESS;
	
	printf("Opening SD Card to read\r\n");
	
	file_name[0] = LUN_ID_SD_MMC_0_MEM + '0';
	res = f_open(&file_object, (char const *)file_name, FA_OPEN_EXISTING | FA_READ);
	if (res != FR_OK)
	{
		result = CANNOT_OPEN;
		printf("Error: Cannot Open file\r\n");
		return result;
	}
	
	printf("Reading from SD\r\n");
	
	memset(data_buffer, 0, DATA_SIZE);
	byte_to_read = file_object.fsize;

	for (int i = 0; i < byte_to_read; i += DATA_SIZE)
	{
		res = f_read(&file_object, data_buffer, DATA_SIZE, &byte_read);
		if (res != FR_OK)
		{
			result = READ_FAILED;
			printf("Error: Cannot Open file\r\n");
			return result;
		}
	}
	
	printf("Closing the file\r\n");
	res = f_close(&file_object);
	if (res != FR_OK)
	{
		result = CANNOT_CLOSE;
		printf("Error: Cannot Open file\r\n");
		return result;
	}	
}

/**
 * StoreConfig(void)
 * @brief Store the configuration settings to respective elements
 */
extern int StoreConfig(void)
{
	/*	Fetch addresses from text read and store to global structure	*/
	uint8_t i=0, j=0, k=0;
	UINT byte_to_read, byte_read;
	do
	{
		Nod.id[i].Addr[j] = data_buffer[k];
		if((Nod.id[i].Addr[j]==0x0A)||(Nod.id[i].Addr[j]==0x0D))		//Exit if End of line is detected
		{
			break;
		}
		
		if (Nod.id[i].Addr[j]==0x2c)		//Separate Data based on commas
		{
			Nod.id[i].Addr[j] = 0x0D;
			Nod.id[i].Addr[++j] = 0X0A;
			j=0;
			i++;
		}
		else
		{
			j++;
		}
		
		k++;
		
	} while (1);
	
	for (int i=0; i<NOD_MAX_CNT; i++)		//Print the segregated data
	{
		printf("%s", Nod.id[i].Addr);
	}
	
	return 0;
}
