/*
 * task_main.c
 *
 * Created: 9/23/2015 3:33:48 PM
 *  Author: Hriday Mehta
 */ 

#include "task_main.h"
#include "Board_Init.h"
#include "common.h"
#include "Config_Settings.h"
#include "drv_uart.h"
#include "drv_gpio.h"
#include "task_quinticInterface.h"
#include "task_dataProcessor.h"
#include "task_fabricSense.h"
#include "task_sdCardWrite.h"
#include "task_stateMachine.h"
#include "Functionality_Tests.h"
#include <string.h>
#include "DebugLog.h"



extern drv_uart_config_t uart0Config;
extern drv_uart_config_t uart1Config;
extern drv_uart_config_t usart0Config;
extern drv_uart_config_t usart1Config;
extern brainSettings_t brainSettings;
volatile bool enableRecording = false; 
extern xQueueHandle queue_dataHandler;
extern uint32_t totalBytesWritten; 
extern uint32_t totalFramesWritten;
extern unsigned long sgSysTickCount;
uint32_t PioIntMaskA, PioIntMaskB, PioIntMaskC;
bool toggle;
uint32_t oldSysTick, newSysTick;
 
//imuConfiguration array, defined here for now
//has maximum amount of NODs possible is 10
imuConfiguration_t imuConfig[] =
{
	{.macAddress = "1ABBCCDDEEFF", .imuId = 0},
	{.macAddress = "2ABBCCDDEEFF", .imuId = 1},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 2},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 3},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 4},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 5},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 6},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 7},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 8},
	{.macAddress = "3ABBCCDDEEFF", .imuId = 9}
	
};

quinticConfiguration_t quinticConfig[] =
{
	{
		.qId = 0, 
		.imuArray =	{&imuConfig[0],&imuConfig[1],&imuConfig[2]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice =  &uart1Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST1
	},
	{
		.qId = 1,
		.imuArray = {&imuConfig[3],&imuConfig[4],&imuConfig[5]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice = &usart0Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST2
	},
	{
		.qId = 2,
		.imuArray = {&imuConfig[6],&imuConfig[7],&imuConfig[8]},
		.expectedNumberOfNods = 3,
		.isinit = 0,
		.uartDevice =&usart1Config,
		.resetPin = DRV_GPIO_PIN_BLE_RST3
	}
};

fabricSenseConfig_t fsConfig = 
{
	.samplePeriod_ms = 20,
	.numAverages = 20, 
	.uartDevice = &uart0Config	
};
static void checkInputGpio(void);

/**
 * \brief Called if stack overflow during execution
 */
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed char *pcTaskName)
{
	//printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;) {
	}
}

/**
 * \brief This function is called by FreeRTOS idle task
 */
extern void vApplicationIdleHook(void)
{
}

/**
 * \brief This function is called by FreeRTOS each tick
 */
extern void vApplicationTickHook(void)
{
}

char runTimeStats[50*10] = {0}; 

/***********************************************************************************************
 * printStats()
 * @brief Prints the Stats of the system to serial terminal
 * @param 
 * @return 
 ***********************************************************************************************/
void printStats()
{
	int i = 0; 
	size_t numberOfImus = sizeof(imuConfig) / sizeof(imuConfiguration_t);
	size_t numberOfQuintics = sizeof(quinticConfig) / sizeof(quinticConfiguration_t); 	
	printf("QUINTIC STATS \r\n");
	for(i = 0; i < numberOfQuintics; i++)
	{
		printf("Q%d:\r\n", i);
		printf("	Corrupt Packets: %d\r\n", quinticConfig[i].corruptPacketCnt);
		printf("	Dropped Bytes:   %d\r\n", drv_uart_getDroppedBytes(quinticConfig[i].uartDevice));
		vTaskDelay(1);
	}
	printf("IMU STATS \r\n");
	for(i = 0; i < numberOfImus; i++)
	{		
		printf("IMU%d:\r\n", imuConfig[i].imuId);
		printf("	IMU Present: %d\r\n", imuConfig[i].imuPresent);
		printf("	IMU Connected: %d\r\n", imuConfig[i].imuConnected);
		vTaskDelay(1);
		printf("	Dropped Packets: %d\r\n", imuConfig[i].stats.droppedPackets);
		printf("	Average Rx interval(ticks): %d\r\n",imuConfig[i].stats.avgPacketTime);
		printf("	Packet Rx Count:   %d\r\n", imuConfig[i].stats.packetCnt);
		vTaskDelay(1);
	}
	printf("Total Bytes Written: %d\r\n", totalBytesWritten); 
	vTaskDelay(1);
	printf("Total Frames Written: %d \r\n", totalFramesWritten); 
	int queuedMessages = uxQueueMessagesWaiting(queue_dataHandler);
	printf("Queued Messages: %d\r\n", queuedMessages); 
	printf("--- task ## %u", (unsigned int)uxTaskGetNumberOfTasks());	
	vTaskList((signed portCHAR *)runTimeStats);
	printf(runTimeStats);
}

