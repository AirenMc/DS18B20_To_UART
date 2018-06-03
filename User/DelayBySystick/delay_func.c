#include "delay_func.h"

uint32_t Timer_Counter = 0;

/*
//函数名：SysTick_Init()
//功能：配置系统定时器，使得单次计算单位为μs
//输入：null
//输出：null
//补充：SystemCoreClock/1			:以 s 为时间单位
//  		SystemCoreClock/1000		:以 ms 为时间单位
//      SystemCoreClock/1000000:以 μs 为时间单位
*/
void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock/1000000))
		while(1);
}


/*
//函数名：TimerCounter_Decrement()
//功能：中断服务函数里被触发，当 Timer_Counter 值为0则表示延迟时间已达到。
//输入：null
//输出：null
//补充：
*/
void TimerCounter_Decrement(void)
{
	if(Timer_Counter != 0)
		Timer_Counter--;
}


/*
//函数名：Delay_us()
//功能：微秒级延迟
//输入：Time_us --- 指定延迟的微秒数
//输出：null
//补充：
*/
void Delay_us(uint32_t Time_us)
{
	Timer_Counter = Time_us;
	while(Timer_Counter != 0);
}


/*
//函数名：Delay_ms()
//功能：毫秒级延迟
//输入：Time_us --- 指定延迟的毫秒数
//输出：null
//补充：
*/
void Delay_ms(uint32_t Time_ms)
{
	Timer_Counter = Time_ms * 1000;
	while(Timer_Counter != 0);
}

/*
//函数名：Delay_s()
//功能：秒级延迟
//输入：Time_us --- 指定延迟的秒数
//输出：null
//补充：
*/
void Delay_s(uint32_t Time_s)
{
	Timer_Counter = Time_s * 1000000;
	while(Timer_Counter != 0);
}
