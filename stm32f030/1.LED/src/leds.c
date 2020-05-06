#include "stm32f0xx.h"

/* Call before using LEDS */
void	init_leds(void)
{
	RCC->AHBENR |= (1 << 19);
	GPIOC->MODER |= (1 << 26);
	GPIOC->OTYPER &= ~(1 << 13);
	GPIOC->OSPEEDR |= (1 << 26);
	GPIOC->PUPDR &= ~(3 << 26);
}

/* Switch on all leds using ODR or BSRR register chose one because yusing both 
 * is useless */
void switch_on_leds(void)
{
	// GPIOC->ODR = (1 << 13);
	GPIOC->BSRR = (1 << 13);
}

/* Switch off all leds using BRR register */
void switch_off_leds(void)
{
	GPIOC->ODR &= ~(1 << 13);
}
