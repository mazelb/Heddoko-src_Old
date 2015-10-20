/*
 * task_quinticInterface.c
 *
 * Created: 9/18/2015 10:49:35 AM
 *  Author: sean
 */
/**
 * @file  task_quinticInterface.c
 * @brief handler task and associated functions for management of the Quintic modules
 * @author Heddoko
 * @note 
 *
 * @copy Heddoko (2015)
 */ 
#include "task_quinticInterface.h"
#include "task_dataProcessor.h"
#include "task_stateMachine.h"
#include <string.h>

//#define DEBUG_DUMMY_DATA 
extern xQueueHandle queue_dataHandler, queue_stateMachineEvents;
extern uint16_t packetReceivedMask; 
extern bool enableRecording; 
//static declarations
static status_t sendString(drv_uart_config_t* uartConfig, char* cmd);
static status_t getAck(drv_uart_config_t* uartConfig);
static status_t scanForImus(quinticConfiguration_t* qConfig); 
static status_t connectToImus(quinticConfiguration_t* qConfig);
static void createDummyData(int imuId, int seqNumber, int numVals, char* buf, size_t bufSize);
extern uint32_t sgSysTickCount;
extern drv_uart_config_t uart0Config;

/***********************************************************************************************
 * task_quinticHandler(void *pvParameters)
 * @brief The main task for a quintic module, associate UART has to be initialized before calling this
 *	function. 
 * @param pvParameters, void pointer to structure containing quintic and imu configuration. 
 * @return void
 ***********************************************************************************************/
void task_quinticHandler(void *pvParameters)
{
	//cast void pointer to quintic config type. 
	quinticConfiguration_t *qConfig = (quinticConfiguration_t*)pvParameters; 
	//initialize all structures ,check everything is alright
	int i = 0;
	//initialize all buffers and pointers to zero
	for(i = 0 ; i < QUINTIC_MAX_NUMBER_OF_IMUS ; i++)
	{
		//qConfig->imuArray[i]->bufferEnd = 0;
		//qConfig->imuArray[i]->bufferHead = 0;
		//memset(qConfig->imuArray[i]->packetBuffer, 0 , IMU_BUFFER_SIZE*IMU_PACKET_LENGTH); 	
		qConfig->imuArray[i]->stats.avgPacketTime = 0;
		qConfig->imuArray[i]->stats.droppedPackets = 0;
		qConfig->imuArray[i]->stats.lastPacketTime = sgSysTickCount;
		qConfig->imuArray[i]->stats.maxPacketTime = 0;
		qConfig->imuArray[i]->stats.packetCnt = 0;
	}
	
	if(drv_uart_isInit(qConfig->uartDevice) != STATUS_PASS)
	{
		//the UART driver has not been initialized, this should be done before starting this task!
		return; 
	}
	//cycle power on quintic module
	//Has specific IO for each quintic BLE_RST1 - BLE_RST3 TODO add this to configuration structure. 
	//Cycle power for all the IMUs? This is only done when the start command is received. 
	//send all the initialization garbage
	#ifdef DEBUG_DUMMY_DATA
	packetReceivedMask |= (1<<qConfig->imuArray[0]->imuId);
	packetReceivedMask |= (1<<qConfig->imuArray[1]->imuId);
	packetReceivedMask |= (1<<qConfig->imuArray[2]->imuId);
	#else
	//task_quintic_initializeImus(qConfig);
	#endif 
	dataPacket_t packet; 
	packet.type = DATA_PACKET_TYPE_IMU; 
	//main loop of task, this is where we request information and store it. 
	char buf[CMD_RESPONSE_BUF_SIZE] = {0}; 
	int packetNumber = 0;
	uint32_t timeNow = 0;
	int index = -1; 
	qConfig->isinit = true;
	while(1)
	{

		#ifndef DEBUG_DUMMY_DATA
		if(getCurrentState() == SYS_STATE_RECORDING)
		{		
			if(drv_uart_getlineTimed(qConfig->uartDevice, buf, CMD_RESPONSE_BUF_SIZE, 400) == STATUS_PASS)
			{
				if(strncmp(buf, "00", 2) == 0)
				{
					index = 0;
				}
				else if(strncmp(buf, "11", 2) == 0)
				{
					index = 1;
				}
				else if(strncmp(buf, "22", 2) == 0)	
				{				
					index = 2;
				}
				else
				{				
					//this is a corrupt packet, increment the count. 
					qConfig->corruptPacketCnt++;
					//vTaskDelay(10);
				}
				//validate the index
				if(index >= 0 && index <= 2)
				{
					packet.imuId = qConfig->imuArray[index]->imuId; 
					packet.imuIndex = packet.imuId; 
					qConfig->imuArray[index]->stats.packetCnt++; //increment the packets received count
					timeNow = sgSysTickCount; 
					//calculate the new running average packet time --> (average + (last packet received time - current Time)/2)
					qConfig->imuArray[index]->stats.avgPacketTime = (qConfig->imuArray[index]->stats.avgPacketTime + ( timeNow - qConfig->imuArray[index]->stats.lastPacketTime ))>>1;
					qConfig->imuArray[index]->stats.lastPacketTime = timeNow; 
					packetReceivedMask |= (1<<qConfig->imuArray[index]->imuId); 				
					memcpy(packet.data,buf+2, 12+1);				
					if(queue_dataHandler != NULL)
					{
						if(xQueueSendToBack( queue_dataHandler,( void * ) &packet,5) != TRUE)
						{
							//error failed to queue the packet.
							qConfig->imuArray[index]->stats.droppedPackets++; 
							//vTaskDelay(10);
						}													
					}				
				}
			}
		}
		//taskYIELD(); 
		vTaskDelay(5);
		#else

		#endif
					
		
	}	
}

