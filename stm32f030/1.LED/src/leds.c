#include "stm32f0xx.h"

/* Call before using LEDS */
void	init_leds(void)
{
	// Set IOPCEN to enable GPIOC
	RCC->AHBENR |= (1 << 19);

	// Output push pull for PC13
	GPIOC->OTYPER &= ~(1 << 13);
	// low speed for PC13 
	GPIOC->OSPEEDR &= ~(3 << 26);
	// No PUPDR for PC13
	GPIOC->PUPDR &= ~(3 << 26);
	// General purpose output mode for PC13
	GPIOC->MODER |= (1 << 26);
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
