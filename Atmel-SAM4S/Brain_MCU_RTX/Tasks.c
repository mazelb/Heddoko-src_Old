/**
 * @file Tasks.c
 * @author Heddoko 
 * @date July 22nd, 2015
 * @brief Handle all RTX tasks
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <rtl.h>
#include <SAM4S.h>
#include "BrainMCU.h"
#include "commands.h"
#include "Serial.h"
#include "UART_functionality.h"
#include "Tasks.h"
#include "LED.h"
#include "Button.h"

//	Task ids
OS_TID tsk_main, tsk_init, tsk_us1, tsk_ur0, tsk_us0, tsk_ur1, tsk_swbtn, tsk_othr, tsk_idle, id9;

int i=0, j=0;

volatile uint8_t buttonState;

/*
 * task_uart0(void)
 * @brief UART0 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_uart0(void) 
{
	for(;;) 
	{
		//Will have code similar to that of ReadUsart1
		os_evt_set(0x0004, tsk_main);
	}
}

/*
 * task_uart1(void)
 * @brief UART1 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_uart1(void) 
{
	for(;;) 
	{		
		//Will have code similar to that of ReadUsart1
		os_evt_set(0x0004, tsk_main);
	}
}

/*
 * task_usart0(void)
 * @brief USART0 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_usart0(void) 
{
	for(;;) 
	{
		//Will have code similar to that of ReadUsart1
		os_evt_set(0x0004, tsk_main);
	}
}

/*
 * task_usart1(void)
 * @brief USART1 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_usart1(void) 
{	
	for(;;) 
	{
		SerialPrint(Q1,"send1\r\n");
		cnt=Nod.id[0].ElementCount;
		printf("\r\nstart1\r\n");
		while(Nod.id[0].ElementCount<(cnt+21)){
			if(Nod.id[0].BufCount<gBufSize)
				ReadUsart1(0);
			if(Nod.id[0].DataCount>=gDataSize){
		
				Nod.id[0].ElementCount++;
				Nod.id[0].DataCount=0;
			}
			if(Nod.id[0].ElementCount>=gElementNum){
				
				Nod.id[0].BufCount++;
				Nod.id[0].ElementCount=0;
				Nod.id[0].DataCount=0;
			}
			if(Nod.id[0].BufCount>gBufSize)
				Nod.id[0].BufCount=0;
		}
		printf("\r\nend1\r\n");
		
		SerialPrint(Q1,"send2\r\n");
		cnt=Nod.id[1].ElementCount;
		printf("\r\nstart2\r\n");
		while(Nod.id[1].ElementCount<(cnt+21)){
			if(Nod.id[1].BufCount<gBufSize)
				ReadUsart1(1);
			if(Nod.id[1].DataCount>=gDataSize){
		
				Nod.id[1].ElementCount++;
				Nod.id[1].DataCount=0;
			}
			if(Nod.id[1].ElementCount>=gElementNum){
				
				Nod.id[1].BufCount++;
				Nod.id[1].ElementCount=0;
				Nod.id[1].DataCount=0;
			}
			if(Nod.id[1].BufCount>gBufSize)
				Nod.id[1].BufCount=0;
		}
		printf("\r\nend2\r\n");
		
		SerialPrint(Q1,"send3\r\n");
		cnt=Nod.id[2].ElementCount;
		printf("\r\nstart3\r\n");
		while(Nod.id[2].ElementCount<(cnt+21)){
			if(Nod.id[2].BufCount<gBufSize)
				ReadUsart1(2);
			if(Nod.id[2].DataCount>=gDataSize){
		
				Nod.id[2].ElementCount++;
				Nod.id[2].DataCount=0;
			}
			if(Nod.id[2].ElementCount>=gElementNum){
				
				Nod.id[2].BufCount++;
				Nod.id[2].ElementCount=0;
				Nod.id[2].DataCount=0;
			}
			if(Nod.id[2].BufCount>gBufSize)
				Nod.id[2].BufCount=0;
		}
		printf("\r\nend3\r\n");
		os_evt_set(0x0004, tsk_main);
//		if((Nod.id[0].BufCount>gBufSize)&(Nod.id[1].BufCount>gBufSize)&(Nod.id[2].BufCount>gBufSize))		// Use if want to run the loop only for one cycle of Buffers
//			os_evt_set(0x0004, tsk_othr);
	}
}

/*
 * task_swdbutton(void)
 * @brief SWD BUTTON task  with basic debouncing (press registered on release)
 * @note PINS: SWD button
 */
