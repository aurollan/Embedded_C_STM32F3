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
	/* First way */
	/* Light up all leds with ODR register */
	/*    AHB2    +  GPIOE + ODR |= (1 << led_number) */
	/* 0x48000000 + 0x1000 + 0x14  |= (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8); */
	GPIOE->ODR = (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8);

	/* Second way */
	/* Light up all leds with BSRR register */
	/*    AHB2    +  GPIOE +  ODR  |= (1 << led_number)	*/
	/* 0x48000000 + 0x1000 + 0x14  |= (1 << led_number)	*/
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

	/* Second way */
	GPIOE->BSRR = (1 << (15 + 16));
	GPIOE->BSRR = (1 << (14 + 16));
	GPIOE->BSRR = (1 << (13 + 16));
	GPIOE->BSRR = (1 << (12 + 16));
	GPIOE->BSRR = (1 << (11 + 16));
	GPIOE->BSRR = (1 << (10 + 16));
	GPIOE->BSRR = (1 << (9 + 16));
	GPIOE->BSRR = (1 << (8 + 16));

	/* Third way */
	/* Set all leds bit to 0 in ODR */
	GPIOE->ODR &= ~((1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8));
}
