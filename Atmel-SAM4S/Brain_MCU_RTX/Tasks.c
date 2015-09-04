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

OS_TID tsk_main, tsk_init, tsk_us1, tsk_ur0, tsk_us0, tsk_ur1, tsk_swbtn, tsk_othr, tsk_idle, id9;
int i=0, j=0;

volatile uint8_t buttonState;

/*
 * task_uart0(void)
 * @brief UART0 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_uart0(void) {

	for(;;) 
	{
			//os_evt_wait_or(0x0004,0xFFFF);
			//if(q2i<bufsize)
				//read_uart0();
//			if(qn.id[1].data[q2i][q2j][q2k]!='\0')
//				q2k++;
//			if(q2k>=datasize){
//					
//					q2j++;
//					q2k=0;
//				}
//				if(q2j>=elmntnb){
//					
//					q2i++;
//					q2j=0;
//					q2k=0;
//				}
//			os_evt_set(0x0004,tsk_main);

	}
}

/*
 * task_uart1(void)
 * @brief UART1 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_uart1(void) {

	for(;;) {
				//os_evt_wait_or(0x0004,0xFFFF);
				
				//read_uart1();
				os_evt_set(0x0004,tsk_main);
	}
}

/*
 * task_usart0(void)
 * @brief USART0 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_usart0(void) {
	for(;;) {
				//os_evt_wait_or(0x0004,0xFFFF);
				//if(q3i<bufsize)
					//read_usart0();
//				if(qn.id[2].data[q3i][q3j][q3k]!='\0')
//					q3k++;
//				if(q3k>=datasize){
//					
//					q3j++;
//					q3k=0;
//				}
//				if(q3j>=elmntnb){
//					
//					q3i++;
//					q3j=0;
//					q3k=0;
//				}
//				os_evt_set(0x0004,tsk_main);

	}
}

/*
 * task_usart1(void)
 * @brief USART1 task listening for incoming traffic from: DEVICE
 * @note PINS: 
 */
__task void task_usart1(void) {
	
	for(;;) {
//				os_evt_wait_or(0x0004,0xFFFF);
//				//if(q1i<bufsize)
//					read_usart1();
//				if(qn.id[0].data[q1i][q1j][q1k]!='\0')		//Heddoko: To increment the buffer only if a character is received
//					q1k++;
//				if(q1k>=datasize){
//			
//					q1j++;
//					q1k=0;
//				}
//				if(q1j>=elmntnb){
//					
//					q1i++;
//					q1j=0;
//					q1k=0;
//				}
		ser_print(Q1,"send1\r\n");
		cnt=nod.id[0].elmnt_cnt;
		printf("\r\nstart1\r\n");
		while(nod.id[0].elmnt_cnt<(cnt+21)){
			if(nod.id[0].buf_cnt<bufsize)
				read_usart1(0);
			if(nod.id[0].data_cnt>=datasize){
		
				nod.id[0].elmnt_cnt++;
				nod.id[0].data_cnt=0;
			}
			if(nod.id[0].elmnt_cnt>=elmntnb){
				
				nod.id[0].buf_cnt++;
				nod.id[0].elmnt_cnt=0;
				nod.id[0].data_cnt=0;
			}
			if(nod.id[0].buf_cnt>bufsize)
				nod.id[0].buf_cnt=0;
		}
		printf("\r\nend1\r\n");
		
		ser_print(Q1,"send2\r\n");
		cnt=nod.id[1].elmnt_cnt;
		printf("\r\nstart2\r\n");
		while(nod.id[1].elmnt_cnt<(cnt+21)){
			if(nod.id[1].buf_cnt<bufsize)
				read_usart1(1);
			if(nod.id[1].data_cnt>=datasize){
		
				nod.id[1].elmnt_cnt++;
				nod.id[1].data_cnt=0;
			}
			if(nod.id[1].elmnt_cnt>=elmntnb){
				
				nod.id[1].buf_cnt++;
				nod.id[1].elmnt_cnt=0;
				nod.id[1].data_cnt=0;
			}
			if(nod.id[1].buf_cnt>bufsize)
				nod.id[1].buf_cnt=0;
		}
		printf("\r\nend2\r\n");
		
		ser_print(Q1,"send3\r\n");
		cnt=nod.id[2].elmnt_cnt;
		printf("\r\nstart3\r\n");
		while(nod.id[2].elmnt_cnt<(cnt+21)){
			if(nod.id[2].buf_cnt<bufsize)
				read_usart1(2);
			if(nod.id[2].data_cnt>=datasize){
		
				nod.id[2].elmnt_cnt++;
				nod.id[2].data_cnt=0;
			}
			if(nod.id[2].elmnt_cnt>=elmntnb){
				
				nod.id[2].buf_cnt++;
				nod.id[2].elmnt_cnt=0;
				nod.id[2].data_cnt=0;
			}
			if(nod.id[2].buf_cnt>bufsize)
				nod.id[2].buf_cnt=0;
		}
		printf("\r\nend3\r\n");
		os_evt_set(0x0004, tsk_main);
//		if((nod.id[0].buf_cnt>bufsize)&(nod.id[1].buf_cnt>bufsize)&(nod.id[2].buf_cnt>bufsize))
//			os_evt_set(0x0004, tsk_othr);
	}
}

