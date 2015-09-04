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
#include <stdint.h>
#include <rt_misc.h>
#include <rt_sys.h>
#include <SAM4S.h>
#include <string.h>
#include "Retarget.h"
#include "Serial.h"
#include "File_Config.h"

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };

/* Standard IO device handles. */
#define STDIN   0x8001
#define STDOUT  0x8002
#define STDERR  0x8003

#define STDIO 1

/* Standard IO device name defines. */
const char __stdin_name[]  = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";


int fputc(int c, FILE *f) {
  return (ser_putchar_ur1(c));
}


int fgetc(FILE *f) {
  return (ser_getchar_ur1());
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int c) {
  ser_putchar_ur1(c);
}


void _sys_exit(int return_code) {
  while (1);
//	label:  goto label;  /* endless loop */
}


#ifdef STDIO
/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int sendchar (int ch) {

  if (ch == '\n')  {
    ser_putchar_ur1 ('\r');
  }
  ser_putchar_ur1 (ch);

  return (ch);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int getkey (void) {

  return (ser_getchar_ur1());
}
#endif

/*--------------------------- _sys_open -------------------------------------*/

FILEHANDLE _sys_open (const char *name, int openmode) {
  /* Register standard Input Output devices. */
  if (strcmp(name, "STDIN") == 0) {
    return (STDIN);
  }
  if (strcmp(name, "STDOUT") == 0) {
    return (STDOUT);
  }
  if (strcmp(name, "STDERR") == 0) {
    return (STDERR);
  }
  return (__sys_open (name, openmode));
}

/*--------------------------- _sys_close ------------------------------------*/

int _sys_close (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (0);
  }
  return (__sys_close (fh));
}

/*--------------------------- _sys_write ------------------------------------*/

int _sys_write (FILEHANDLE fh, const uint8_t *buf, uint32_t len, int mode) {
#ifdef STDIO
  if (fh == STDOUT) {
    /* Standard Output device. */
    for (  ; len; len--) {
      sendchar (*buf++);
    }
    return (0);
  }
#endif
  if (fh > 0x8000) {
    return (-1);
  }
  return (__sys_write (fh, buf, len));
}

/*--------------------------- _sys_read -------------------------------------*/

int _sys_read (FILEHANDLE fh, uint8_t *buf, uint32_t len, int mode) {
#ifdef STDIO
  if (fh == STDIN) {
    /* Standard Input device. */
    for (  ; len; len--) {
      *buf++ = getkey ();
    }
    return (0);
  }
#endif
  if (fh > 0x8000) {
    return (-1);
  }
  return (__sys_read (fh, buf, len));
}

/*--------------------------- _sys_istty ------------------------------------*/

int _sys_istty (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (1);
  }
  return (0);
}

/*--------------------------- _sys_seek -------------------------------------*/

int _sys_seek (FILEHANDLE fh, long pos) {
  if (fh > 0x8000) {
    return (-1);
  }
  return (__sys_seek (fh, pos));
}

/*--------------------------- _sys_ensure -----------------------------------*/

int _sys_ensure (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (-1);
  }
  return (__sys_ensure (fh));
}

/*--------------------------- _sys_flen -------------------------------------*/

long _sys_flen (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (0);
  }
  return (__sys_flen (fh));
}

/*--------------------------- _sys_tmpnam -----------------------------------*/

int _sys_tmpnam (char *name, int sig, unsigned maxlen) {
  return (1);
}

/*--------------------------- _sys_command_string ---------------------------*/

char *_sys_command_string (char *cmd, int len) {
  return (cmd);
}

/*--------------------------- _sys_exit -------------------------------------*/