/***********************************************************************************************
 * processCommand(char* command, size_t cmdSize)
 * @brief A general Command processor which receives commands from Serial terminal and executes them
 * @param char* command, size_t cmdSize
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error 
 ***********************************************************************************************/
status_t processCommand(char* command, size_t cmdSize)
{
	status_t status = STATUS_PASS; 
	if(strncmp(command, "SDCardTest\r\n",cmdSize) == 0)
	{
		printf("received the SD card test command\r\n");				
	}
	else if(strncmp(command, "dataBoardGpioTest\r\n",cmdSize) == 0)
	{
		printf("received the GPIO test command\r\n");
	}
	else if(strncmp(command, "BLE Test\r\n",cmdSize) == 0)
	{
		printf("received the GPIO test command\r\n",cmdSize);
	}
	else if(strncmp(command, "StartImus\r\n",cmdSize) == 0)
	{
		task_quintic_startRecording(&quinticConfig[0]);
		task_quintic_startRecording(&quinticConfig[1]);
		task_quintic_startRecording(&quinticConfig[2]);
		task_fabSense_start(&fsConfig); 
		printf("start command Issued\r\n"); 	
		enableRecording = true; 
	}	
	else if(strncmp(command, "StopImus\r\n",cmdSize) == 0)
	{		
		task_quintic_stopRecording(&quinticConfig[0]);
		task_quintic_stopRecording(&quinticConfig[1]);
		task_quintic_stopRecording(&quinticConfig[2]);	
		task_fabSense_stop(&fsConfig); 
		printf("stop command issued\r\n"); 	
		enableRecording = false; 
	}
	else if(strncmp(command, "setRst2Low\r\n",cmdSize) == 0)
	{
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST2, DRV_GPIO_PIN_STATE_LOW);
		printf("Pin set low\r\n");
		enableRecording = false;
	}	
	else if(strncmp(command, "setRst2High\r\n",cmdSize) == 0)
	{
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST2, DRV_GPIO_PIN_STATE_HIGH);
		printf("Pin set high\r\n");
		enableRecording = false;
	}
	else if(strncmp(command, "rstBLE\r\n",cmdSize) == 0)
	{
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST3, DRV_GPIO_PIN_STATE_LOW);
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST1, DRV_GPIO_PIN_STATE_LOW);
		vTaskDelay(50);
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST3, DRV_GPIO_PIN_STATE_HIGH);
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST1, DRV_GPIO_PIN_STATE_HIGH);
		printf("Pin reset\r\n");
		enableRecording = false;
	}	
	else if(strncmp(command, "disableUARTs\r\n",cmdSize) == 0)
	{
		drv_uart_deInit(&uart1Config);
		drv_uart_deInit(&usart0Config);
		drv_uart_deInit(&usart1Config);
		drv_gpio_ConfigureBLEForProgramming(); 
		printf("UARTs set as High impedance\r\n");
		enableRecording = false;
	}	
	else if(strncmp(command, "flushUarts\r\n",cmdSize) == 0)
	{
		drv_uart_flushRx(&usart1Config);
		drv_uart_flushRx(&uart0Config);
		drv_uart_flushRx(&usart0Config);
	}
	else if(strncmp(command,"getStats\r\n", cmdSize) == 0)
	{
		printStats(); 
	}	
	else
	{
		printf("Received unknown command: %s \r\n", command);
		status = STATUS_PASS; 
	}
	return status;	
}

/**
 * \brief This task, when started will loop back \r\n terminated strings
 */
static void task_serialReceiveTest(void *pvParameters)
{
	UNUSED(pvParameters);
	int result = 0;
	char buffer[100] = {0};
	int pointer = 0;
	//char val = 0xA5; 
	while(1)
	{
		if(drv_uart_getline(&uart0Config,buffer,sizeof(buffer)) == STATUS_PASS)
		{
			//drv_uart_putString(&uart1Config,buffer); 
			processCommand(buffer,sizeof(buffer)); 
		}		
		vTaskDelay(10);
		//taskYIELD(); 
	}
}

//TEMP REMOVE THIS
extern FIL dataLogFile_obj; 

/**
 * \brief This task is initialized first to initiate the board peripherals and run the initial tests
 */
