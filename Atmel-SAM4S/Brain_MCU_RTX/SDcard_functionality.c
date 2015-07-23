/**
 * @file SDcard_functionality.c
 * @author Ron Brash - Heddoko 
 * @date July 23nd, 2015
 * @brief Handle all persistent SD card functionality
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <rtl.h>
#include <SAM4S.h>

#include <ctype.h>                    /* character functions                 */
#include <string.h>                   /* string and memory functions         */
#include "File_Config.h"
#include "Serial.h"
#include "Retarget.h"

#include "LED.h"
#include "SDcard_functionality.h"

#define CNTLQ       0x11
#define CNTLS       0x13
#define DEL         0x7F
#define BACKSPACE   0x08
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

/*----------------------------------------------------------------------------
 *        Process input string for long or short name entry
 *---------------------------------------------------------------------------*/
static char *get_entry (char *cp, char **pNext) {
  char *sp, lfn = 0, sep_ch = ' ';

  if (cp == NULL) {                           /* skip NULL pointers          */
    *pNext = cp;
    return (cp);
  }

  for ( ; *cp == ' ' || *cp == '\"'; cp++) {  /* skip blanks and starting  " */
    if (*cp == '\"') { sep_ch = '\"'; lfn = 1; }
    *cp = 0;
  }
 
  for (sp = cp; *sp != CR && *sp != LF; sp++) {
    if ( lfn && *sp == '\"') break;
    if (!lfn && *sp == ' ' ) break;
  }

  for ( ; *sp == sep_ch || *sp == CR || *sp == LF; sp++) {
    *sp = 0;
    if ( lfn && *sp == sep_ch) { sp ++; break; }
  }

  *pNext = (*sp) ? sp : NULL;                 /* next entry                  */
  return (cp);
}

static void init_card (void) {
  U32 retv = 1;

  while (retv != 0) {        /* Wait until the Card is ready*/
		retv = finit ("M0:");
    if (retv == 1) {
      printf ("\nSD/MMC Init Failed\n");
      printf ("\nInsert Memory card and press key...\r\n");
			//getkey ();

    }
    else {
      printf ("\nSD/MMC Card is Unformatted");
		}
	}
	if (fformat ("") == 0) {
      printf ("Memory Card Formatted.\n");
      printf ("Card Label is %s\n","KEIL");
    }
    else {
      printf ("Formatting failed.\n");
    }
}
/*----------------------------------------------------------------------------
 *        Create a file and fill it with some text
 *---------------------------------------------------------------------------*/
void cmd_fill (char *par) {
  char *fname, *next;
  FILE *f;
  int i,cnt = 1000;

//  fname = get_entry (par, &next);
//  if (fname == NULL) {
//    printf ("\nFilename missing.\n");
//    return;
//  }
//  if (next) {
//    par = get_entry (next, &next);
//    if (sscanf (par,"%d", &cnt) == 0) {
//      printf ("\nCommand error.\n");
//      return;
//    }
//  }
	init_card();
  f = fopen ("test.txt", "w");               /* open a file for writing           */
  if (f == NULL) {
    printf ("\nCan not open file!\n");  /* error when trying to open file    */
    return;
  } 
  for (i = 0; i < cnt; i++)  {
    fprintf (f, "This is line # %d in file %s\n", i, fname);
  }
  fclose (f);                         /* close the output file               */
  printf ("\nFile closed.\n");
}