__task void task_swdbutton(void) 
{
	for(;;) 
	{
		if (svSwBtnIrqFlag==1)
		{
			os_tsk_prio_self(2);
			printf("Button press and release\n\r");
			LED_blink(3);
			SerialPrint(Q1,"stop\r\n");
			SerialPrint(Q2,"stop\r\n");
			SerialPrint(Q3,"stop\r\n");
			buttonState = 0;
			svSwBtnIrqFlag=0;
			os_tsk_prio_self(0);
		}
		os_evt_set(0x0004,tsk_main);
	}
}

/*
 * task_other(void)
 * @brief Other task functions 
 * @note PINS:
 */
__task void task_other(void) 
{
	for(;;)
	{
		os_evt_wait_or(0x0004,0xFFFF);
		
		os_evt_wait_or(0x0004, 0xffff);
		os_tsk_prio_self(10);
		SerialPrint(Q1,"stop\r\n");
		SerialPrint(Q2,"stop\r\n");
		SerialPrint(Q3,"stop\r\n");
		
		printf("Printing Buffers\r\n");
		
		printf("Q1\r\n\r\n");
		for(i=0;i<gBufSize;i++)
		{
			for(j=0;j<gElementNum;j++)
			{
				printf("%s\r\n",Nod.id[0].Data[i][j]);
			}
		}
	
		printf("Q2\r\n\r\n");
		for(i=0;i<gBufSize;i++)
		{
			for(j=0;j<gElementNum;j++)
			{
				printf("%s\r\n",Nod.id[1].Data[i][j]);
			}
		}
	
		printf("Q3\r\n\r\n");
		for(i=0;i<gBufSize;i++)
		{
			for(j=0;j<gElementNum;j++)
			{
				printf("%s\r\n",Nod.id[2].Data[i][j]);
			}
		}
	
		os_tsk_prio_self(1);
		os_evt_set(0x0004,tsk_main); 
	}
}

/*
 * task_main(void)
 * @brief Main task loop
 * @note Can be tidied up a bit - may need some performance tuning
 */
__task void task_main(void) 
{
	os_evt_wait_or(0x0004, 0xffff);
	tsk_us1 = os_tsk_create(task_usart1,0);
	tsk_ur0 = os_tsk_create(task_uart0,0);
	tsk_us0 = os_tsk_create(task_usart0,0);
	tsk_ur1 = os_tsk_create(task_uart1,0);
	tsk_swbtn = os_tsk_create(task_swdbutton,0);
	tsk_othr = os_tsk_create(task_other,0);	
	svMainTaskCmpFlag=1;
	for(;;) 
	{
		// UARTS & USARTS
		os_evt_set(0x0004,tsk_us1);
		os_evt_wait_or(0x0004,0xFFFF);
		os_evt_set(0x0004,tsk_ur0);
		os_evt_wait_or(0x0004,0xFFFF);
		os_evt_set(0x0004,tsk_us0);
		os_evt_wait_or(0x0004,0xFFFF);
		
		// Reserved for stretch sense
		os_evt_set(0x0004,tsk_ur1);
		os_evt_wait_or(0x0004,0xFFFF);
	
		// Reserved for SWD button
		os_evt_set(0x0004,tsk_swbtn);
		os_evt_wait_or(0x0004,0xFFFF);
		
		os_evt_set(0x0004,tsk_othr);
		os_evt_wait_or(0x0004,0xFFFF);
	}
}

/*
 * task_init(void)
 * @brief Inititalizing the Quintics and SS.
 * @note Runs just once at the beginning. 
 */
__task void task_init(void) 
{
		os_evt_wait_or(0x0004, 0xffff);
		os_tsk_prio_self(3);
		Q1Init();
//		Q2Init();
//		Q3Init();
	
		svMenuLevel=1;
		
		svInitCmpFlag=1;
		os_tsk_pass();
		os_tsk_prio_self(0);
		os_tsk_prio(tsk_idle, 3);
}

/*
 * task_idle(void)
 * @brief Sleep mode task.
 * @note Ever running task. 
 */
