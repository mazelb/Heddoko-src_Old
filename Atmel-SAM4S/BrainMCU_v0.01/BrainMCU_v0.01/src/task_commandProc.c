/*
 * task_commandProc.c
 *
 * Created: 10/28/2015 9:46:58 AM
 *  Author: sean
 */ 
#include <string.h>
#include <stdio.h>
#include "settings.h"
#include "task_commandProc.h"
#include "task_stateMachine.h"
#include "task_quinticInterface.h"
#include "task_sdCardWrite.h"
#include "task_fabricSense.h"
#include "rtc.h"

//external values
extern xQueueHandle queue_dataHandler;
extern uint32_t totalBytesWritten;
extern uint32_t totalFramesWritten;
extern unsigned long sgSysTickCount;
extern quinticConfiguration_t quinticConfig[];
extern imuConfiguration_t imuConfig[];
extern fabricSenseConfig_t fsConfig;
extern drv_uart_config_t uart0Config;
extern drv_uart_config_t uart1Config;
extern drv_uart_config_t usart0Config;
extern drv_uart_config_t usart1Config;
extern brainSettings_t brainSettings;


volatile bool enableRecording = false;
volatile bool bluetoothConnected = false;
volatile bool debugPrintsEnabled = false; 
char runTimeStats[50*10] = {0}; 
commandProcConfig_t* config;	

//static function forward declarations	
static void printStats();
static status_t processCommand(char* command, size_t cmdSize); 
static void setTimeFromString(char* dateTime); 
static char* getTimeString();	
/**
 * \brief This task, when started will loop back \r\n terminated strings
 */
void task_commandHandler(void *pvParameters)
{
	UNUSED(pvParameters);
	int result = 0;
	char buffer[100] = {0};
	int pointer = 0;
	config = (commandProcConfig_t*)pvParameters; 
	//configure the bluetooth dongle, if brand new
	//TODO add a check to make sure it needs to be reconfigured first. 
	//drv_uart_deInit(config->uart);
	//drv_uart_init(&uartInitialization);
	//drv_uart_putString(&uartInitialization,"AT+NAMEHEDDOKO01\r\n");
	//vTaskDelay(100);
	//drv_uart_putString(&uartInitialization,"AT+BAUD8\r\n");
	//vTaskDelay(100);
	//drv_uart_init(config->uart);
	
	//char val = 0xA5; 
	while(1)
	{
		if(drv_uart_getline(config->uart,buffer,sizeof(buffer)) == STATUS_PASS)
		{
			//drv_uart_putString(&uart1Config,buffer); 
			processCommand(buffer,sizeof(buffer)); 
		}		
		vTaskDelay(10);
		//taskYIELD(); 
	}
}



/***********************************************************************************************
 * printStats()
 * @brief Prints the Stats of the system to serial terminal
 * @param 
 * @return 
 ***********************************************************************************************/
