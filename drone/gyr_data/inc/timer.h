#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f303xc.h"

void	TIM6_enable(void);
void	TIM7_enable(void);
void	delay(uint16_t ms);
uint16_t  get_time();
#endif
