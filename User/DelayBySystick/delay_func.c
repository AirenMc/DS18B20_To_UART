#include "delay_func.h"

uint32_t Timer_Counter = 0;

/*
//��������SysTick_Init()
//���ܣ�����ϵͳ��ʱ����ʹ�õ��μ��㵥λΪ��s
//���룺null
//�����null
//���䣺SystemCoreClock/1			:�� s Ϊʱ�䵥λ
//  		SystemCoreClock/1000		:�� ms Ϊʱ�䵥λ
//      SystemCoreClock/1000000:�� ��s Ϊʱ�䵥λ
*/
void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock/1000000))
		while(1);
}


/*
//��������TimerCounter_Decrement()
//���ܣ��жϷ������ﱻ�������� Timer_Counter ֵΪ0���ʾ�ӳ�ʱ���Ѵﵽ��
//���룺null
//�����null
//���䣺
*/
void TimerCounter_Decrement(void)
{
	if(Timer_Counter != 0)
		Timer_Counter--;
}


/*
//��������Delay_us()
//���ܣ�΢�뼶�ӳ�
//���룺Time_us --- ָ���ӳٵ�΢����
//�����null
//���䣺
*/
void Delay_us(uint32_t Time_us)
{
	Timer_Counter = Time_us;
	while(Timer_Counter != 0);
}


/*
//��������Delay_ms()
//���ܣ����뼶�ӳ�
//���룺Time_us --- ָ���ӳٵĺ�����
//�����null
//���䣺
*/
void Delay_ms(uint32_t Time_ms)
{
	Timer_Counter = Time_ms * 1000;
	while(Timer_Counter != 0);
}

/*
//��������Delay_s()
//���ܣ��뼶�ӳ�
//���룺Time_us --- ָ���ӳٵ�����
//�����null
//���䣺
*/
void Delay_s(uint32_t Time_s)
{
	Timer_Counter = Time_s * 1000000;
	while(Timer_Counter != 0);
}
