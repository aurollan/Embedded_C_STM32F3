#include "pwm.h"
void TIM16_enable(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
}

void pwm(uint16_t frequency, uint16_t duty_cycle)
{
	/* This counter counts ms */
	/* Frequency of the signal */
	TIM16->ARR = frequency;
	/* duty cycle of the signal */
	TIM16->CCR1 = duty_cycle;
	/* prescaler set to 7999 + 1 = each 8 000 000 / 8 000 = 1 000 
	 * clock cycle we count 1 ms */
	TIM16->PSC = (uint16_t)7999;

	/* Set CCR to output mode */
	TIM16->CCMR1 &= ~((uint16_t)0x3);
	/* PWM mode 1 Channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive */
	TIM16->CCMR1 |= (uint16_t)(0x6 << 4);
	/* Enable preload register for CCR1 if preload bit is setm new CCR1 value will be load at next event */
	TIM16->CCMR1 |= (uint16_t)(0x1 << 3);
	/* Must enable it if upcounting ARPE ? Auto reload preload enable for ARR register to load it after each event and not directly */
	TIM16->CR1 = (uint16_t)(1 << 7);
	/* because preload register are load when event occurm we set a software event before starting the counter (you have to initialize all the registers) */
	TIM16->EGR |= (uint16_t)(0x1 << 0);

	/* active high */
	TIM16->CCER |= (uint16_t)(0x1 << 1);
	/* active OC1 CC1E? */
	TIM16->CCER |= (uint16_t)(0x1 << 0);
	/* Bit 2 CC1NE */
	TIM16->CCER |= (uint16_t)(0x1 << 2);
	/* make sure OC1N hight is active (polarity) */
	TIM16->CCER &= ~(uint16_t)(0x1 << 3);
	/* MOE 1: OC and OCN outputs are enabled if their respective enable bits are set (CCxE, CCxNE in
TIMx_CCER register)*/
	TIM16->BDTR = (1 << 15);

	/* enable counter */
	TIM16->CR1 |= (1 << 0);
}

