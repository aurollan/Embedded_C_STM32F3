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

	/* Enable GPIOD */
	RCC->AHBENR |= RCC_AHBENR_GPIODEN; // ok
	/* AF2 mode for GPIOD 12 */
	GPIOD->MODER |= (2 << 24);
	/* medium speed 0x01 mode for GPIOD 12 */
	GPIOD->OSPEEDR |= (1 << 24);
	/* Make sure type is push-pull */
	GPIOD->OTYPER &= ~(1 << 12);
	/* Make sure NoPuPd */
	GPIOD->PUPDR &= ~(3 << 24);
	/* Clean value before setting */
	GPIOD->AFR[1] &= 0x00000000;
	/* AF1 TIM16_CH1  Alternate function  GPIOD pin 12*/
	GPIOD->AFR[1] |= (2 << 16);

	/* Delay */
	TIM16_enable();
	pwm(20000, 1500);

	switch_on_leds();

	/* LED is always of except for 1s when interrupt occurs */
	while (1) {}
	return (0);
}


