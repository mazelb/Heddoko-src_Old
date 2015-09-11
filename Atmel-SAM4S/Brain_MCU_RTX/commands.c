

#include <stdio.h>
#include <stdarg.h>
#include <SAM4S.h>
#include "Serial.h"
#include "LED.h"
//#include "BTN.h"
#include "commands.h"
#include "BrainMCU.h"

/*	Scan complete flag and Connection complete flag	*/
uint8_t vScanCmpQ1=0, vConCmpQ1=0;		
uint8_t vScanCmpQ2=0, vConCmpQ2=0;
uint8_t vScanCmpQ3=0, vConCmpQ3=0;

/*
 *****************************************************************************************
 * @brief Initiaize connection with Quintic 1
 * @param None
 * @description 
 *
 * This function initicalizes the connection with the Quintic module and sends the NOD 
 * address, fires the scan and connect command
 *****************************************************************************************
 */
int Q1Init(void)
{

	char vScanSuccess=0, vConSuccess=0, vScanLoopCount=0;
	while(QnAck(Q1)!=1);
	
	printf("Received ACK from Q1\r\n");
	
	SerialPrint(Q1,"Ack\r\n");
	
	SerialPrint(Q1,"begin\r\n");
	while(QnAck(Q1)!=1);
	printf("Begin Ack ");
	
	SerialPrint(Q1,Nod.id[0].Addr);
	while(QnAck(Q1)!=1);
	printf("Add Ack ");
	
	SerialPrint(Q1,Nod.id[1].Addr);
	while(QnAck(Q1)!=1);
	printf("Add Ack ");
	
	SerialPrint(Q1,Nod.id[2].Addr);
	while(QnAck(Q1)!=1);
	printf("Add Ack ");
  
	//Delay(1000);
	
	do
	{
		SerialPrint(Q1,"scan\r\n");
		vScanSuccess=QnScanAck(Q1);
		if(vScanSuccess==1)
		{
			printf("Scan Ack ");
			vScanLoopCount=4;
			vScanCmpQ1=1;
		}
		else if(vScanSuccess==2)
		{			
			printf("Scan Err ");
			vScanLoopCount++;
		}
	}while(vScanLoopCount<=QN_MAX_CONN);
	
	if(vScanLoopCount>=QN_MAX_CONN)
	{
		SerialPrint(Q1,"connect\r\n");
		vConSuccess=QnConAck(Q1);
		if(vConSuccess==1)
		{
			printf("Con Ack ");
			vConCmpQ1=1;
		}
		else if(vConSuccess==2)
			printf("Con Err ");
	}
	else
	{
		printf("Scan incomplete\r\n");
	}
}

/*
 *****************************************************************************************
 * @brief Initiaize connection with Quintic 2
 * @param None
 * @description 
 *
 * This function initicalizes the connection with the Quintic module and sends the NOD 
 * address, fires the scan and connect command
 *****************************************************************************************
 */
int Q2Init(void)
{
	char vScanSuccess=0, vConSuccess=0, vScanLoopCount=0;
	while(QnAck(Q2)!=1);
	
	printf("Received ACK from Q2\r\n");
	
	SerialPrint(Q2,"Ack\r\n");
	
	SerialPrint(Q2,"begin\r\n");
	while(QnAck(Q2)!=1);
	printf("Begin Ack ");
	
	SerialPrint_p(Q2,Nod.id[3].Addr);
	while(QnAck(Q2)!=1);
	printf("Add Ack ");
	
	SerialPrint_p(Q2,Nod.id[4].Addr);
	while(QnAck(Q2)!=1);
	printf("Add Ack ");
	
	SerialPrint_p(Q2,Nod.id[5].Addr);
	while(QnAck(Q2)!=1);
	printf("Add Ack ");
  
	//Delay(1000);
	
	do
	{
		SerialPrint(Q2,"scan\r\n");
		vScanSuccess=QnScanAck(Q2);
		if(vScanSuccess==1)
		{			
			printf("Scan Ack ");
			vScanLoopCount=4;
			vScanCmpQ2=1;
		}
		else if(vScanSuccess==2)
		{			
			printf("Scan Err ");
			vScanLoopCount++;
		}
	}while(vScanLoopCount<=QN_MAX_CONN);
	
	if(vScanLoopCount>=QN_MAX_CONN)
	{
		SerialPrint(Q2,"connect\r\n");
		vConSuccess=QnConAck(Q2);
		if(vConSuccess==1)
		{
			printf("Con Ack ");
			vConCmpQ2=1;
		}
		else if(vConSuccess==2)
		{
			printf("Con Err ");
			printf("Con incomplete\r\n");
		}
	}
	else
	{	
		printf("Scan incomplete\r\n");
	}
}

