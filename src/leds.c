/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   Leds.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/24 17:14:31 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/24 17:16:04 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "drone.h"

void	leds(void)
{
	GPIOE_enable();
	GPIOE_full_init();
	while (1)
	{
		GPIOE->BSRR = (1 << 15);
		delay(10000);
		GPIOE->BSRR = (1 << 14);
		GPIOE->BSRR = (1 << (15 + 16));
		delay(10000);
		GPIOE->BSRR = (1 << 13);
		GPIOE->BSRR = (1 << (14 + 16));
		delay(10000);
		GPIOE->BSRR = (1 << 12);
		GPIOE->BSRR = (1 << (13 + 16));
		delay(10000);
		GPIOE->BSRR = (1 << 11);
		GPIOE->BSRR = (1 << (12 + 16));
		delay(10000);
		GPIOE->BSRR = (1 << 10);
		GPIOE->BSRR = (1 << (11 + 16));
		delay(10000);
		GPIOE->BSRR = (1 << 9);
		GPIOE->BSRR = (1 << (10 + 16));
		delay(10000);
		GPIOE->BSRR = (1 << 8);
		GPIOE->BSRR = (1 << (9 + 16));
		delay(10000);
		GPIOE->BSRR = (1 << (8 + 16));
	}
}
