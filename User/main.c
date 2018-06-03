//*****************************************
//	���ܣ�ÿ��5s��õ�ǰ�¶ȣ�����ӡ������
//	ģ�飺DS18B20�¶ȼ�⴫����
//	�����壺Ұ�����stm32f103vet6
//	�汾��V1.0(2018-06-04)
//	�������ߣ�ARM-Keil V5.25.2.0
//	���ߣ�JasonHwang
//	˵���������ο�������֤ȫ�ԡ�
//****************************************


#include "stm32f10x.h"
#include "uart_print.h"
#include "delay_func.h"
#include "ds18b20_func.h"
#include "stdio.h"

int main(void)
{
	uint8_t counter = 100;
	float temp = 0;
	/*���ڴ�ӡ��ʼ��*/
	Printer_Init();
	/*ϵͳ��ʱ����ʼ��*/
	SysTick_Init();
	while( DS18B20_Init() )
	{
		printf("sorry,I am going to init again\n");
		Delay_s(10);
	}
	while(counter != 0)
	{
		temp = DS18B20_GetTemp();
		printf("temp = %.5f\n\n", temp);
		counter--;
		Delay_s(5);
	}
	return 0;
}


