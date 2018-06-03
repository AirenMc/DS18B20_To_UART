#ifndef __DS18B20_FUNC_H
#define __DS18B20_FUNC_H

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay_func.h"
#include "uart_print.h"
#include "stdio.h"

#define HIGH 											1
#define LOW												0

#define RCC_GPIO_DS18B20					RCC_APB2Periph_GPIOE
#define GPIO_DS18B20_PORT					GPIOE
#define GPIO_DS18B20_PIN					GPIO_Pin_6

#define DS18B20_DATA_OUT(LEVEL)		if(LEVEL == LOW) GPIO_ResetBits(GPIO_DS18B20_PORT, GPIO_DS18B20_PIN);\
																		else GPIO_SetBits(GPIO_DS18B20_PORT, GPIO_DS18B20_PIN)

uint8_t DS18B20_Init(void);
float DS18B20_GetTemp(void);																	
																		
#endif  /*__DS18B20_FUNC_H*/
