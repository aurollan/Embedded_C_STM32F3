void TIM16_enable(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
}

void pwm(uint16_t frequency, uint16_t duty_cycle)
{
	/* Frequency of the signal */
	TIM16->ARR = frequency;
	/* duty cycle of the signal */
	TIM16->CCRx = duty_cycle;
	/* prescaler set to 7999 + 1 = each 8 000 000 / 8 000 = 1 000 
	 * clock cycle we count 1 ms */
	TIM16->PSC = (uint16_t)7999;
	/* enable timer */
	TIM16->CR1 |= (uint16_t)1;
	/* Wait timer status say done */
	while (TIM16->SR == 0);
	/* Reset timer status */
	TIM16->SR = 0;
	/* Break and dead timer register */
	TIM16->BDTR = ;

	/* Set CCR to output mode */
	TIM16->CCRM &= ~((uint16_t)0x1);
	/* PWM mode 1 Channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive */
	TIM16->CCRM |= (uint16_t)(0x6 << 4);
	/* Mustg enable it if upcounting ARPE ? Auto reload preload enable */
	TIM16->CR1 = (uint16_t)(1 << 7);
	/* Enable preload register */
	TIM16->CCRM |= (uint16_t)(0x1 << 3);
	/* before starting the counter, you have to initialize all the registers */
	TIM16->EGR |= (uint16_t)(0x1 << 0);


}