__task void task_idle(void)
{
	tsk_idle = os_tsk_self();
	
	//Create Initialization task
	tsk_init = os_tsk_create(task_init, 0);
	
	//Enable interrupts on ports
	NVIC_EnableIRQ(PIOA_IRQn);
	NVIC_EnableIRQ(PIOB_IRQn);
	
	os_tsk_prio_self(1);

	//Fill the NOD buffer with addresses
	for(i=0; i<NOD_MAX_CNT; i++)
	{	
		for(j=0;j<20;j++)
		{
			Nod.id[i].Addr[j]=sgaNodAddr[i][j];
		}
	}
	
	for(;;)
	{
		os_evt_wait_or(0x0004, 0xffff);
		os_tsk_prio_self(255);
		
		if(svPowerSwIrqFlag==1)
		{
			//Initialize timer to know elasped time for entering in Sleep mode
			svCurrentTicks = TC0->TC_CHANNEL[0].TC_CV;
			
			//Decide whether to enter sleep mode or not
			if(svCurrentTicks-svOldTicks>=25000)
			{
				printf("Sleep mode\r\n");
				QnStop();
				if(svMainTaskCmpFlag==1)
				{
					os_tsk_delete(tsk_main);
					os_tsk_delete(tsk_us1);
					os_tsk_delete(tsk_ur0);
					os_tsk_delete(tsk_us0);
					os_tsk_delete(tsk_ur1);
					os_tsk_delete(tsk_swbtn);
					svMainTaskCmpFlag=0;
				}
				PIOA->PIO_CODR = (PIO_PA19 | PIO_PA8);
				LED_on(0);
				os_dly_wait(10);
				LED_off(0);
				os_dly_wait(10);
				LED_on(0);
				os_dly_wait(10);
				LED_off(0);
				svInitCmpFlag=0;
				svStartStopToggle=0;
			}
			else
			{
				printf("Power button pressed");	
				QnStop();
				if(svInitCmpFlag==0)
					os_evt_set(0x0004, tsk_init);	
			}
			
			svPowerSwIrqFlag=0;
			svCurrentTicks=0;
			os_tsk_prio_self(1);
		}
		
		if((svActionSw1IrqFlag==1)&(svMenuLevel==1))										//Disable and enable Jack EN 
		{																											//init Quintics and SS
			printf("Action switch 1 pressed");
			
			//stop the Quintics comm if enabled
			QnStop();	
			
			//Delete all the tasks and reinitialize
			if(svMainTaskCmpFlag==1){
				os_tsk_delete(tsk_main);
				os_tsk_delete(tsk_us1);
				os_tsk_delete(tsk_ur0);
				os_tsk_delete(tsk_us0);
				os_tsk_delete(tsk_ur1);
				os_tsk_delete(tsk_swbtn);
				svMainTaskCmpFlag=0;
			}
			PIOA->PIO_SODR = (PIO_PA19 | PIO_PA8);
			PIOA->PIO_CODR = (PIO_PA19 | PIO_PA8);			//Power cycle Jack ENs
			
			PIOA->PIO_SODR = (PIO_PA17 | PIO_PA18);
			PIOB->PIO_SODR = (PIO_PB10);
			PIOA->PIO_CODR = (PIO_PA17 | PIO_PA18);
			PIOB->PIO_CODR = (PIO_PB10);
			PIOA->PIO_SODR = (PIO_PA17 | PIO_PA18);
			PIOB->PIO_SODR = (PIO_PB10);								//Power cycle and reset Quintics
	
			svStartStopToggle=0;
			os_evt_set(0x0004, tsk_init);
			
			//SS_init();
			svActionSw1IrqFlag=0;
			os_tsk_prio_self(1);
		}
			
		if((svActionSw2IrqFlag==1)&(svMenuLevel==1))
		{
			printf("Action switch 2 pressed");
			
			if(svStartStopToggle==0)
			{
				QnStart();
				LED_on(0);
				tsk_main = os_tsk_create(task_main,0);
				os_evt_set(0x0004, tsk_main);
			}
			if(svStartStopToggle==1)
			{
				QnStop();
				LED_off(0);
			}
			svStartStopToggle=!svStartStopToggle;
			svActionSw2IrqFlag=0;
			os_tsk_prio_self(1);
		}
	}
}

void PIOA_IRQHandler (void) __irq
{
	svPioaIsrVal=PIOA->PIO_ISR;
	if(svPioaIsrVal==PIO_PA1)
	{
		svActionSw1IrqFlag=1;
		isr_evt_set(0x0004, tsk_idle);
	}
	if(svPioaIsrVal==PIO_PA20)
	{
		svActionSw2IrqFlag=1;	
		isr_evt_set(0x0004, tsk_idle);
	}
	if(svPioaIsrVal==PIO_PA0)
	{
		if(svPowerSwIrqFlag==0)
		{
			StartTimer();
			isr_evt_set(0x0004, tsk_idle);
		}
		svPowerSwIrqFlag=1;
	}
	if(svPioaIsrVal==PIO_PA2)
		svSwBtnIrqFlag=1;
}

void PIOB_IRQHandler (void) __irq
{
	if(PIOA->PIO_ISR==PIO_PA11)	//LBO
	{	
		PIOA->PIO_CODR = (PIO_PA19 | PIO_PA8);
	}
	if(PIOA->PIO_ISR==PIO_PA2)	//STAT
	{		
		
	}
}

int StartTimer(void)
{
	TC0->TC_WPMR = 0x54494D00; 
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;
	TC0->TC_WPMR = 0x54494D01;
	while(TC0->TC_CHANNEL[0].TC_CV!=0);
	svOldTicks = TC0->TC_CHANNEL[0].TC_CV;
}
