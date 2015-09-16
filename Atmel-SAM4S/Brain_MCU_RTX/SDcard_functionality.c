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
//#include "MCI_SAM4S.h"

#include "LED.h"
#include "SDcard_functionality.h"

#define CNTLQ       0x11
#define CNTLS       0x13
#define DEL         0x7F
#define BACKSPACE   0x08
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

static int i;

/*----------------------------------------------------------------------------
 *        Process input string for long or short name entry
 *---------------------------------------------------------------------------*/
//static char *get_entry (char *cp, char **pNext) {
//  char *sp, lfn = 0, sep_ch = ' ';

//  if (cp == NULL) {                           /* skip NULL pointers          */
//    *pNext = cp;
//    return (cp);
//  }

//  for ( ; *cp == ' ' || *cp == '\"'; cp++) {  /* skip blanks and starting  " */
//    if (*cp == '\"') { sep_ch = '\"'; lfn = 1; }
//    *cp = 0;
//  }
// 
//  for (sp = cp; *sp != CR && *sp != LF; sp++) {
//    if ( lfn && *sp == '\"') break;
//    if (!lfn && *sp == ' ' ) break;
//  }

//  for ( ; *sp == sep_ch || *sp == CR || *sp == LF; sp++) {
//    *sp = 0;
//    if ( lfn && *sp == sep_ch) { sp ++; break; }
//  }

//  *pNext = (*sp) ? sp : NULL;                 /* next entry                  */
//  return (cp);
//}

int sd_init(void)
{

	/*	Initialize the HSMCI peripherals	*/
	
	/* Enable PIOA, MCI and DMA clock. */
  PMC->PMC_WPMR  = PMC_WPDIS_KEY;       /* Disable PMC write protection       */
  PMC->PMC_PCER0 = PMC_PCER0_PID11 |    /* Parallel I/O Controller A clock    */
                   PMC_PCER0_PID18 ;    /* Multimedia Card Interface clock    */
  PMC->PMC_WPMR  = PMC_WPEN_KEY;        /* Enable PMC write protection        */
  
  /* Configure I/O pins */
  PIOA->PIO_WPMR = PIO_WPDIS_KEY;       /* Disable PIO write protection       */
  
  PIOA->PIO_IDR  = PIO_IDR_PIN_MSK;
  PIOA->PIO_PDR  = PIO_PDR_PIN_MSK;
  PIOA->PIO_PUER = PIO_PUER_PIN_MSK;

  /* Assign memory card I/O lines to peripheral C (HSMCI) */
  PIOA->PIO_ABCDSR[0] &= ~PIO_ABCDSR_PIN_MSK;
  PIOA->PIO_ABCDSR[1] |=  PIO_ABCDSR_PIN_MSK;
  
  PIOA->PIO_WPMR = PIO_WPEN_KEY;        /* Enable PIO write protection        */
  
  /* Disable HSMCI write protection */
  HSMCI->HSMCI_WPMR = HSMCI_WPDIS_KEY;

  /* Initialize HSMCI peripheral */
  HSMCI->HSMCI_CR   = HSMCI_CR_SWRST;
  HSMCI->HSMCI_CR   = HSMCI_CR_PWSDIS | HSMCI_CR_MCIDIS;
  HSMCI->HSMCI_IDR  = 0xFCFFF1FF;
  HSMCI->HSMCI_DTOR = HSMCI_DTOR_DTOMUL_1048576   | HSMCI_DTOR_DTOCYC(15);
  HSMCI->HSMCI_CSTOR= HSMCI_CSTOR_CSTOMUL_1048576 | HSMCI_CSTOR_CSTOCYC (1);
  HSMCI->HSMCI_CFG  = HSMCI_CFG_HSMODE | HSMCI_CFG_FERRCTRL | HSMCI_CFG_FIFOMODE;
  HSMCI->HSMCI_MR   = HSMCI_MR_PDCMODE | HSMCI_MR_WRPROOF   |
                      HSMCI_MR_RDPROOF | HSMCI_MR_PWSDIV (7);

	/* Select SlotA bus width */
  HSMCI->HSMCI_SDCR &= ~(HSMCI_SDCR_SDCBUS_Msk | HSMCI_SDCR_SDCSEL_Msk);
	HSMCI->HSMCI_SDCR |= HSMCI_SDCR_SDCBUS_4;

  /* Enable HSMCI write protection */
  HSMCI->HSMCI_WPMR = HSMCI_WPEN_KEY;
  
  /* Enable HSMCI */
  HSMCI->HSMCI_CR   = HSMCI_CR_MCIEN;

  /* Send 74 clock cycles for memory card initialization sequence */
  HSMCI->HSMCI_CMDR = HSMCI_CMDR_SPCMD_INIT;
  for (i = HSMCI_TOUT; i; i--)
	{
    if (HSMCI->HSMCI_SR & HSMCI_SR_CMDRDY) 
		{
      return (1);
    }
  }
  return (0);
}


static void init_card (void)
{
  U32 retv = 1;

	while (retv != 0) 
	{        /* Wait until the Card is ready	*/
		retv = finit (NULL);
		if (retv == 1) 
		{
			printf ("SD/MMC Init Failed\n\r");
			printf ("Insert Memory card and press key...\n\r");
			//getkey ();

		}
		else 
		{
			printf ("SD/MMC Card is Unformatted\n\r");
		}
	}
	if (fformat ("") == 0) 
	{
    printf ("Memory Card Formatted.\n\r");
    printf ("Card Label is %s\n\r","KEIL");
  }
  else 
	{
    printf ("Formatting failed.\n\r");
  }
}
/*----------------------------------------------------------------------------
 *        Create a file and fill it with some text
 *---------------------------------------------------------------------------*/
void cmd_fill (char *par) 
{
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
  f = fopen("test.txt", "w");               /* open a file for writing           */
  if (f == NULL) 
	{
    printf ("nCan not open file!\n\r");  /* error when trying to open file    */
    return;
  } 
  for (i = 0; i < cnt; i++)  
	{
    fprintf (f, "This is line # %d in file %s\n\r", i, fname);
  }
  fclose (f);                         /* close the output file               */
  printf ("File closed.\n\r");
}