/***********************************************************************************************
 * task_quintic_initializeImus(void *pvParameters)
 * @brief Start the initializing process for IMUs.
 * @param pvParameters, void pointer to structure containing quintic and imu configuration. 
 * @return void
 ***********************************************************************************************/
void task_quintic_initializeImus(void *pvParameters)
{
	quinticConfiguration_t *qConfig = (quinticConfiguration_t*)pvParameters;
	status_t result = STATUS_PASS;
	status_t scanSuccess=STATUS_FAIL, connSuccess=STATUS_FAIL;
	int vScanLoopCount=0;
	vTaskDelay(1000);
	//reset the quintic here...
	drv_uart_flushRx(qConfig->uartDevice);	//flush the uart first
	drv_gpio_setPinState(qConfig->resetPin,DRV_GPIO_PIN_STATE_LOW);
	vTaskDelay(100);
	drv_gpio_setPinState(qConfig->resetPin,DRV_GPIO_PIN_STATE_HIGH);
	//wait for first ACK
	result = getAck(qConfig->uartDevice);
	drv_uart_flushRx(qConfig->uartDevice);	//flush the uart first
	vTaskDelay(10);
	//get quintic ready to receive the
	sendString(qConfig->uartDevice,QCMD_BEGIN);
	vTaskDelay(10);
	result |= getAck(qConfig->uartDevice);
	
	//send MAC addresses for each NOD
	int i = 0;
	for(i=0;i<qConfig->expectedNumberOfNods; i++)
	{
		sendString(qConfig->uartDevice,qConfig->imuArray[i]->macAddress);
		vTaskDelay(10);
		result |= getAck(qConfig->uartDevice);
	}
	sendString(qConfig->uartDevice,"end\r\n");
	vTaskDelay(10);
	result |= getAck(qConfig->uartDevice);
	scanSuccess = scanForImus(qConfig);
	if(scanSuccess == STATUS_PASS)
	{
		connSuccess = connectToImus(qConfig);
	}
	if(connSuccess != STATUS_PASS)
	{
		scanSuccess = scanForImus(qConfig);
		if(scanSuccess == STATUS_PASS)
		{
			connSuccess = connectToImus(qConfig);
		}
	}
	if(scanSuccess == STATUS_PASS && connSuccess == STATUS_PASS)
	{
		
		printf("connected to IMUs %d, %d, %d\r\n",qConfig->imuArray[0]->imuId,qConfig->imuArray[1]->imuId,qConfig->imuArray[2]->imuId);
		eventMessage_t msg = {.sysEvent = SYS_EVENT_RESET_COMPLETE, .data = qConfig->imuArray[0]->imuId};
		if(queue_stateMachineEvents != NULL)
		{
			xQueueSendToBack(queue_stateMachineEvents, &msg,5);
		}	
	}
	else
	{
		printf("Failed connection to IMUs %d, %d, %d\r\n",qConfig->imuArray[0]->imuId,qConfig->imuArray[1]->imuId,qConfig->imuArray[2]->imuId);
		result = STATUS_FAIL; 
	}
	
	//return the result;
	return result;

}


