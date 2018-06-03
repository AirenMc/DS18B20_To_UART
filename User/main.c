//*****************************************
//	功能：每隔5s获得当前温度，并打印到串口
//	模块：DS18B20温度检测传感器
//	开发板：野火秉火stm32f103vet6
//	版本：V1.0(2018-06-04)
//	开发工具：ARM-Keil V5.25.2.0
//	作者：JasonHwang
//	说明：仅供参考，不保证全对。
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
	/*串口打印初始化*/
	Printer_Init();
	/*系统定时器初始化*/
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


