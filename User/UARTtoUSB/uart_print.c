#include "uart_print.h"


/*
//函数名：Printer_Config()
//功能：使能RCC、初始化GPIO口、使能USART
//输入：null
//输出：null
//补充：null
*/
void Printer_Init(void)
{
	/*使能RCC*/
	RCC_APB2PeriphClockCmd(RCC_GPIO_PRINT, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_USART_PRINT, ENABLE);

	/*配置、初始化TX和RX对应的GPIO口*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_PRINT_TX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PRINT_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_PRINT_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIO_PRINT_PORT, &GPIO_InitStruct);
	
	/*配置、初始化、使能USART*/
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = USART_PRINT_BaudRate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART_PRINT_PORT, &USART_InitStruct);
	USART_Cmd(USART_PRINT_PORT, ENABLE);
}

/*
//函数名：fputc(int ch, FILE *f)
//功能：将printf()函数打印方向重定向到串口
//输入：null
//输出：null
//补充：FILE类型可能会出现未定义情况，需要重新定义“typedef struct __FILE FILE;”
*/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART_PRINT_PORT, (uint8_t)ch);
	while(USART_GetFlagStatus(USART_PRINT_PORT, USART_FLAG_TXE) == RESET);
	return ch;
}

