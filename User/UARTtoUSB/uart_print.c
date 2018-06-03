#include "uart_print.h"


/*
//��������Printer_Config()
//���ܣ�ʹ��RCC����ʼ��GPIO�ڡ�ʹ��USART
//���룺null
//�����null
//���䣺null
*/
void Printer_Init(void)
{
	/*ʹ��RCC*/
	RCC_APB2PeriphClockCmd(RCC_GPIO_PRINT, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_USART_PRINT, ENABLE);

	/*���á���ʼ��TX��RX��Ӧ��GPIO��*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_PRINT_TX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PRINT_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_PRINT_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIO_PRINT_PORT, &GPIO_InitStruct);
	
	/*���á���ʼ����ʹ��USART*/
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
//��������fputc(int ch, FILE *f)
//���ܣ���printf()������ӡ�����ض��򵽴���
//���룺null
//�����null
//���䣺FILE���Ϳ��ܻ����δ�����������Ҫ���¶��塰typedef struct __FILE FILE;��
*/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART_PRINT_PORT, (uint8_t)ch);
	while(USART_GetFlagStatus(USART_PRINT_PORT, USART_FLAG_TXE) == RESET);
	return ch;
}

