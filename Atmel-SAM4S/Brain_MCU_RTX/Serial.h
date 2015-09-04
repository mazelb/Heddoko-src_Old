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

extern void serial_init   		(void);
extern int  ser_getchar_ur0   (void);
extern int  ser_putchar_ur0   (int input_char);
extern int  ser_getchar_ur1   (void);
extern int  ser_putchar_ur1   (int input_char);
extern int  ser_getchar_us1   (void);
extern int  ser_putchar_us1   (int input_char);
extern int  ser_getchar_us0   (void);
extern int  ser_putchar_us0   (int input_char);
extern int  ser_getchar_us1nb (void);
extern int  ser_getchar_ur1nb (void);
extern int  ser_getchar_us0nb (void);
extern int  ser_getchar_ur0nb (void);
extern int 	ser_print_us1 		(char str[20]);
extern int  ser_print					(int c, char str[20]);		//print string
extern int  ser_print_p				(int c, char *str);				//print string pointer
extern int  len								(char *);
extern int  qn_ack 						(int c);
extern int  qn_scan_ack				(int c);
extern int  qn_con_ack				(int c);


#endif
