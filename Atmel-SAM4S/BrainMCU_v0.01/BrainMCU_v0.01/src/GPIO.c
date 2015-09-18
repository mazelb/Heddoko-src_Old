/*
 * GPIO.c
 *
 * Created: 9/9/2015 12:39:25 PM
 *  Author: Hriday Mehta
 */ 

#include "GPIO.h"

/**
 * ButtonInit(void)
 * @brief Initialize button functionality
 * Power button		: PA0
 * Action Switch 1: PA1
 * Action Switch 2: PA20
 */
void ButtonInit(void) {
  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER =
  PIOA->PIO_IER  = (PIO_PA2);
	PIOA->PIO_AIMER	 = (PIO_PA2);
	PIOA->PIO_ESR    = (PIO_PA2);						/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA2);
  PIOA->PIO_ODR  =
  PIOA->PIO_PER  = (PIO_PA2);             /* Setup PA2 for user button SW0    */

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect             */
	
	/*	Power button	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER   = 
  PIOA->PIO_IER    = (PIO_PA0);
	PIOA->PIO_AIMER	 = (PIO_PA0);
	PIOA->PIO_LSR    = (PIO_PA0);						/* Level detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA0);
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA0);             /* Setup PA0 for power button 	    */

  PMC->PMC_WPMR = 0x504D4301;        
	
	/*	Action Switch 1	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

	PIOA->PIO_PUER   = (PIO_PA1);
  PIOA->PIO_IER    = (PIO_PA1);
	PIOA->PIO_AIMER	 = (PIO_PA1);
	PIOA->PIO_ESR    = (PIO_PA1);						/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA1);
	PIOA->PIO_IFER	 = (PIO_PA1);
	PIOA->PIO_IFSCER = (PIO_PA1);
	PIOA->PIO_SCDR	 = 
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA1);             /* Setup PA1 for Action Switch 1    */

  PMC->PMC_WPMR = 0x504D4301;  
	
	/*	Action Switch 2	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER   = (PIO_PA20);
  PIOA->PIO_IER    = (PIO_PA20);
	PIOA->PIO_AIMER	 = (PIO_PA20);
	PIOA->PIO_ESR    = (PIO_PA20);					/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA20);
//	PIOA->PIO_IFER	 = (PIO_PA20);
	PIOA->PIO_IFSCER = (PIO_PA20);
	PIOA->PIO_SCDR	 = 
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA20);             /* Setup PA20 for Action Switch 2    */

  PMC->PMC_WPMR = 0x504D4301;  
	
		/*	Jack OC 1	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER   =
  PIOA->PIO_IER    = (PIO_PA16);
	PIOA->PIO_ESR    = (PIO_PA16);					/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA16);
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA16);             /* Setup PA16 for     */

  PMC->PMC_WPMR = 0x504D4301;  
	
	/*	Jack OC 2	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER   =
  PIOA->PIO_IER    = (PIO_PA15);
	PIOA->PIO_ESR    = (PIO_PA15);					/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA15);
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA15);             /* Setup PA15 for     */

  PMC->PMC_WPMR = 0x504D4301;  
	
	/*	Jack detect 1	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER   =
  PIOA->PIO_IER    = (PIO_PA24);
	PIOA->PIO_ESR    = (PIO_PA24);					/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA24);
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA24);             /* Setup PA24 for     */

  PMC->PMC_WPMR = 0x504D4301;  
	
	/*	Jack detect 2	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER   =
  PIOA->PIO_IER    = (PIO_PA25);
	PIOA->PIO_ESR    = (PIO_PA25);					/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA25);
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA25);             /* Setup PA25 for     */

  PMC->PMC_WPMR = 0x504D4301;  
	
	/*	Jack EN 1	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PER    =
	PIOA->PIO_OER    =
	PIOA->PIO_PUDR   =
  PIOA->PIO_OWER    = (PIO_PA19);             /* Setup PA19 for     */

  PMC->PMC_WPMR = 0x504D4301;  
	
	/*	Jack EN 2	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PER    =
	PIOA->PIO_OER    =
	PIOA->PIO_PUDR   =
  PIOA->PIO_OWER    = (PIO_PA8);              /* Setup PA8 for     */

  PMC->PMC_WPMR = 0x504D4301;  
	
	/*	BLE RST 1	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOB);      /* enable clock for push button     */

  PIOB->PIO_PER    =
	PIOB->PIO_OER    =
	PIOB->PIO_PUDR   =
  PIOB->PIO_OWER    = (PIO_PB10);              /* Setup PA8 for     */

  PMC->PMC_WPMR = 0x504D4301; 
	
	/*	BLE RST 2	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PER    =
	PIOA->PIO_OER    =
	PIOA->PIO_PUDR   =
  PIOA->PIO_OWER    = (PIO_PA17);              /* Setup PA8 for     */

  PMC->PMC_WPMR = 0x504D4301; 
	
	/*	BLE RST 3	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PER    =
	PIOA->PIO_OER    =
	PIOA->PIO_PUDR   =
  PIOA->PIO_OWER    = (PIO_PA18);              /* Setup PA8 for     */

  PMC->PMC_WPMR = 0x504D4301; 
	
	/*	Green LED	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOB);      /* enable clock for push button     */

  PIOB->PIO_PER    =
	PIOB->PIO_OER    =
	PIOB->PIO_PUDR   =
  PIOB->PIO_OWER    = (PIO_PB13);              /* Setup PA8 for     */

  PMC->PMC_WPMR = 0x504D4301; 
	
	/*	Red LED	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOB);      /* enable clock for push button     */

  PIOB->PIO_PER    =
	PIOB->PIO_OER    =
	PIOB->PIO_PUDR   =
  PIOB->PIO_OWER    = (PIO_PB1);              /* Setup PA8 for     */

  PMC->PMC_WPMR = 0x504D4301; 
	
	/*	Blue LED	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOB);      /* enable clock for push button     */

  PIOB->PIO_PER    =
	PIOB->PIO_OER    =
	PIOB->PIO_PUDR   =
  PIOB->PIO_OWER   = (PIO_PB0);              /* Setup PB0 for     */

  PMC->PMC_WPMR = 0x504D4301; 
	
		/*	LBO	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER   =
  PIOA->PIO_IER    = (PIO_PA11);
	PIOA->PIO_AIMER	 = (PIO_PA11);
	PIOA->PIO_ESR    = (PIO_PA11);					/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA11);
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA11);             /* Setup PA11 for LBO    */

  PMC->PMC_WPMR = 0x504D4301;  
	
			/*	STAT	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOA);      /* enable clock for push button     */

  PIOA->PIO_PUER   =
  PIOA->PIO_IER    = (PIO_PA2);
	PIOA->PIO_AIMER	 = (PIO_PA2);
	PIOA->PIO_ESR    = (PIO_PA2);					/* Edge detect interrupt	*/
	PIOA->PIO_FELLSR = (PIO_PA2);
  PIOA->PIO_ODR    =
  PIOA->PIO_PER    = (PIO_PA2);             /* Setup PA2 for STAT    */

  PMC->PMC_WPMR = 0x504D4301; 

			/*	CD	*/
	PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect            */

  PMC->PMC_PCER0 = (1UL << ID_PIOB);      /* enable clock for push button     */

  PIOB->PIO_PUER   =
  PIOB->PIO_IER    = (PIO_PB11);
	PIOB->PIO_AIMER	 = (PIO_PB11);
	PIOB->PIO_ESR    = (PIO_PB11);					/* Edge detect interrupt	*/
	PIOB->PIO_FELLSR = (PIO_PB11);
  PIOB->PIO_ODR    =
  PIOB->PIO_PER    = (PIO_PB11);             /* Setup PB11 for CD    */

  PMC->PMC_WPMR = 0x504D4301;  
	
	TC0->TC_WPMR = 0x54494D00; 
	PMC->PMC_WPMR = 0x504D4300;
	PMC->PMC_PCER0 = (1UL << ID_TC0);
	
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN;
	TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK5;
	
	//PMC->PMC_WPMR = 0x504D4301; 
	TC0->TC_WPMR = 0x54494D01; 
}


/**
 * button_get(void)
 * @brief Return button states
 * @return Unsigned Int for button state, else 0 for not pressed
 */
unsigned int button_get(void) {
  
  return (~(PIOA->PIO_PDSR) & (PIO_PA2));
}
