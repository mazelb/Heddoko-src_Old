/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __UART      - UART  (UART1)  interface
 *                        - disabled  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2013 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <SAM4S.h>                        /* SAM4S definitions                */
#include "Serial.h"
#include <stdio.h>
#include <string.h>
#include "BrainMCU.h"


#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

/* Clock Definitions */
#define BAUD(b) ((SystemCoreClock + 8*b)/(16*b))

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

#ifdef __UART
  PMC->PMC_PCER0 = ((1UL << ID_PIOB) |    /* enable PIOA clock                */
                    (1UL << ID_UART1)  ); /* enable UART clock                */

  /* Configure UART1 Pins (PB3 = TX, PB2 = RX). */
  PIOB->PIO_IDR        =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_PUDR       =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_ABCDSR[0] &= ~(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_ABCDSR[1] &= ~(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);
  PIOB->PIO_PDR        =  (PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);

  /* Configure UART for 115200 baud. */
  UART1->UART_CR   = (UART_CR_RSTRX | UART_CR_RSTTX) |
                     (UART_CR_RXDIS | UART_CR_TXDIS);
  UART1->UART_IDR  = 0xFFFFFFFF;
  UART1->UART_BRGR   = BAUD(115200);
  UART1->UART_MR   =  (0x4 <<  9);        /* (UART) No Parity                 */
  UART1->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  UART1->UART_CR   = UART_CR_RXEN | UART_CR_TXEN;
#endif

// configure uart0 for Q2
#ifdef __UART
  PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    /* enable PIOA clock                */
                    (1UL << ID_UART0)  ); /* enable UART clock                */

  /* Configure UART0 Pins (PA10 = TX, PA9 = RX). */
  PIOA->PIO_IDR        =  (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
  PIOA->PIO_PUDR       =  (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
  PIOA->PIO_ABCDSR[0] &= ~(PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
  PIOA->PIO_ABCDSR[1] &= ~(PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);
  PIOA->PIO_PDR        =  (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);

  /* Configure UART for 115200 baud. */
  UART0->UART_CR   = (UART_CR_RSTRX | UART_CR_RSTTX) |
                     (UART_CR_RXDIS | UART_CR_TXDIS);
  UART0->UART_IDR  = 0xFFFFFFFF;
  UART0->UART_BRGR   = BAUD(115200);
  UART0->UART_MR   =  (0x4 <<  9);        /* (UART) No Parity                 */
  UART0->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  UART0->UART_CR   = UART_CR_RXEN | UART_CR_TXEN;
#endif

//	configure usart1 for Q1
#ifdef __UART
	PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    // enable PIOA clock                
                    (1UL << ID_USART1)  ); // enable USART clock                

  /* Configure USART1 Pins (PA22 = TX, PA21 = RX). */
  PIOA->PIO_IDR        =  (PIO_PA21A_RXD1 | PIO_PA22A_TXD1);
  PIOA->PIO_PUDR       =  (PIO_PA21A_RXD1 |  PIO_PA22A_TXD1);
  PIOA->PIO_ABCDSR[0] &= ~(PIO_PA21A_RXD1 | PIO_PA22A_TXD1);
  PIOA->PIO_ABCDSR[1] &= ~(PIO_PA21A_RXD1 | PIO_PA22A_TXD1);
  PIOA->PIO_PDR        =  (PIO_PA21A_RXD1 | PIO_PA22A_TXD1);
	
	/* configure USART1 enable Pin (PA23) */
  PIOA->PIO_PUDR   =  (PIO_PA23);
  PIOA->PIO_CODR   =  (PIO_PA23);
  PIOA->PIO_OER    =  (PIO_PA23);
  PIOA->PIO_PER    =  (PIO_PA23);

  /* Configure USART for 115200 baud. */
  USART1->US_CR   = (US_CR_RSTRX | US_CR_RSTTX) |
                     (US_CR_RXDIS | US_CR_TXDIS);
  USART1->US_IDR  = 0xFFFFFFFF;
  USART1->US_BRGR   = BAUD(115200);
  USART1->US_MR   =  (US_MR_USART_MODE_NORMAL | US_MR_USCLKS_MCK | US_MR_CHRL_8_BIT |
												US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL) ;//(0x4 <<  9);        /* (USART) No Parity                 */
  USART1->US_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  USART1->US_CR   = UART_CR_RXEN | UART_CR_TXEN;
	
#endif

//	configure usart0 for Q3
#ifdef __UART
	PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    // enable PIOA clock                
                    (1UL << ID_USART0)  ); // enable USART clock                

  /* Configure USART0 Pins (PA6 = TX, PA5 = RX). */
  PIOA->PIO_IDR        =  (PIO_PA5A_RXD0 | PIO_PA6A_TXD0);
  PIOA->PIO_PUDR       =  (PIO_PA5A_RXD0 | PIO_PA6A_TXD0);
  PIOA->PIO_ABCDSR[0] &= ~(PIO_PA5A_RXD0 | PIO_PA6A_TXD0 | PIO_PA2B_SCK0);
  PIOA->PIO_ABCDSR[1] &= ~(PIO_PA5A_RXD0 | PIO_PA6A_TXD0 | PIO_PA2B_SCK0);
  PIOA->PIO_PDR        =  (PIO_PA5A_RXD0 | PIO_PA6A_TXD0);
	
	/* configure USART0 enable Pin (PA2) Peripheral-B */
  PIOA->PIO_PUDR   =  (PIO_PA2);
  PIOA->PIO_CODR   =  (PIO_PA2);
  PIOA->PIO_OER    =  (PIO_PA2);
  PIOA->PIO_PER    =  (PIO_PA2);

  /* Configure USART for 115200 baud. */
  USART0->US_CR   = (US_CR_RSTRX | US_CR_RSTTX) |
                     (US_CR_RXDIS | US_CR_TXDIS);
  USART0->US_IDR  = 0xFFFFFFFF;
  USART0->US_BRGR   = BAUD(115200);
  USART0->US_MR   =  (US_MR_USART_MODE_NORMAL | US_MR_USCLKS_MCK | US_MR_CHRL_8_BIT |
												US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL) ;//(0x4 <<  9);        /* (USART) No Parity                 */
  USART0->US_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
  USART0->US_CR   = UART_CR_RXEN | UART_CR_TXEN;
	
#endif

////	configure SPI for Q2
//#ifdef __UART
//	PMC->PMC_PCER0 = ((1UL << ID_PIOA) |    // enable PIOA clock                
//                    (1UL << ID_SPI)  ); // enable USART clock                

//  /* Configure USART1 Pins (PA22 = TX, PA21 = RX). */
//  PIOA->PIO_IDR        =  (PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//  PIOA->PIO_PUDR       =  (PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//  PIOA->PIO_ABCDSR[0] &= ~(PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//  PIOA->PIO_ABCDSR[1] &= ~(PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//  PIOA->PIO_PDR        =  (PIO_PA12A_MISO | PIO_PA13A_MOSI | PIO_PA11A_NPCS0 | PIO_PA14A_SPCK);
//	
//	/* configure USART1 enable Pin (PA23) */
//  PIOA->PIO_PUDR   =  (PIO_PA23);
//  PIOA->PIO_CODR   =  (PIO_PA23);
//  PIOA->PIO_OER    =  (PIO_PA23);
//  PIOA->PIO_PER    =  (PIO_PA23);

//  /* Configure USART for 115200 baud. */
//  SPI->SPI_CR   = (SPI_CR_SPIEN);
//  SPI->SPI_IDR  = 0xFFFFFFFF;
//  SPI->SPI_MR   =  (SPI_MR_MSTR |  US_MR_CHRL_8_BIT |
//												US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL) ;//(0x4 <<  9);        /* (USART) No Parity                 */
//  USART1->US_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
//  USART1->US_CR   = UART_CR_RXEN | UART_CR_TXEN;
//	
//#endif


	PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */


#endif



}

