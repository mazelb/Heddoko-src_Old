/**
 * @file  LED.c
 * @brief Low level LED functions
 * @author uVision & Ron Brash@heddoko
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
#include "LED.h"

const unsigned long led_mask[] = { PIO_PC23 };

/*
 * LED_init(void)
 * @brief Initialize LED pins
 */
void LED_init(void) {
  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOC);      /* enable PIOC clock                */

  PIOC->PIO_PER  =0;
  PIOC->PIO_OER  =0;
  PIOC->PIO_PUDR =0;
  PIOC->PIO_OWER = (PIO_PC23);            /* Setup PC23 for LEDs              */

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */

  LED_out (0);                            /* switch LEDs off                  */
}

/*
 * LED_on(unsigned int num)
 * @brief Turn on LED pins
 * @param num
 */
void LED_on(unsigned int num) {

  if (num < LED_NUM) {
    PIOC->PIO_CODR = led_mask[num];
  }
}

/*
 * LED_off(unsigned int num)
 * @brief Turn off LED pins
 * @param num
 */
void LED_off (unsigned int num) {

  if (num < LED_NUM) {
    PIOC->PIO_SODR = led_mask[num];
  }
}

/*
 * LED_out(unsigned int value) 
 * @brief Output LED pins
 * @param num
 */
void LED_out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_on (i);
    } else {
      LED_off(i);
    }
  }
}

