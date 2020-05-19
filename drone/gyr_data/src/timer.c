#include "timer.h"

void TIM6_enable(void)
{
	/* Enable TIM6 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	// SET OPM
	TIM6->CR1 |= (uint16_t)(1 << 3);
}

/* Delay function using system clock */
/* SR, the status register. */
/* EGR, the event generation register. */
/* CNT, the counter register. */
/* PSC, the prescaler register. */
/* ARR, the autoreload register. */
void delay(uint16_t ms)
{
	/* Set PSC frequency */
	TIM6->PSC = (uint16_t)7999;
	/* Value to reach */
	TIM6->ARR = ms;
	/* enable clock */
	TIM6->CR1 |= (uint16_t)1;
	/* wait event signaling that counter has reach value */
	while (TIM6->SR == 0);
	/* Reset event */
	TIM6->SR = 0;
}

void TIM7_enable(void)
{
	/* Enable TIM7 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

	/* Set PSC frequency */
	/* clock is 8 Mhz  each 7999 + 1 count we count 1 ms */
	TIM7->PSC = (uint16_t)7999;

	/* Value to reach  = 65s */
	TIM7->ARR = 0xFFFF;

	/* enable clock */
	TIM7->CR1 |= (uint16_t)1;
}

uint16_t  get_time()
{
	return (TIM7->CNT);
}