//UART 1
/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  #ifdef __UART
    while ((UART1->UART_SR & UART_SR_TXEMPTY) == 0);
    UART1->UART_THR = c;
  #endif
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  #ifdef __UART
    while((UART1->UART_SR & UART_SR_RXRDY) == 0);
    return UART1->UART_RHR;
  #endif
#endif
}

// for UART0
/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar0 (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  #ifdef __UART
    while ((UART0->UART_SR & UART_SR_TXEMPTY) == 0);
    UART0->UART_THR = c;
  #endif
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar0 (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  #ifdef __UART
    while((UART0->UART_SR & UART_SR_RXRDY) == 0);
    return UART0->UART_RHR;
  #endif
#endif
}


// for USART1
/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar_us1 (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  #ifdef __UART
    while ((USART1->US_CSR & US_CSR_TXEMPTY) == 0);
    USART1->US_THR = c;
  #endif
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar_us1 (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  #ifdef __UART
    while((USART1->US_CSR & US_CSR_RXRDY) == 0);
    return USART1->US_RHR;
  #endif
#endif
}

// for USART0
/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar_us0 (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  #ifdef __UART
    while ((USART0->US_CSR & US_CSR_TXEMPTY) == 0);
    USART0->US_THR = c;
  #endif
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar_us0 (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  #ifdef __UART
    while((USART0->US_CSR & US_CSR_RXRDY) == 0);
    return USART0->US_RHR;
  #endif
#endif
}

/*----------------------------------------------------------------------------
  Read character from Serial Port   (non-blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar_us1nb (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  #ifdef __UART
    if((USART1->US_CSR & US_CSR_RXRDY)&&(USART1->US_CSR & US_CSR_RXBUFF)){
		USART1->US_IER=US_IDR_RXBUFF;
		return USART1->US_RHR;
		USART1->US_IER=US_IER_RXBUFF;
	}
  #endif
#endif
}

/*
 *****************************************************************************************
 * @brief Print string to requested Quintic
 * @param[in] 1,2 or 3 spefies the Quintic module to print to
 * @param[in] Input string to print
 * @description 
 *
 * This function prints the input strind to the required Serial port or requested Quintic.
 *****************************************************************************************
 */
int ser_print(int c, char str[20]){
	
	int i=0,a=0,j=0;

	while(str[j]!='\0'){
	
		j++;
	}
	
	//printf("j=%d",j);
	if(c==Q1){		// Send to Q1
		for (a=0;a<j;a++){
		
			SER_PutChar_us1(str[a]);
		}
	}
	
	if(c==Q2){		//Send to Q2
		for (a=0;a<j;a++){
		
			SER_PutChar0(str[a]);
		}
	}
	
	if(c==Q3){		//Send to Q3
		for (a=0;a<j;a++){
		
			SER_PutChar_us0(str[a]);
		}
	}
}

/*----------------------------------------------------------------------------
  Calculate the number of characters in a string  
 *----------------------------------------------------------------------------*/
int len(char *t){
    
	int count=0;
  while(*t!='\0'){
        count++;
        t++;
    }
  return(count);
}

/*
 *****************************************************************************************
 * @brief Wait for Acknowledge signal from a desired Quintic module
 * @param[in] 1,2 or 3 specifies the Quintic module to wait for
 * @return 1 if successful
 * @description 
 *
 * This function waits for an Ack signal from desired Quintic module. This is a blocking
 * function.
 *****************************************************************************************
 */
int qn_ack(int c){

	char QnAck[20]="QnAck";
	char u_data[128]={0};	
	uint8_t j=0, i=0;
	
	if(c==Q1){
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar_us1();
			}while(u_data[j++]!=0x0a);
			
	//		for(i=0;i<j;i++){		// Debug print function
	//		
	//		  printf("%c",u_data[i]);
	//		}
	//		printf("\r\n");
		}while(memcmp(u_data,QnAck,5)!=0);
		
		return 1;
	}
	
	if(c==Q2){
	
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar0();
			}while(u_data[j++]!=0x0a);
			
	//		for(i=0;i<j;i++){		// Debug print function
	//		
	//		  printf("%c",u_data[i]);
	//		}
	//		printf("\r\n");
		}while(memcmp(u_data,QnAck,5)!=0);
		
		return 1;
	}
	
	if(c==Q3){
	
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar_us0();
			}while(u_data[j++]!=0x0a);
			
	//		for(i=0;i<j;i++){		// Debug print function
	//		
	//		  printf("%c",u_data[i]);
	//		}
	//		printf("\r\n");
		}while(memcmp(u_data,QnAck,5)!=0);
		
		return 1;
	}
}