/***********************************************************************************************
 * task_quintic_startRecording(quinticConfiguration_t* qConfig)
 * @brief Send the start command to the IMUs to request Notif data from them
 * @param quinticConfiguration_t* qConfig 
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error
 ***********************************************************************************************/
status_t task_quintic_startRecording(quinticConfiguration_t* qConfig)
{
	//send the start command. 	
	sendString(qConfig->uartDevice, "start\r\n");
	return STATUS_PASS; 
}

/***********************************************************************************************
 * task_quintic_stopRecording(quinticConfiguration_t* qConfig)
 * @brief Send the stop command to the IMUs to stop Notif data from them
 * @param quinticConfiguration_t* qConfig 
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error
 ***********************************************************************************************/
status_t task_quintic_stopRecording(quinticConfiguration_t* qConfig)
{
	//send the stop
	sendString(qConfig->uartDevice, "stop\r\n");
	//wait for a bit
	vTaskDelay(2);
	//flush the Rx buffer, it'll still have crap in it
	drv_uart_flushRx(qConfig->uartDevice);
	return STATUS_PASS;
}
//static functions

/***********************************************************************************************
 * sendString(drv_uart_config_t* uartConfig, char* cmd)
 * @brief Send a string to requested serial port
 * @param drv_uart_config_t* uartConfig, char* cmd 
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error
 ***********************************************************************************************/
static status_t sendString(drv_uart_config_t* uartConfig, char* cmd)
{
	int len = strlen(cmd); 
	int i = 0; 
	for(;i<len;)
	{
		if(drv_uart_putChar(uartConfig,cmd[i]) == STATUS_PASS) //returns pass only when char sent
		{
			i++;
		}
	}
}

/***********************************************************************************************
 * getAck(drv_uart_config_t* uartConfig)
 * @brief Look for an acknowledge from a Quintic
 * @param drv_uart_config_t* uartConfig
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error
 ***********************************************************************************************/
static status_t getAck(drv_uart_config_t* uartConfig)
{
	status_t result = STATUS_PASS; 
	char buf[CMD_RESPONSE_BUF_SIZE] = {0}; //should move to static buffer for each quintic?
	result = drv_uart_getline(uartConfig, buf,CMD_RESPONSE_BUF_SIZE);
	if(result == STATUS_PASS)
	{
		if(strcmp(buf,QCMD_QN_ACK) != 0)
		{
			result = STATUS_FAIL;
		}
	}
	return result; 
}

/***********************************************************************************************
 * getResponse(drv_uart_config_t* uartConfig, char* expectedResponse)
 * @brief Look for a response from a Quintic
 * @param drv_uart_config_t* uartConfig, char* expectedResponse
 * @return STATUS_PASS if successful, STATUS_FAIL if there is an error
 ***********************************************************************************************/
