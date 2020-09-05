/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/04 17:16:55 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "timer.h"
#include "pwm.h"

int main(void)
{
	/* LED */
	init_leds();

	/* Enable GPIOB */
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	/* AF mode for GPIOB 4 */
	GPIOB->MODER |= (2 << 8);
	/* medium speed mode for GPIOB 4 */
	GPIOB->OSPEEDR |= (1 << 8);
	/* Make sure type is push-pull */
	GPIOB->OTYPER &= ~(1 << 4);
	/* Make sure pull down default state ? maybe useless */
	GPIOB->PUPDR |= (1 << 8);
	/* Clean value before setting */
	GPIOB->AFR[0] &= 0x000F0000;
	/* AF1 TIM16_CH1  Alternate function  GPIOB pin 4*/
	GPIOB->AFR[0] |= (1 << 16);

	/* Delay */
	TIM16_enable();
	pwm(20, 2);

	switch_on_leds();

	/* LED is always of except for 1s when interrupt occurs */
	while (1) {}
	return (0);
}

