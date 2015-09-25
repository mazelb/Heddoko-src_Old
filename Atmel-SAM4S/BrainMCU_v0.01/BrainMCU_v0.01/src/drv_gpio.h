/*
 * drv_gpio.h
 *
 * Created: 9/25/2015 8:12:59 AM
 *  Author: Sean Cloghesy
 */ 

#include "asf.h"
#include "common.h"

#ifndef DRV_GPIO_H_
#define DRV_GPIO_H_


#define DRV_GPIO_ID_PIN_SW0			PIO_PA2_IDX /* SOFTWARE BUTTON 0*/
#define DRV_GPIO_ID_PIN_PW_SW		PIO_PA0_IDX /*	Power button	*/
#define DRV_GPIO_ID_PIN_AC_SW1		PIO_PA1_IDX /*	Action Switch 1	*/
#define DRV_GPIO_ID_PIN_AC_SW2		PIO_PA20_IDX /*	Action Switch 2	*/
#define DRV_GPIO_ID_PIN_JC_OC1		PIO_PA16_IDX /*	Jack OC 1	*/
#define DRV_GPIO_ID_PIN_JC_OC2		PIO_PA15_IDX /*	Jack OC 2	*/
#define DRV_GPIO_ID_PIN_JC_DC1		PIO_PA24_IDX /*	Jack detect 1	*/
#define DRV_GPIO_ID_PIN_JC_DC2		PIO_PA25_IDX /*	Jack detect 2	*/
#define DRV_GPIO_ID_PIN_JC_EN1		PIO_PA19_IDX /*	Jack EN 1	*/
#define DRV_GPIO_ID_PIN_JC_EN2		PIO_PA8_IDX /*	Jack EN 2	*/
#define DRV_GPIO_ID_PIN_BLE_RST1	PIO_PB10_IDX /*	BLE RST 1	*/
#define DRV_GPIO_ID_PIN_BLE_RST2	PIO_PA17_IDX /*	BLE RST 2	*/
#define DRV_GPIO_ID_PIN_BLE_RST3	PIO_PA18_IDX /*	BLE RST 3	*/
#define DRV_GPIO_ID_PIN_GREEN_LED	PIO_PB13_IDX /*	Green LED	*/
#define DRV_GPIO_ID_PIN_BLUE_LED	PIO_PB0_IDX /*	Blue LED	*/
#define DRV_GPIO_ID_PIN_RED_LED		PIO_PB1_IDX /*	Red LED	*/
#define DRV_GPIO_ID_PIN_LBO			PIO_PA11_IDX /*	LBO	*/
#define DRV_GPIO_ID_PIN_STAT		PIO_PA2_IDX /*	STAT	*/
#define DRV_GPIO_ID_PIN_SD_CD		PIO_PB11_IDX /* SD CARD DETECT	*/

#define DRV_GPIO_PIN_SW0			PIO_PA2 /* SOFTWARE BUTTON 0*/
#define DRV_GPIO_PIN_PW_SW			PIO_PA0 /*	Power button	*/
#define DRV_GPIO_PIN_AC_SW1			PIO_PA1 /*	Action Switch 1	*/
#define DRV_GPIO_PIN_AC_SW2			PIO_PA20 /*	Action Switch 2	*/
#define DRV_GPIO_PIN_JC_OC1			PIO_PA16 /*	Jack OC 1	*/
#define DRV_GPIO_PIN_JC_OC2			PIO_PA15 /*	Jack OC 2	*/
#define DRV_GPIO_PIN_JC_DC1			PIO_PA24 /*	Jack detect 1	*/
#define DRV_GPIO_PIN_JC_DC2			PIO_PA25 /*	Jack detect 2	*/
#define DRV_GPIO_PIN_JC_EN1			PIO_PA19 /*	Jack EN 1	*/
#define DRV_GPIO_PIN_JC_EN2			PIO_PA8 /*	Jack EN 2	*/
#define DRV_GPIO_PIN_BLE_RST1		PIO_PB10 /*	BLE RST 1	*/
#define DRV_GPIO_PIN_BLE_RST2		PIO_PA17 /*	BLE RST 2	*/
#define DRV_GPIO_PIN_BLE_RST3		PIO_PA18 /*	BLE RST 3	*/
#define DRV_GPIO_PIN_GREEN_LED		PIO_PB13 /*	Green LED	*/
#define DRV_GPIO_PIN_BLUE_LED		PIO_PB0 /*	Blue LED	*/
#define DRV_GPIO_PIN_RED_LED		PIO_PB1 /*	Red LED	*/
#define DRV_GPIO_PIN_LBO			PIO_PA11 /*	LBO	*/
#define DRV_GPIO_PIN_STAT			PIO_PA2 /*	STAT	*/
#define DRV_GPIO_PIN_SD_CD			PIO_PB11 /* SD CARD DETECT	*/
	
typedef enum 
{
	DRV_GPIO_PIN_MODE_OUTPUT,
	DRV_GPIO_PIN_MODE_INPUT
}drv_gpio_pinMode_t;
typedef enum
{
	DRV_GPIO_INTERRUPT_HIGH_EDGE, 
	DRV_GPIO_INTERRUPT_LOW_EDGE,
	DRV_GPIO_INTERRUPT_HIGH_LVL, 
	DRV_GPIO_INTERRUPT_LOW_LVL,
	DRV_GPIO_INTERRUPT_NONE	
}drv_gpio_interrupt_t;

typedef enum
{
	DRV_GPIO_PIN_STATE_LOW,
	DRV_GPIO_PIN_STATE_HIGH	
}drv_gpio_pin_state_t;

typedef struct
{
	uint32_t pinId;
	uint32_t pinMask;
	drv_gpio_pinMode_t pinMode; 
	drv_gpio_pin_state_t initialPinState;
	drv_gpio_interrupt_t interruptType; 
	void* interruptHandler;
	uint8_t pullUpEnabled;
	uint8_t debounceEnabled; 	
}drv_gpio_config_t;


status_t drv_gpio_initializeAll(void); 
status_t drv_gpio_config(drv_gpio_config_t* gpioConfig);
status_t drv_gpio_setPinState(uint32_t pinId, drv_gpio_pin_state_t state);
status_t drv_gpio_getPinState(uint32_t pinId, drv_gpio_pin_state_t* state);
status_t drv_gpio_togglePin(uint32_t pinId);

//status_t drv_uart_isInit(drv_gpio_config_t* gpioConfig);

#endif /* DRV_GPIO_H_ */