static status_t getResponse(drv_uart_config_t* uartConfig, char* expectedResponse)
{
	status_t result = STATUS_PASS;
	char buf[CMD_RESPONSE_BUF_SIZE] = {0}; //should move to static buffer for each quintic?
	if(drv_uart_getline(uartConfig, buf,CMD_RESPONSE_BUF_SIZE) == STATUS_PASS)
	{
		if(strcmp(buf,expectedResponse) != 0)
		{
			result = STATUS_FAIL;
		}
	}
	return result;
}

/***********************************************************************************************
 * createDummyData(int imuId, int seqNumber, int numVals, char* buf, size_t bufSize)
 * @brief Create dummy data of IMUs
 * @param int imuId, int seqNumber, int numVals, char* buf, size_t bufSize
 * @return void
 ***********************************************************************************************/
static void createDummyData(int imuId, int seqNumber, int numVals, char* buf, size_t bufSize)
{
	int i = 0; 
	int numChars = 0; 
	for(i =0 ; i < numVals; i++)
	{
		numChars += sprintf(buf+numChars,"%02dYY%04dRRR%d",i,seqNumber,imuId);
		if(numChars >= bufSize)
		{
			break;
		}
	}
}

/***********************************************************************************************
 * scanForImus(quinticConfiguration_t* qConfig)
 * @brief Issue Scan command to Quintics and check the response. If it fails it tries two more times
 * @param quinticConfiguration_t* qConfig
 * @return void
 ***********************************************************************************************/
static status_t scanForImus(quinticConfiguration_t* qConfig)
{
	status_t status = STATUS_FAIL; 
	char buf[150] = {0}; 
	int i = 0;	
	char* bufPtr = buf; 
	int vScanLoopCount = 0;
	int presentImuCount = 0;
	do
	{
		sendString(qConfig->uartDevice,QCMD_SCAN); //send the scan command
		vTaskDelay(1);
		if(drv_uart_getline(qConfig->uartDevice,buf, sizeof(buf)) == STATUS_PASS)
		{
			sendString(&uart0Config,buf);
			if(strncmp(buf,"ScanResp",8) == 0)
			{
				bufPtr = buf + 8; 
				presentImuCount = 0;
				for(i=0;i<5;i++)
				{
					if(bufPtr[i] == '1')
					{
						qConfig->imuArray[i]->imuPresent = 1; 	
						presentImuCount++;					
					}
					else
					{
						qConfig->imuArray[i]->imuPresent = 0; 
					}			
				}
				if(presentImuCount >= qConfig->expectedNumberOfNods)
				{
					status = STATUS_PASS; 
					break; 
				}
			}
		}		
		vTaskDelay(10);
		vScanLoopCount++;
	}while(vScanLoopCount<=QN_MAX_CONN);	
	return status; 
}

/***********************************************************************************************
 * connectToImus(quinticConfiguration_t* qConfig)
 * @brief Issue Connect command to Quintics and check the response. If it fails it tries two more times
 * @param quinticConfiguration_t* qConfig
 * @return void
 ***********************************************************************************************/
static status_t connectToImus(quinticConfiguration_t* qConfig)
{
	status_t status = STATUS_FAIL;
	char buf[150] = {0};
	int i = 0;
	char* bufPtr = buf;
	int vConnectionLoopCount = 0;
	int connectedImuCount = 0;

	sendString(qConfig->uartDevice,QCMD_CONNECT); //send the connect command
	vTaskDelay(1);
	if(drv_uart_getline(qConfig->uartDevice,buf, sizeof(buf)) == STATUS_PASS)
	{
		if(strncmp(buf,"ConnResp",8) == 0)
		{
			bufPtr = buf + 8;
			connectedImuCount = 0;
			for(i=0;i<5;i++)
			{
				if(bufPtr[i] == '1')
				{
					qConfig->imuArray[i]->imuConnected = 1;
					connectedImuCount++;
				}
				else
				{
					qConfig->imuArray[i]->imuConnected = 0;
				}
			}
			if(connectedImuCount >= qConfig->expectedNumberOfNods)
			{
				status = STATUS_PASS;
			}
		}
	}
	return status;
}

