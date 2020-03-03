/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   leds.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/24 17:14:31 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/04 17:45:31 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "drone.h"

/* Enable led-compass led */
/* REFERENCE MANUAL P166 RCC register map */
void GPIOE_enable(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
}

/* Initialize all led form led-compass */
/* REFERENCE MANUAL P228 General-purpose I/Os (GPIO)*/
void GPIOE_full_init(void)
{
  GPIOE->MODER = 0x55555555;      /*!< GPIO port mode register,                                   */
  GPIOE->OTYPER = 0x0000;       /*!< GPIO port output type register,                            */
  GPIOE->OSPEEDR = 0xFFFFFFFF;      /*!< GPIO port output speed register,                           */
  GPIOE->PUPDR = 0x0000;        /*!< GPIO port pull-up/pull-down register,                      */
}

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
