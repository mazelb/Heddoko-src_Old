/*
 * GPIO.h
 *
 * Created: 9/9/2015 12:40:11 PM
 *  Author: Hriday Mehta
 */ 

#ifndef GPIO_H_
#define GPIO_H_

#include "BrainMCU.h"
#include <stdlib.h>
#include <board.h>
#include <gpio.h>
#include <ioport.h>
#include <wdt.h>
#include <asf\common\services\gpio\sam_gpio\sam_gpio.h>

extern void         ButtonInit(void);
extern unsigned int button_get (void);

/* SOFTWARE BUTTON 0*/
#define PIN_SW0_GPIO  (PIO_PA2_IDX)
#define PIN_SW0_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT)

/*	Power button	*/
#define PIN_PW_SW_GPIO  (PIO_PA0_IDX)
#define PIN_PW_SW_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT)

/*	Action Switch 1	*/
#define PIN_AC_SW1_GPIO  (PIO_PA1_IDX)
#define PIN_AC_SW1_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT)

/*	Action Switch 2	*/
#define PIN_AC_SW2_GPIO  (PIO_PA20_IDX)
#define PIN_AC_SW2_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT)

/*	Jack OC 1	*/
#define PIN_JC_OC1_GPIO  (PIO_PA16_IDX)
#define PIN_JC_OC1_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT)

/*	Jack OC 2	*/
#define PIN_JC_OC2_GPIO  (PIO_PA15_IDX)
#define PIN_JC_OC2_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT)

/*	Jack detect 1	*/
#define PIN_JC_DC1_GPIO  (PIO_PA24_IDX)
#define PIN_JC_DC1_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT | PIO_PER_P24)

/*	Jack detect 2	*/
#define PIN_JC_DC2_GPIO  (PIO_PA25_IDX)
#define PIN_JC_DC2_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT | PIO_PER_P25)

/*	Jack EN 1	*/
#define PIN_JC_EN1_GPIO  (PIO_PA19_IDX)
#define PIN_JC_EN1_FLAGS (PIO_TYPE_PIO_OUTPUT_1 | PIO_PER_P19)

/*	Jack EN 2	*/
#define PIN_JC_EN2_GPIO  (PIO_PA8_IDX)
#define PIN_JC_EN2_FLAGS (PIO_TYPE_PIO_OUTPUT_1 | PIO_PER_P8)

/*	BLE RST 1	*/
#define PIN_BLE_RST1_GPIO  (PIO_PB10_IDX)
#define PIN_BLE_RST1_FLAGS (PIO_TYPE_PIO_OUTPUT_1 | PIO_PER_P10)

/*	BLE RST 2	*/
#define PIN_BLE_RST2_GPIO  (PIO_PA17_IDX)
#define PIN_BLE_RST2_FLAGS (PIO_TYPE_PIO_OUTPUT_1 | PIO_PER_P17)

/*	BLE RST 3	*/
#define PIN_BLE_RST3_GPIO  (PIO_PA18_IDX)
#define PIN_BLE_RST3_FLAGS (PIO_TYPE_PIO_OUTPUT_1 | PIO_PER_P18)

/*	Green LED	*/
#define PIN_GREEN_LED_GPIO  (PIO_PB13_IDX)
#define PIN_GREEN_LED_FLAGS (PIO_TYPE_PIO_OUTPUT_1 | PIO_PER_P13)

/*	Red LED	*/
#define PIN_RED_LED_GPIO  (PIO_PB1_IDX)
#define PIN_RED_LED_FLAGS (PIO_TYPE_PIO_OUTPUT_1 | PIO_PER_P1)

/*	Blue LED	*/
#define PIN_BLUE_LED_GPIO  (PIO_PB0_IDX)
#define PIN_BLUE_LED_FLAGS (PIO_TYPE_PIO_OUTPUT_1 | PIO_PER_P0)

/*	LBO	*/
#define PIN_LBO_GPIO  (PIO_PA11_IDX)
#define PIN_LBO_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT | PIO_PER_P11)

/*	STAT	*/
#define PIN_STAT_GPIO  (PIO_PA2_IDX)
#define PIN_STAT_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT | PIO_PER_P2)

/* SD CARD DETECT	*/
#define PIN_SD_CD_GPIO  (PIO_PB11_IDX)
#define PIN_SD_CD_FLAGS (PIO_IT_RISE_EDGE | PIO_TYPE_PIO_INPUT | PIO_PER_P11)

#endif /* GPIO_H_ */