/*
 *****************************************************************************************
 * @brief Wait for Scan Acknowledge signal from a desired Quintic module
 * @param[in] 1,2 or 3 specifies the Quintic module to wait for
 * @return 1 if successful
 * @description 
 *
 * This function waits for an Scan Ack signal from desired Quintic module. This is a blocking
 * function. The Quintic modules scan for the desired NODs and after the scan complete event
 * sends this ScanAck signal
 *****************************************************************************************
 */
int qn_scan_ack(int c){

	uint8_t QnScanAck[20]="QnScanAck";
	uint8_t QnScanErr[20]="QnScanErr";
	char u_data[128]={0};	
	uint8_t j=0, i=0, flag=0;
	
	if(c==Q1){
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar_us1();
			}while(u_data[j++]!=0x0a);
			
	//		for(i=0;i<j;i++){		// Debug print function
	//		
	//			printf("%c",u_data[i]);
	//		}
	//		printf("\r\n");
			if(memcmp(u_data,QnScanAck,9)==0)
				flag=1;
			else if(memcmp(u_data,QnScanErr,9)==0)
				flag=2;
		}while(flag==0);
		
		return flag;
	}
	
	if(c==Q2){
	
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar0();
			}while(u_data[j++]!=0x0a);
			
//			for(i=0;i<j;i++){		// Debug print function
//		
//			  printf("%c",u_data[i]);
//			}
//			printf("\r\n");
			
			if(memcmp(u_data,QnScanAck,9)==0)
				flag=1;
			else if(memcmp(u_data,QnScanErr,9)==0)
				flag=2;
		}while(flag==0);
		
		return flag;
	}
	
	if(c==Q3){
	
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar_us0();
			}while(u_data[j++]!=0x0a);
			
	//		for(i=0;i<j;i++){		// Debug print function
	//		
	//		  printf("%c",u_data[i]);
	//		}
	//		printf("\r\n");
			if(memcmp(u_data,QnScanAck,9)==0)
				flag=1;
			else if(memcmp(u_data,QnScanErr,9)==0)
				flag=2;
		}while(flag==0);
		
		return flag;
	}
}

