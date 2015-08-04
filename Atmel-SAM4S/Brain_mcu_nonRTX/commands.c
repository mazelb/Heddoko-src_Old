

#include <stdio.h>
#include <stdarg.h>
#include <SAM4S.h>
#include "Serial.h"
#include "LED.h"
#include "BTN.h"
#include "commands.h"
#include "BrainMCU.h"

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
int Q1_init(void){

	while(qn_ack(1)!=1);
	
	printf("Received ACK from Q1\r\n");
	
	ser_print(1,"Ack\r\n");
	
	ser_print(1,"begin\r\n");
	while(qn_ack(1)!=1);
	printf("Begin Ack ");
	
	ser_print(1,"A0E5E9001398\r\n");
	while(qn_ack(1)!=1);
	printf("Add Ack ");
	
	ser_print(1,"A0E5E9001498\r\n");
	while(qn_ack(1)!=1);
	printf("Add Ack ");
	
	ser_print(1,"A0E5E9000387\r\n");
	while(qn_ack(1)!=1);
	printf("Add Ack ");
  
	Delay(1000);
	
	ser_print(1,"scan\r\n");
	while(qn_scan_ack(1)!=1);
	printf("Scan Ack ");
	
	ser_print(1,"connect\r\n");
	while(qn_con_ack(1)!=1);
	printf("Connect Ack ");
	
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
int Q2_init(void){

	
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
int Q3_init(void){

	
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
void Qn_start(void){

	ser_print(1,"start\r\n");	//Q1
	//Q2
	//Q3
}


volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 10 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}
