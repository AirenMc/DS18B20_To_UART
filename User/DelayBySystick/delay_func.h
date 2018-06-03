#ifndef __DELAY_FUNC_H
#define __DELAY_FUNC_H

#include "stm32f10x.h"
#include "core_cm3.h"

extern uint32_t Timer_Counter;

void SysTick_Init(void);
void TimerCounter_Decrement(void);
void Delay_us(uint32_t Time_us);
void Delay_ms(uint32_t Time_ms);
void Delay_s(uint32_t Time_s);

#endif /*__DELAY_FUNC_H*/

