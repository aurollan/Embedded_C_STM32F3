#include "leds.h"

/* Call before using LEDS */
void	init_leds(void)
{
	/* Enable GPIOE */
	/*    AHB1    +    RCC  +  AHBENR |= IOPEEN    */
	/* 0x40020000 +  0x1000 +   0x14  |= (1 << 21) */
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;

	/* Set all GPIOE to output mode 							*/
	/* GPIO mode register, set all value to 01 (output mode)	*/
	/*    AHB2    +  GPIOE + MODER |= (1 << (led_number * 2))	*/
	/* 0x48000000 + 0x1000 + 0x00  |= (1 << 30) | (1 << 28) | (1 << 26) | (1 << 24) | (1 << 22) | (1 << 20) | (1 << 18) | (1 << 16)	*/
	GPIOE->MODER = 0x55550000;

}

/* Switch on all leds using ODR or BSRR register chose one because yusing both 
 * is useless */
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
	/* First way */
	GPIOE->BRR = (1 << 15);
	GPIOE->BRR = (1 << 14);
	GPIOE->BRR = (1 << 13);
	GPIOE->BRR = (1 << 12);
	GPIOE->BRR = (1 << 11);
	GPIOE->BRR = (1 << 10);
	GPIOE->BRR = (1 << 9);
	GPIOE->BRR = (1 << 8);
}