/*
 * task_swdbutton(void)
 * @brief SWD BUTTON task  with basic debouncing (press registered on release)
 * @note PINS: SWD button
 */
__task void task_swdbutton(void) {
	
	for(;;) {
				//os_evt_wait_or(0x0004,0xFFFF);
//				if ((button_get() != 0) && (buttonState !=1)) {
//					buttonState = 1;
//					LED_on(0);
//				} else 
				if (btn_irq==1){
					os_tsk_prio_self(2);
					printf("Button press and release\n\r");
					LED_blink(3);
					//LED_off(0);
					ser_print(Q1,"stop\r\n");
					ser_print(Q2,"stop\r\n");
					ser_print(Q3,"stop\r\n");
					buttonState = 0;
					btn_irq=0;
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
__task void task_other(void) {
	
	for(;;){
	//os_evt_wait_or(0x0004,0xFFFF);
//	while((q1i<bufsize)&&(q2i<bufsize)&&(q3i<bufsize)) {
//			;			
//	}
	
	os_evt_wait_or(0x0004, 0xffff);
	os_tsk_prio_self(10);
//	ser_print(Q1,"stop\r\n");
//	ser_print(Q2,"stop\r\n");
//	ser_print(Q3,"stop\r\n");
	
	printf("Printing Buffers\r\n");
	
	printf("Q1\r\n\r\n");
	for(i=0;i<bufsize;i++){
		
		for(j=0;j<elmntnb;j++){
		
			//printf("%s\r\n",qn.id[0].data[i][j]);
			printf("%s\r\n",nod.id[0].data[i][j]);
		}
	}
	
	printf("Q2\r\n\r\n");
	for(i=0;i<bufsize;i++){
		
		for(j=0;j<elmntnb;j++){
		
			//printf("%s\r\n",qn.id[1].data[i][j]);
			printf("%s\r\n",nod.id[1].data[i][j]);
		}
	}
	
	printf("Q3\r\n\r\n");
	for(i=0;i<bufsize;i++){
		
		for(j=0;j<elmntnb;j++){
		
			//printf("%s\r\n",qn.id[2].data[i][j]);
			printf("%s\r\n",nod.id[2].data[i][j]);
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
__task void task_main(void) {
	
	os_evt_wait_or(0x0004, 0xffff);
	main_flg=1;
	tsk_us1 = os_tsk_create(task_usart1,0);
//	tsk_ur0 = os_tsk_create(task_uart0,0);
//	tsk_us0 = os_tsk_create(task_usart0,0);
//	tsk_ur1 = os_tsk_create(task_uart1,0);
//	tsk_swbtn = os_tsk_create(task_swdbutton,0);
//	tsk_othr = os_tsk_create(task_other,0);	
	for(;;) {
		// UARTS & USARTS
		os_evt_set(0x0004,tsk_us1);
		os_evt_wait_or(0x0004,0xFFFF);
//		os_evt_set(0x0004,tsk_ur0);
//		os_evt_wait_or(0x0004,0xFFFF);
//		os_evt_set(0x0004,tsk_us0);
//		os_evt_wait_or(0x0004,0xFFFF);
//		
//		// Reserved for stretch sense
//		os_evt_set(0x0004,tsk_ur1);
//		os_evt_wait_or(0x0004,0xFFFF);
//	
//		// Reserved for SWD button - is a signal better???
//		os_evt_set(0x0004,tsk_swbtn);
//		os_evt_wait_or(0x0004,0xFFFF);
//		
//		os_evt_set(0x0004,tsk_othr);
//		os_evt_wait_or(0x0004,0xFFFF);
		
	}
}

/*
 * task_init(void)
 * @brief Inititalizing the Quintics and SS.
 * @note Runs just once at the beginning. 
 */
__task void task_init(void) {
	
		os_evt_wait_or(0x0004, 0xffff);
//		tsk_init = os_tsk_self();
		os_tsk_prio_self(3);
			Q1_init();
//			Q2_init();
//	 		Q3_init();
	
		btn_menu_lvl=1;
		
		init_flg=1;
		os_tsk_pass();
		os_tsk_prio_self(0);
		os_tsk_prio(tsk_idle, 3);
}

/*
 * task_idle(void)
 * @brief Sleep mode task.
 * @note Ever running task. 
 */
__task void task_idle(void){

	tsk_idle = os_tsk_self();
	tsk_init = os_tsk_create(task_init, 0);
	NVIC_EnableIRQ(PIOA_IRQn);
	os_tsk_prio_self(1);

// Fill the NOD buffer with addresses
	for(i=0; i<NOD_MAX_CNT; i++){
	
		for(j=0;j<20;j++){
			nod.id[i].addr[j]=nod_addr[i][j];
		}
	}
	
	for(;;){
		
		os_evt_wait_or(0x0004, 0xffff);
		os_tsk_prio_self(255);
		if(pw_irq==1){
			
			cur_ticks = TC0->TC_CHANNEL[0].TC_CV;
			
			if(cur_ticks-old_ticks>=25000){
				
				printf("Sleep mode\r\n");
				Qn_stop();
				if(main_flg==1){
					os_tsk_delete(tsk_main);
					os_tsk_delete(tsk_us1);
//					os_tsk_delete(tsk_ur0);
//					os_tsk_delete(tsk_us0);
//					os_tsk_delete(tsk_ur1);
//					os_tsk_delete(tsk_swbtn);
					main_flg=0;
				}
				PIOA->PIO_CODR = (PIO_PA19 | PIO_PA8);
				LED_on(0);
				os_dly_wait(10);
				LED_off(0);
				os_dly_wait(10);
				LED_on(0);
				os_dly_wait(10);
				LED_off(0);
				init_flg=0;
				strt_stop=0;
			}
			else{
			
				printf("Power button pressed");	
				Qn_stop();
				if(init_flg==0)
					os_evt_set(0x0004, tsk_init);
						
			}
			
			pw_irq=0;
			cur_ticks=0;
			os_tsk_prio_self(1);
		}
		
		if((sw1_irq==1)&(btn_menu_lvl==1)){										//Disable and enable Jack EN 
																													//init Quintics and SS
			
			printf("Action switch 1 pressed");
			
			//stop the Quintics comm if enabled
			Qn_stop();	
			
			//Delete all the tasks and reinitialize
			if(main_flg==1){
				os_tsk_delete(tsk_main);
				os_tsk_delete(tsk_us1);
//				os_tsk_delete(tsk_ur0);
//				os_tsk_delete(tsk_us0);
//				os_tsk_delete(tsk_ur1);
//				os_tsk_delete(tsk_swbtn);
				main_flg=0;
			}
			PIOA->PIO_SODR = (PIO_PA19 | PIO_PA8);
			PIOA->PIO_CODR = (PIO_PA19 | PIO_PA8);			//Power cycle Jack ENs
			
			PIOA->PIO_SODR = (PIO_PA17 | PIO_PA18);
			PIOB->PIO_SODR = (PIO_PB10);
			PIOA->PIO_CODR = (PIO_PA17 | PIO_PA18);
			PIOB->PIO_CODR = (PIO_PB10);
			PIOA->PIO_SODR = (PIO_PA17 | PIO_PA18);
			PIOB->PIO_SODR = (PIO_PB10);								//Power cycle and reset Quintics
	
			strt_stop=0;
			os_evt_set(0x0004, tsk_init);
			
			//SS_init();
			sw1_irq=0;
			os_tsk_prio_self(1);
		}
			
		if((sw2_irq==1)&(btn_menu_lvl==1)){
		
			printf("Action switch 2 pressed");
			
			if(strt_stop==0){
				Qn_start();
				LED_on(0);
				tsk_main = os_tsk_create(task_main,0);
				os_evt_set(0x0004, tsk_main);
			}
			if(strt_stop==1){
				Qn_stop();
				LED_off(0);
			}
			strt_stop=!strt_stop;
			sw2_irq=0;
			os_tsk_prio_self(1);
		}
		
	}
}

void PIOA_IRQHandler (void) __irq{

	pioa=PIOA->PIO_ISR;
	if(pioa==PIO_PA1){
		
		sw1_irq=1;
		//printf("Action switch 1");	
		isr_evt_set(0x0004, tsk_idle);
	}
	if(pioa==PIO_PA20){
		
		sw2_irq=1;	
		//printf("Action switch 2");
		isr_evt_set(0x0004, tsk_idle);
	}
	if(pioa==PIO_PA0){
		
		if(pw_irq==0){
		
			call_timer();
			isr_evt_set(0x0004, tsk_idle);
		}
		pw_irq=1;
		//printf("Power button");
	}
	if(pioa==PIO_PA2)
		btn_irq=1;
	
}

void PIOB_IRQHandler (void) __irq{

	if(PIOA->PIO_ISR==PIO_PA11){	//LBO
	
		PIOA->PIO_CODR = (PIO_PA19 | PIO_PA8);
	}
	
	if(PIOA->PIO_ISR==PIO_PA2){		//STAT
	
		
	}
}

int call_timer(void){

	TC0->TC_WPMR = 0x54494D00; 
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;
	TC0->TC_WPMR = 0x54494D01;
	while(TC0->TC_CHANNEL[0].TC_CV!=0);
	old_ticks = TC0->TC_CHANNEL[0].TC_CV;
}
