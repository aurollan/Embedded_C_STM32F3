#include "stm32f1xx.h"

/* Call before using LEDS */
void	init_leds(void)
{
	// Set IOPCEN to enable GPIOC
	RCC->APB2ENR |= (1 << 4);

	// CNF General purpose output push-pull 
	GPIOC->CRH &= ~(3 << 22);

	// MODE Output mode, max speed 2 Mhz
	GPIOC->CRH |= (2 << 20);
}

void switch_high_leds(void)
{
	// Switch on PC13 with ODR register
	GPIOC->ODR = (1 << 13);
	// Switch on PC13 with BSRR register
	GPIOC->BSRR = (1 << 13);
}

void switch_low_leds(void)
{
	// Switch off PC13 with ODR register
	GPIOC->ODR &= ~(1 << 13);
	// Switch off PC13 with BSRR register
	GPIOC->BSRR = (1 << 26);
	// Switch off PC13 with BRR register
	GPIOC->BRR = (1 << 13);
}
