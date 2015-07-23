/**
 * @file  Button.c
 * @brief Low level button functions
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

#include <SAM4S.h>


/**
 * button_init(void)
 * @brief Initialize button functionality
 */
void button_init(void) {
  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER =
  PIOA->PIO_IDR  =
  PIOA->PIO_ODR  =
  PIOA->PIO_PER  = (PIO_PA2);             /* Setup PA2 for user button SW0    */

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */
}


/**
 * button_get(void)
 * @brief Return button states
 * @return Unsigned Int for button state, else 0 for not pressed
 */
unsigned int button_get(void) {
  
  return (~(PIOA->PIO_PDSR) & (PIO_PA2));
}