static void printStats()
{
	int i = 0; 
	size_t numberOfImus = 9;//sizeof(imuConfig) / sizeof(imuConfiguration_t);
	size_t numberOfQuintics = 3;//sizeof(quinticConfig) / sizeof(quinticConfiguration_t); 	
	printString("QUINTIC STATS \r\n");
	for(i = 0; i < numberOfQuintics; i++)
	{
		printf("Q%d:\r\n", i);
		printf("	Corrupt Packets: %d\r\n", quinticConfig[i].corruptPacketCnt);
		printf("	Dropped Bytes:   %d\r\n", drv_uart_getDroppedBytes(quinticConfig[i].uartDevice));
		vTaskDelay(1);
	}
	printString("IMU STATS \r\n");
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
static status_t processCommand(char* command, size_t cmdSize)
{
	status_t status = STATUS_PASS; 
	if(strncmp(command, "SDCardTest\r\n",cmdSize) == 0)
	{
		printString("received the SD card test command\r\n");				
	}
	else if(strncmp(command, "AT-AB ",6) == 0)
	{
		//this is a bluetooth module command
		if(strncmp(command+6,"ConnectionUp",12) == 0)
		{
			bluetoothConnected = true; 
		}
		else if(strncmp(command+6,"ConnectionDown\r\n",cmdSize -6) == 0)
		{
			bluetoothConnected = false; 
		}
	}
	else if(strncmp(command, "dataBoardGpioTest\r\n",cmdSize) == 0)
	{
		printString("received the GPIO test command\r\n");
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
		printString("start command Issued\r\n"); 	
		enableRecording = true; 
	}	
	else if(strncmp(command, "StopImus\r\n",cmdSize) == 0)
	{		
		task_quintic_stopRecording(&quinticConfig[0]);
		task_quintic_stopRecording(&quinticConfig[1]);
		task_quintic_stopRecording(&quinticConfig[2]);	
		task_fabSense_stop(&fsConfig); 
		printString( "stop command issued\r\n"); 	
		enableRecording = false; 
	}
	else if (strncmp(command, "Record\r\n",cmdSize) == 0)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_RECORD_SWITCH, 0x00);
		printString("Ack\r\n");
	}
	else if (strncmp(command, "Reset\r\n",cmdSize) == 0)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_RESET_SWITCH, 0x00);
		printString("Ack\r\n");
	}
	else if (strncmp(command, "Power\r\n",cmdSize) == 0)
	{
		task_stateMachine_EnqueueEvent(SYS_EVENT_POWER_SWITCH,0);
		printString( "ACK\r\n");
	}
	else if(strncmp(command, "GetState\r\n",cmdSize) == 0)
	{
		switch(getCurrentState())
		{
			case SYS_STATE_IDLE: 
			printString("Idle\r\n"); 
			break; 
			case SYS_STATE_RESET:
			printString("Reset\r\n");
			break;
			case SYS_STATE_RECORDING:
			printString("Recording\r\n");
			break;
			case SYS_STATE_ERROR:
			printString("Error\r\n");
			break;
			default: 
			printString("UnDef\r\n");
			break;			
		}
	}
	else if(strncmp(command, "AutoOff",7) == 0)
	{
		if(*(command+7) == '1')
		{
			brainSettings.autoTurnOff = true; 
		}
		else
		{
			brainSettings.autoTurnOff = false; 
		}
		printString( "ACK\r\n");
	}	
	else if(strncmp(command, "DebugEn",7) == 0)
	{
		if(*(command+7) == '1')
		{
			brainSettings.debugPrintsEnabled = true; 
		}
		else
		{
			brainSettings.debugPrintsEnabled = false; 
		}
		printString( "ACK\r\n");
	}				
	else if (strncmp(command, "CheckRssi\r\n",cmdSize) == 0)
	{
		printString("RSSI level:\r\n");
		task_quintic_checkRssiLevel(&quinticConfig[0]);
		task_quintic_checkRssiLevel(&quinticConfig[1]);
		task_quintic_checkRssiLevel(&quinticConfig[2]);
	}	
	else if(strncmp(command, "setRst2Low\r\n",cmdSize) == 0)
	{
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST2, DRV_GPIO_PIN_STATE_LOW);
		printString("Pin set low\r\n");
		enableRecording = false;
	}	
	else if(strncmp(command, "setRst2High\r\n",cmdSize) == 0)
	{
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST2, DRV_GPIO_PIN_STATE_HIGH);
		printString("Pin set high\r\n");
		enableRecording = false;
	}
	else if(strncmp(command, "rstBLE\r\n",cmdSize) == 0)
	{
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST3, DRV_GPIO_PIN_STATE_LOW);
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST1, DRV_GPIO_PIN_STATE_LOW);
		vTaskDelay(50);
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST3, DRV_GPIO_PIN_STATE_HIGH);
		drv_gpio_setPinState(DRV_GPIO_PIN_BLE_RST1, DRV_GPIO_PIN_STATE_HIGH);
		printString("Pin reset\r\n");
		enableRecording = false;
	}	
	else if(strncmp(command, "disableUARTs\r\n",cmdSize) == 0)
	{
		drv_uart_deInit(&uart1Config);
		drv_uart_deInit(&usart0Config);
		drv_uart_deInit(&usart1Config);
		drv_gpio_ConfigureBLEForProgramming(); 
		printString("UARTs set as High impedance\r\n");
		enableRecording = false;
	}	
	else if(strncmp(command, "setTime",7) == 0)
	{
		if(strlen(command) >= 24)
		{
			setTimeFromString(command+7); 
		}
	}
	else if(strncmp(command, "?\r\n",3) == 0)
	{
		printf("HEDDOKO BRAIN PACK %s\r\n", VERSION); 
		printf("BUILD DATE: %s %s\r\n", __DATE__,__TIME__);
		printf("SUIT NUMBER: %s\r\n",brainSettings.suitNumber); 
		printf("SUIT TIME: %s \r\n",getTimeString());  		
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
	else if(strncmp(command,"sendConnect\r\n", cmdSize) == 0)
	{
		drv_uart_putString(quinticConfig[0].uartDevice,"connect\r\n");
		drv_uart_putString(quinticConfig[2].uartDevice,"connect\r\n");
	}	
	else if(strncmp(command,"HardReset\r\n", cmdSize) == 0)	
	{
		rstc_start_software_reset(RSTC);
	}
	else if (strncmp(command,"fsp", 3) == 0)
	{
		//drv_uart_putString(&uart0Config,command+3);
		//if(drv_uart_getlineTimed(&uart0Config,stringBuf,200,1000) == STATUS_PASS)
		//{
			//printString(stringBuf);
		//}
		//else
		//{
			//printString("no Response from Fabric Sense\r\n");
		//}
		
	}
	else if (strncmp(command,"debugPackets\r\n", cmdSize) == 0)
	{
		//turns on the zeroing of missing packets in the output data. 
		brainSettings.debugPackets = true; 
	}			
	else
	{
		printf("Received unknown command: %s \r\n", command);
		status = STATUS_PASS; 
	}
	return status;	
}

static void setTimeFromString(char* dateTime)
{
	uint32_t year, month, day, dow; //dow is day of week (1-7)
	uint32_t hour, minute, second; 
	if(sscanf(dateTime,"%d-%d-%d-%d-%d:%d:%d\r\n", &year, &month, &day, &dow, &hour, &minute, &second ) == 7)
	{
		// we successfully parsed the data, set the time and date
		rtc_set_time(RTC,hour,minute,second); 
		rtc_set_date(RTC,year,month,day,dow); 
		printString("ACK\r\n");
	}
	else
	{
		printString("NACK\r\n");
	}
}
char timeString[100] = {0}; 
static char* getTimeString()
{
	uint32_t hour, minute, second; 
	rtc_get_time(RTC,&hour,&minute,&second); 
	sprintf(timeString,"%02d:%02d:%02d",hour,minute,second); 
	return timeString; 
} 
void debugPrintString(char* str)
{
	char length = 0;
	//if(debugPrintsEnabled)
	//{
		drv_uart_putString((config->uart), str);
	//}		
	length = strlen(str);
	task_debugLogWriteEntry(str, length);
}

void printString(char* str)
{
	drv_uart_putString((config->uart), str);
}

void sendPacket(char* buf, size_t length)
{
	drv_uart_putData((config->uart), buf, length);
}