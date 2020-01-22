/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   setup.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:33:45 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/22 18:11:10 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "stm32f30x_it.h"
#include "stm32f30x_conf.h"


void TIM6_enable(void)
{
	/* Enable TIM6 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	// SET OPM
	TIM6->CR1 |= (uint16_t)(1 << 3);
}

/* Delay function using system clock */
/* SR, the status register. */
/* EGR, the event generation register. */
/* CNT, the counter register. */
/* PSC, the prescaler register. */
/* ARR, the autoreload register. */
void delay(uint16_t ms)
{
	/* Set PSC frequency */
	TIM6->PSC = (uint16_t)799;
	/* Value to reach */
	TIM6->ARR = ms;
	/* enable clock */
	TIM6->CR1 |= (uint16_t)1;
	/* wait event signaling that counter has reach value */
	while (TIM6->SR == 0);
	/* Reset event */
	TIM6->SR = 0;
}

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