/*
 *****************************************************************************************
 * @brief Initiaize connection with Quintic 3
 * @param None
 * @description 
 *
 * This function initicalizes the connection with the Quintic module and sends the NOD 
 * address, fires the scan and connect command
 *****************************************************************************************
 */
int Q3Init(void)
{
	char vScanSuccess=0, vConSuccess=0, vScanLoopCount=0;
	while(QnAck(Q3)!=1);
	
	printf("Received ACK from Q3\r\n");
	
	SerialPrint(Q3,"Ack\r\n");
	
	SerialPrint(Q3,"begin\r\n");
	while(QnAck(Q3)!=1);
	printf("Begin Ack ");
	
	SerialPrint_p(Q3,Nod.id[6].Addr);
	while(QnAck(Q3)!=1);
	printf("Add Ack ");
	
	SerialPrint_p(Q3,Nod.id[7].Addr);
	while(QnAck(Q3)!=1);
	printf("Add Ack ");
	
	SerialPrint_p(Q3,Nod.id[8].Addr);
	while(QnAck(Q3)!=1);
	printf("Add Ack ");
  
	//Delay(1000);
	
	do
	{
		SerialPrint(Q3,"scan\r\n");
		vScanSuccess=QnScanAck(Q3);
		if(vScanSuccess==1)
		{			
			printf("Scan Ack ");
			vScanLoopCount=4;
			vScanCmpQ3=1;
		}
		else if(vScanSuccess==2)
		{			
			printf("Scan Err ");
			vScanLoopCount++;
		}
	}while(vScanLoopCount<=QN_MAX_CONN);
	
	if(vScanLoopCount>=QN_MAX_CONN)
	{
		SerialPrint(Q3,"connect\r\n");
		vConSuccess=QnConAck(Q3);
		if(vConSuccess==1)
		{
			printf("Con Ack ");
			vConCmpQ3=1;
		}
		else if(vConSuccess==2)
			printf("Con Err ");
	}
	else
	{	
		printf("Scan incomplete\r\n");
	}
}

/*
 *****************************************************************************************
 * @brief Trigger the start command
 * @param None
 * @description 
 *
 * This function triggers the start command to receive notification data from the Quintics.
 *****************************************************************************************
 */
int QnStart(void)
{
		printf("Writing to receive notif\r\n");
	if((vScanCmpQ1==1)&&(vConCmpQ1==1))
		SerialPrint(Q1,"start\r\n");		//Q1
	
	if((vScanCmpQ2==1)&&(vConCmpQ2==1))
		SerialPrint(Q2,"start\r\n");		//Q2
	
	if((vScanCmpQ3==1)&&(vConCmpQ3==1))
		SerialPrint(Q3,"start\r\n");		//Q3
	
}

/*
 *****************************************************************************************
 * @brief Trigger the stop command
 * @param None
 * @description 
 *
 * This function triggers the stop command to stop receive notification data from the Quintics.
 *****************************************************************************************
 */
int QnStop(void)
{
		printf("Writing to stop notif\r\n");
		SerialPrint(Q1,"stop\r\n");		//Q1
		SerialPrint(Q2,"stop\r\n");		//Q2
		SerialPrint(Q3,"stop\r\n");		//Q3
	
}

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 10 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) 
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}
