/*
 * drv_gpio.h
 *
 * Created: 9/24/2015 12:02:59 PM
 *  Author: Hriday Mehta
 */ 

#include "asf.h"
#include "common.h"

#ifndef DRV_GPIO_H_
#define DRV_GPIO_H_

typedef struct
{
	Usart *p_usart;
	usart_serial_options_t uart_options;
	int mem_index;
}drv_gpio_config_t;

status_t drv_uart_init(drv_gpio_config_t* gpioConfig);
status_t drv_uart_putChar(drv_gpio_config_t* gpioConfig, char c);
status_t drv_uart_getChar(drv_gpio_config_t* gpioConfig, char* c);
status_t drv_uart_deInit(drv_gpio_config_t* gpioConfig);
status_t drv_uart_isInit(drv_gpio_config_t* gpioConfig);

#endif /* DRV_GPIO_H_ */