/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   setup.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:33:45 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/24 14:15:11 by aurollan    ###    #+. /#+    ###.fr     */
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

/*void ITM_init(void)
{
	ITM->LAR = 0xC5ACCE55;
	ITM->TER = 0x0;
	ITM->TCR = 0x0;
	TPI->SPPR = 2;
	TPI->ACPR = 0xf;
	ITM->TPR = 0x0;
	DWT->CTRL = 0x400003FE;
	ITM->TCR = 0x0001000D;
	TPI->FFCR = 0x00000100;
	ITM->TER = 0x1;
}*/

void ITM_init(void)
{
	// TRACESWO -> SWJ-DP -> AHB-AP -> ITM -> TPIU -> TRACESWO
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	TPI->ACPR = 72000000 / 2000000 - 1;
	TPI->FFCR |= 0x100; // default value bti 8 always at 1 and bit 2 activate formatter
	TPI->SPPR = 2; // other asynchronous value : NRZ
	DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
	ITM->TPR = ITM_TPR_PRIVMASK_Msk;
	ITM->LAR = 0xC5ACCE55;						/* ITM Lock Access Register */
	TPI->CSPSR |= (1 << 0); // 0x1 for 1 pin (default)
	ITM->TCR |= ITM_TCR_ITMENA_Msk | (1 << 3) | (1 << 16);     /* ITM enabled */
	ITM->TER |= (1UL << 0); 			/* ITM Port #0 enabled */

	/* Trace Control Rgeister | Msk ITM ENAble */
	// TODO : previous value 0x10009 unchanged after |
	// ITM->TCR = 0x00010005;
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
