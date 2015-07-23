/*----------------------------------------------------------------------------
 * Name:    Serial.h
 * Purpose: Low level serial definitions
 * Note(s):
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

#ifndef __SERIAL_H
#define __SERIAL_H

extern void serial_init   (void);
extern int  serial_get_char_ur0   (void);
extern int  serial_put_char_ur0   (int input_char);
extern int  serial_get_char_ur1   (void);
extern int  serial_put_char_ur1   (int input_char);
extern int  serial_get_char_us1   (void);
extern int  serial_put_char_us1   (int input_char);

#endif
