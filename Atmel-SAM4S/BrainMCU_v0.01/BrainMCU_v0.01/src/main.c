/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Functionality_Tests.h"
#include "Board_Init.h"
#include "Config_Settings.h"
#include "conf_board.h"
#include "BrainMCU.h"
#include "Tasks.h"
#include "Ser.h"
#include "Commands.h"
#include "GPIO.h"

int main (void)
{	
	irq_initialize_vectors();
	cpu_irq_enable();
	
	BoardInit();

	StartupTest();
		
	/*	Perform Read Write Tests	*/
	if (SDWriteTest() == SUCCESS)
	{
		printf("Success: Passed Write Tests\r\n");
	}
	
	if (SDReadTest() == SUCCESS)
	{
		printf("Success: Passed Read Tests\r\n");
	}
	
	
	/*	Retrieve and store Configuration Settings from SD Card	*/
	if (ReadConfigSD() == SUCCESS)
	{
		printf("Success: Configuration Read\r\n");
	}
	
	if (StoreConfig() == SUCCESS)
	{
		printf("Success: Store Configuration\r\n");
	}
	
	// Insert application code here, after the board has been initialized.
	//Q1Init();

	/* Create task to make led blink */
	if (xTaskCreate(task_led, "Led", TASK_LED_STACK_SIZE, NULL, TASK_LED_STACK_PRIORITY, NULL ) != pdPASS) 
	{
		printf("Failed to create test led task\r\n");
	}
	
	/* Start the scheduler. */
	vTaskStartScheduler();
	
	/*	Debug code */
	while (1) 
	{		

		//SerialPrint(Q1,"send1\r\n");
		//cnt=Nod.id[0].ElementCount;
		//SerialPrint(SS,"\r\nstart1\r\n");
		//while(Nod.id[0].ElementCount<(cnt+21))
		//{
			//if(Nod.id[0].BufCount<gBufSize)
			//ReadUsart1(0);
			//if(Nod.id[0].DataCount>=gDataSize)
			//{				
				//Nod.id[0].ElementCount++;
				//Nod.id[0].DataCount=0;
			//}
			//if(Nod.id[0].ElementCount>=gElementNum)
			//{				
				//Nod.id[0].BufCount++;
				//Nod.id[0].ElementCount=0;
				//Nod.id[0].DataCount=0;
			//}
			//if(Nod.id[0].BufCount>gBufSize)
			//Nod.id[0].BufCount=0;
		//}
		//SerialPrint(SS, "\r\nend1\r\n");
		//
		//SerialPrint(Q1,"send2\r\n");
		//cnt=Nod.id[1].ElementCount;
		//SerialPrint(SS, "\r\nstart2\r\n");
		//while(Nod.id[1].ElementCount<(cnt+21))
		//{
			//if(Nod.id[1].BufCount<gBufSize)
			//ReadUsart1(1);
			//if(Nod.id[1].DataCount>=gDataSize)
			//{				
				//Nod.id[1].ElementCount++;
				//Nod.id[1].DataCount=0;
			//}
			//if(Nod.id[1].ElementCount>=gElementNum)
			//{				
				//Nod.id[1].BufCount++;
				//Nod.id[1].ElementCount=0;
				//Nod.id[1].DataCount=0;
			//}
			//if(Nod.id[1].BufCount>gBufSize)
			//Nod.id[1].BufCount=0;
		//}
		//SerialPrint(SS, "\r\nend2\r\n");
		//
		//SerialPrint(Q1,"send3\r\n");
		//cnt=Nod.id[2].ElementCount;
		//SerialPrint(SS, "\r\nstart3\r\n");
		//while(Nod.id[2].ElementCount<(cnt+21))
		//{
			//if(Nod.id[2].BufCount<gBufSize)
			//ReadUsart1(2);
			//if(Nod.id[2].DataCount>=gDataSize)
			//{				
				//Nod.id[2].ElementCount++;
				//Nod.id[2].DataCount=0;
			//}
			//if(Nod.id[2].ElementCount>=gElementNum)
			//{				
				//Nod.id[2].BufCount++;
				//Nod.id[2].ElementCount=0;
				//Nod.id[2].DataCount=0;
			//}
			//if(Nod.id[2].BufCount>gBufSize)
			//Nod.id[2].BufCount=0;
		//}
		//SerialPrint(SS, "\r\nend3\r\n");
	}
	return 0;
}