void TaskMain(void *pvParameters)
{
	int retCode = 0; 
	UNUSED(pvParameters);
	/*	Create a Semaphore to pass between tasks	*/
	//vSemaphoreCreateBinary(DebugLogSemaphore);
	powerOnInit();

	retCode = xTaskCreate(task_quinticHandler, "Q1", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[0], TASK_QUINTIC_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to create Q1 task code %d\r\n", retCode);
	}
	//retCode = xTaskCreate(task_quinticHandler, "Q2", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[1], TASK_QUINTIC_STACK_PRIORITY, NULL );
	//if (retCode != pdPASS)
	//{
		//printf("Failed to create Q2 task code %d\r\n", retCode);
	//}
	retCode = xTaskCreate(task_quinticHandler, "Q3", TASK_QUINTIC_STACK_SIZE, (void*)&quinticConfig[2], TASK_QUINTIC_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to create Q3 task code %d\r\n", retCode);
	}
	
	retCode = xTaskCreate(task_fabSenseHandler, "FS", TASK_FABSENSE_STACK_SIZE,(void*)&fsConfig, TASK_FABSENSE_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to fabric sense task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_serialReceiveTest, "cmd", TASK_SERIAL_RECEIVE_STACK_SIZE,NULL, TASK_SERIAL_RECEIVE_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to Serial handler task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_dataHandler, "DH", TASK_DATA_HANDLER_STACK_SIZE, NULL, TASK_DATA_HANDLER_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to create data handler task code %d\r\n", retCode);
	}	
	retCode = xTaskCreate(task_sdCardHandler, "SD", TASK_SD_CARD_WRITE_STACK_SIZE, NULL, TASK_SD_CARD_WRITE_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to sd card task code %d\r\n", retCode);
	}
	retCode = xTaskCreate(task_stateMachineHandler, "SM", TASK_STATE_MACHINE_STACK_SIZE, NULL, TASK_STATE_MACHINE_PRIORITY, NULL );
	if (retCode != pdPASS)
	{
		printf("Failed to sd card task code %d\r\n", retCode);
	}
	
	printf("Program start\r\n");
	uint8_t interval = 0;
	for (;;) 
	{
		/*	Hardware Test routine	*/
		checkInputGpio();
		
		vTaskDelay(250);
		//res = f_write(&log_file_object,testData ,sizeof(testData), &numBytes);
		//ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
		//res = f_sync(&log_file_object); //sync the file
		//ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		//
		//
		
		
	}
}

/***********************************************************************************************
 * checkInputGpio(void)
 * @brief Check for interrupt flags on every GPIO pins, process them and raise State machine events
 * @param 
 * @return 
 ***********************************************************************************************/
static void checkInputGpio(void)
{
	//TODO maybe the enqueing of event should be done in the interrupts??
	if (drv_gpio_check_Int(DRV_GPIO_PIN_PW_SW) == 1)
	{
		unsigned long PinFlag;
		
		if (toggle == FALSE)
		{
			oldSysTick = sgSysTickCount;
			Pio *p_pio = pio_get_pin_group(DRV_GPIO_ID_PIN_PW_SW);	//peripheral ID
			uint32_t PinMask = pio_get_pin_group_mask(DRV_GPIO_ID_PIN_PW_SW);	//PinMask
			PinFlag = (PIO_IT_RISE_EDGE | PIO_IT_AIME);
			pio_configure_interrupt(p_pio, PinMask, PinFlag);
			toggle = TRUE;
		}
		else
		{
			newSysTick = sgSysTickCount;
			Pio *p_pio = pio_get_pin_group(DRV_GPIO_ID_PIN_PW_SW);	//peripheral ID
			uint32_t PinMask = pio_get_pin_group_mask(DRV_GPIO_ID_PIN_PW_SW);	//PinMask
			uint32_t PioIntMaskA = pio_get_interrupt_mask(PIOA);
			uint32_t PioIntMaskB = pio_get_interrupt_mask(PIOB);
			//uint32_t PioIntMaskC = pio_get_interrupt_mask(PIOC);
			PinFlag = (PIO_IT_FALL_EDGE | PIO_IT_AIME);
			pio_configure_interrupt(p_pio, PinMask, PinFlag);
			toggle = FALSE;
			
			if (newSysTick - oldSysTick >= (5000/portTICK_RATE_MS))
			{
				printf("Sleep mode enabled\r\n");
				task_stateMachine_EnqueueEvent(SYS_EVENT_POWER_SWITCH,0); 
			}
			else
			{
				printf("PW SW pressed\r\n");
			}
			newSysTick = oldSysTick = 0;
		}
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_AC_SW1) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_RECORD_SWITCH,0); 
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_AC_SW2) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_RESET_SWITCH,0); 
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_OC1) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_OVER_CURRENT,1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_OC2) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_OVER_CURRENT,2);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_DC1) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_JACK_DETECT,1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_JC_DC2) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_JACK_DETECT,2);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_LBO) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_LOW_BATTERY,0);
	}
	//no idea what to do with this one...	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_STAT) == 1)
	{
		printf("STAT detected\r\n");
		vTaskDelay(1);
	}	
	if (drv_gpio_check_Int(DRV_GPIO_PIN_SD_CD) == 1)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_SD_CARD_DETECT,0);
	}
}