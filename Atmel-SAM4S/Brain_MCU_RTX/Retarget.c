/** 
 * @file  Retarget.c
 * @brief 'Retarget' layer for target-dependent low level functions
 * @author uVision & Ron Brash@heddoko
 * @note possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *            __UART      - UART  (UART1)  interface
 *                        - disabled  (default)
 *
 * @copy 
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <rt_misc.h>
#include "Serial.h"

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc(int c, FILE *f) {
  return (serial_put_char_ur1(c));
}


int fgetc(FILE *f) {
  return (serial_get_char_ur1());
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int c) {
  serial_put_char_ur1(c);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
