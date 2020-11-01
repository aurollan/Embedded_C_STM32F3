#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f3xx.h"
#include "itm.h"
#include "timer.h"
#include "spi.h"

void	TIM6_enable(void);
void	delay(uint16_t ms);
#endif
