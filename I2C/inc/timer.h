#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f30x.h"
#include "stm32f30x_it.h"

void	TIM6_enable(void);
void	delay(uint16_t ms);
#endif
