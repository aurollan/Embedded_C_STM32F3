#include "timer.h"

void TIM6_enable(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->CR1 |= (uint16_t)(1 << 3);
}

void delay(uint16_t ms)
{
	TIM6->PSC = (uint16_t)7999;
	TIM6->ARR = ms;
	TIM6->CR1 |= (uint16_t)1;
	while (TIM6->SR == 0);
	TIM6->SR = 0;
}
