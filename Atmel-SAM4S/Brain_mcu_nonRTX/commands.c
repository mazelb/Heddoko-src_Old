

#include <stdio.h>
#include <stdarg.h>
#include <SAM4S.h>
#include "Serial.h"
#include "LED.h"
#include "BTN.h"
#include "commands.h"
#include "BrainMCU.h"

uint8_t scan_cmp_q1=0, con_cmp_q1=0;
uint8_t scan_cmp_q2=0, con_cmp_q2=0;
uint8_t scan_cmp_q3=0, con_cmp_q3=0;

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

	char flag_s=0, flag_c=0, scan_loop=0;
	while(qn_ack(Q1)!=1);
	
	printf("Received ACK from Q1\r\n");
	
	ser_print(Q1,"Ack\r\n");
	
	ser_print(Q1,"begin\r\n");
	while(qn_ack(Q1)!=1);
	printf("Begin Ack ");
	
	ser_print(Q1,"A0E5E900133B\r\n");
	while(qn_ack(Q1)!=1);
	printf("Add Ack ");
	
	ser_print(Q1,"A0E5E90016E6\r\n");
	while(qn_ack(Q1)!=1);
	printf("Add Ack ");
	
	ser_print(Q1,"A0E5E90012CD\r\n");
	while(qn_ack(Q1)!=1);
	printf("Add Ack ");
  
	Delay(1000);
	
	do{
		ser_print(Q1,"scan\r\n");
		flag_s=qn_scan_ack(Q1);
		if(flag_s==1){
			
			printf("Scan Ack ");
			scan_loop=4;
		}
		else if(flag_s==2){
			
			printf("Scan Err ");
			scan_loop++;
		}
	}while(scan_loop<=QN_MAX_CONN);
	
	if(scan_loop>=QN_MAX_CONN){
		ser_print(Q1,"connect\r\n");
		flag_c=qn_con_ack(Q1);
		if(flag_c==1)
			printf("Con Ack ");
		else if(flag_c==2)
			printf("Con Err ");
	}
	else{
	
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
int Q2_init(void){

	char flag_s=0, flag_c=0, scan_loop=0;
	while(qn_ack(Q2)!=1);
	
	printf("Received ACK from Q1\r\n");
	
	ser_print(Q2,"Ack\r\n");
	
	ser_print(Q2,"begin\r\n");
	while(qn_ack(Q2)!=1);
	printf("Begin Ack ");
	
	ser_print(Q2,"A0E5E900139B\r\n");
	while(qn_ack(Q2)!=1);
	printf("Add Ack ");
	
	ser_print(Q2,"A0E5E9001717\r\n");
	while(qn_ack(Q2)!=1);
	printf("Add Ack ");
	
	ser_print(Q2,"A0E5E90016DD\r\n");
	while(qn_ack(Q2)!=1);
	printf("Add Ack ");
  
	Delay(1000);
	
	do{
		ser_print(Q2,"scan\r\n");
		flag_s=qn_scan_ack(Q2);
		if(flag_s==1){
			
			printf("Scan Ack ");
			scan_loop=4;
			scan_cmp_q2=1;
		}
		else if(flag_s==2){
			
			printf("Scan Err ");
			scan_loop++;
		}
	}while(scan_loop<=QN_MAX_CONN);
	
	if(scan_loop>=QN_MAX_CONN){
		ser_print(Q2,"connect\r\n");
		flag_c=qn_con_ack(Q2);
		if(flag_c==1){
			printf("Con Ack ");
			con_cmp_q2=1;
		}
		else if(flag_c==2){
			printf("Con Err ");
			printf("Con incomplete\r\n");
		}
	}
	else{
	
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
int Q3_init(void){

	char flag_s=0, flag_c=0, scan_loop=0;
	while(qn_ack(Q3)!=1);
	
	printf("Received ACK from Q1\r\n");
	
	ser_print(Q3,"Ack\r\n");
	
	ser_print(Q3,"begin\r\n");
	while(qn_ack(Q3)!=1);
	printf("Begin Ack ");
	
	ser_print(Q3,"A0E5E9001398\r\n");
	while(qn_ack(Q3)!=1);
	printf("Add Ack ");
	
	ser_print(Q3,"A0E5E90011FD\r\n");
	while(qn_ack(Q3)!=1);
	printf("Add Ack ");
	
	ser_print(Q3,"A0E5E9000387\r\n");
	while(qn_ack(Q3)!=1);
	printf("Add Ack ");
  
	Delay(1000);
	
	do{
		ser_print(Q3,"scan\r\n");
		flag_s=qn_scan_ack(Q3);
		if(flag_s==1){
			
			printf("Scan Ack ");
			scan_loop=4;
		}
		else if(flag_s==2){
			
			printf("Scan Err ");
			scan_loop++;
		}
	}while(scan_loop<=QN_MAX_CONN);
	
	if(scan_loop>=QN_MAX_CONN){
		ser_print(Q3,"connect\r\n");
		flag_c=qn_con_ack(Q3);
		if(flag_c==1)
			printf("Con Ack ");
		else if(flag_c==2)
			printf("Con Err ");
	}
	else{
	
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
void Qn_start(void){

	ser_print(Q1,"start\r\n");		//Q1
	
	if((scan_cmp_q2==1)&&(con_cmp_q2==1))
		ser_print(Q2,"start\r\n");		//Q2
	
	ser_print(Q3,"start\r\n");		//Q3
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
