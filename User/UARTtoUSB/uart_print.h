#ifndef __UART_PRINT_H
#define __UART_PRINT_H

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

#define RCC_GPIO_PRINT 					RCC_APB2Periph_GPIOA
#define RCC_USART_PRINT				  RCC_APB2Periph_USART1
#define GPIO_PRINT_PORT					GPIOA
#define GPIO_PRINT_TX_PIN				GPIO_Pin_9
#define GPIO_PRINT_RX_PIN				GPIO_Pin_10
#define USART_PRINT_BaudRate		115200
#define USART_PRINT_PORT				USART1

typedef struct __FILE FILE;

void Printer_Init(void);
int putc(int ch, FILE *f);

#endif  /*__UART_PRINT_H*/