/*
 *****************************************************************************************
 * @brief Wait for Connection Acknowledge signal from a desired Quintic module
 * @param[in] 1,2 or 3 specifies the Quintic module to wait for
 * @return 1 if successful
 * @description 
 *
 * This function waits for an Connection Ack signal from desired Quintic module. This is a 
 * blocking function. The Quintic modules initializes connection with the NODs and sends 
 * this ConAck signal when connected successfully.
 *****************************************************************************************
 */
int qn_con_ack(int c){

	uint8_t QnConAck[20]="QnConAck";
	uint8_t QnConErr[20]="QnConErr";
	char u_data[128]={0};	
	uint8_t j=0, i=0, flag=0;
	
	if(c==Q1){
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar_us1();
			}while(u_data[j++]!=0x0a);
			
	//		for(i=0;i<j;i++){		// Debug print function
	//		
	//			printf("%c",u_data[i]);
	//		}
	//		printf("\r\n");
			if(memcmp(u_data,QnConAck,8)==0)
				flag=1;
			else if(memcmp(u_data,QnConErr,8)==0)
				flag=2;
		}while(flag==0);
		
		return flag;
	}
	
	if(c==Q2){
	
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar0();
			}while(u_data[j++]!=0x0a);
			
	//		for(i=0;i<j;i++){		// Debug print function
	//		
	//			printf("%c",u_data[i]);
	//		}
	//		printf("\r\n");
			if(memcmp(u_data,QnConAck,8)==0)
				flag=1;
			else if(memcmp(u_data,QnConErr,8)==0)
				flag=2;
		}while(flag==0);
		
		return flag;
	}
	
	if(c==Q3){
	
		do{
			j=0;
			do{
				
				u_data[j]=SER_GetChar_us0();
			}while(u_data[j++]!=0x0a);
			
	//		for(i=0;i<j;i++){		// Debug print function
	//		
	//			printf("%c",u_data[i]);
	//		}
	//		printf("\r\n");
			if(memcmp(u_data,QnConAck,8)==0)
				flag=1;
			else if(memcmp(u_data,QnConErr,8)==0)
				flag=2;
		}while(flag==0);
		
		return flag;
	}
}
