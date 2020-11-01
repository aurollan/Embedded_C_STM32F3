#include "pwm.h"
void TIM16_enable(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
}

void pwm(uint16_t frequency, uint16_t duty_cycle)
{
	/* This counter counts ms */
	/* Frequency of the signal in us */
	TIM4->ARR = frequency; /* OK */
	/* duty cycle of the signal in us */
	TIM4->CCR1 = duty_cycle; /* OK */
	/* prescaler set to 7 + 1 = each 8 000 000 / 8 = 1 000 000
	 * clock cycle we count 1 ms */
	TIM4->PSC = (uint16_t)7; /* OK */

	TIM4->CCER &= (uint32_t)~TIM_CCER_CC1E;

	/* Set CCR to output mode */
	TIM4->CCMR1 &= ~((uint16_t)0x3);
	/* 0111: PWM mode 2 - In upcounting, channel 1 is inactive as long as
	   TIMx_CNT<TIMx_CCR1 else active. In downcounting, channel 1 is active as long as
	   TIMx_CNT>TIMx_CCR1 else inactive. */
	TIM4->CCMR1 |= 0x70;
	/* PWM mode 1 Channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive */
	//TIM4->CCMR1 |= (uint16_t)(0x6 << 4); /* NOT WORKING */

	/* Output polarity low */
	TIM4->CCER |= 0x2;

	/* Output state 1: On - OC1 signal is output on the corresponding output pin*/
	TIM4->CCER |= 0x1;
	///* Enable preload register for CCR1 if preload bit is setm new CCR1 value will be load at next event */
	//TIM4->CCMR1 |= (uint16_t)(0x1 << 3);
	


	/* Must enable it if upcounting ARPE ? Auto reload preload enable for ARR register to load it after each event and not directly */
	/* TIM counter up 0x00 et TIM Clock div 0x00 */
	TIM4->CR1 = (uint16_t)(1 << 7);

	/* At the end when register are set put this bit because preload register are load when event occurm we set a software event before starting the counter (you have to initialize all the registers) */
	TIM4->EGR |= (uint16_t)(0x1 << 0);

	/* enable counter */
	TIM4->CR1 |= (1 << 0);
}

