#include "leds.h"

/* Call before using LEDS */
void	init_leds(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	GPIOE->MODER = 0x55550000;

}

void switch_on_leds(void)
{
	GPIOE->BSRR = (1 << 15);
	GPIOE->BSRR = (1 << 14);
	GPIOE->BSRR = (1 << 13);
	GPIOE->BSRR = (1 << 12);
	GPIOE->BSRR = (1 << 11);
	GPIOE->BSRR = (1 << 10);
	GPIOE->BSRR = (1 << 9);
	GPIOE->BSRR = (1 << 8);
}

/* Switch off all leds using BRR register */
void switch_off_leds(void)
{
	GPIOE->BRR = (1 << 15);
	GPIOE->BRR = (1 << 14);
	GPIOE->BRR = (1 << 13);
	GPIOE->BRR = (1 << 12);
	GPIOE->BRR = (1 << 11);
	GPIOE->BRR = (1 << 10);
	GPIOE->BRR = (1 << 9);
	GPIOE->BRR = (1 << 8);
}
