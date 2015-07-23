/**
 * @file  Serial.c
 * @brief Low level Serial functions
 * @author uVision & Ron Brash@heddoko
 * @note possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __UART      - UART  (UART1)  interface
 *                        - disabled  (default)
 *
 * @copy This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 Keil - An ARM Company. All rights reserved.
 * @copy Heddoko (2015)
 */

#include <SAM4S.h>                        /* SAM4S definitions                */
#include "Serial.h"

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif

/* Clock Definitions */
#define BAUD(b) ((SystemCoreClock + 8*b)/(16*b))

/**
 * serial_init (void)
 * @brief Initialize Serial port
 */
void serial_init (void) {
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

// configure uart0 for Q1
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

//	configure usart1 for Q2
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


	PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */

#endif

}

/**
 * serial_put_char_ur1(int c)
 * @brief Put character on UART1
 * @param input_char
 * @return input_char
 */
int serial_put_char_ur1 (int input_char) {

#ifdef __DBG_ITM
    ITM_SendChar(input_char);
#else
  #ifdef __UART
    while ((UART1->UART_SR & UART_SR_TXEMPTY) == 0);
    UART1->UART_THR = input_char;
  #endif
#endif
  return (input_char);
}


/*
 * serial_get_char_ur1 (void)
 * @brief Read character from Serial Port - UART1  (blocking read)
 * @return integer upon recieve
 */
int serial_get_char_ur1 (void) {

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

/**
 * serial_put_char_ur0(int c)
 * @brief Put character on UART0
 * @param input_char
 * @return input_char
 */
int serial_put_char_ur0 (int c) {

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


/*
 * serial_get_char_ur0 (void)
 * @brief Read character from Serial Port - UART0   (blocking read)
 * @return integer upon recieve
 */
int serial_get_char_ur0 (void) {

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


/**
 * serial_put_char_us1(int c)
 * @brief Put character on USART1
 * @param input_char
 * @return input_char
 */
int serial_put_char_us1 (int input_char) {

#ifdef __DBG_ITM
    ITM_SendChar(input_char);
#else
  #ifdef __UART
    while ((USART1->US_CSR & US_CSR_TXEMPTY) == 0);
    USART1->US_THR = input_char;
  #endif
#endif
  return (input_char);
}


/*
 * serial_get_char_us1 (void)
 * @brief Read character from Serial Port - USART1   (blocking read)
 * @return integer upon recieve
 */
int serial_get_char_us1 (void) {

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
