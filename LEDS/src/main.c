/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/06 16:07:13 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "stm32f30x_it.h"
#include "stm32f30x.h"

void	leds(void)
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

	/* Light up all leds with ODR register */
	/*    AHB2    +  GPIOE + ODR |= (1 << led_number) */
	/* 0x48000000 + 0x1000 + 0x14  |= (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8); */
	GPIOE->ODR = (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8);

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

int main(void)
{
	leds();
	return (0);
}
