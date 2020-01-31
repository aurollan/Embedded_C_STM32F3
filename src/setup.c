/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   setup.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:33:45 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/31 17:37:35 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

/*!< USART CR1 register clear Mask ((~(uint32_t)0xFFFFE6F3)) */
#define CR1_CLEAR_MASK            ((uint32_t)(USART_CR1_M | USART_CR1_PCE | \
                                              USART_CR1_PS | USART_CR1_TE | \
                                              USART_CR1_RE))

#include "stm32f30x_it.h"
#include "stm32f30x_conf.h"

void RCC_USART1_enable()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;            /*!< USART1 clock enable */
}

void GPIOA_connect_PIN9_PIN10()
{
	/* Cleaning bits */
	GPIOA->AFR[1] &= ~(0xF << ((9 % 8) * 4)); // le mask est sur 4 bits
	GPIOA->AFR[1] &= ~(0xF << ((10 % 8) * 4)); // le mask est sur 4 bits
	/* Setting bits */
	GPIOA->AFR[1] |= (GPIO_AF_7 << (9 % 8) * 4);
	GPIOA->AFR[1] |= (GPIO_AF_7 << (10 % 8) * 4);
}

void GPIOA_config()
{
	GPIOA->MODER |= GPIO_Mode_AF << 9 * 2; // alternate function mode
	GPIOA->MODER |= GPIO_Mode_AF << 10 * 2; // alternate function mode
	GPIOA->OTYPER |= GPIO_OType_PP << 9;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOA->OTYPER |= GPIO_OType_PP << 10;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOA->OSPEEDR |= GPIO_Speed_Level_3 << 9 * 2;      /*!< GPIO port output speed register to maximum speed */
	GPIOA->OSPEEDR |= GPIO_Speed_Level_3 << 10 * 2;      /*!< GPIO port output speed register to maximum speed */
	// PUPD_UP ? A VERIFIER
	GPIOA->PUPDR |= GPIO_PuPd_NOPULL << 9 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
	GPIOA->PUPDR |= GPIO_PuPd_NOPULL << 10 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
}

void USART_enable()
{

  /* Clear M, PCE, PS, TE and RE bits */
	USART1->CR1 &= (uint32_t)~((uint32_t)CR1_CLEAR_MASK);
	/* Parity control disabled  */
	USART1->CR1 &= ~USART_CR1_PCE;
	/* oversampling to 16 */
	USART1->CR1 &= ~USART_CR1_OVER8;
	/* 8 bit words, no changes needed m0 = 0 and m1 = 0  */
	USART1->CR1 &= ~USART_CR1_M;
	/* tx enabled */
	USART1->CR1 |= USART_CR1_TE;
	/* rx enabled */
	USART1->CR1 |= USART_CR1_RE;
	//default settings give you no hardware flow control, 8 data bits, no parity and one stop bit
	USART1->BRR = 72000000/115200;
	/* USART enable  */ 
	USART1->CR1 |= USART_CR1_UE; // 0x1 
	// USART1->CR2 &= ~(USART_CR2_STOP_1 | USART_CR2_STOP_0);
}

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

void ITM_init(void)
{
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

int _write(int32_t file, char* ptr, int32_t len)
{
	(void)file;
	int i=0;
	for(i=0 ; i<len ; i++)
		ITM_SendChar((*ptr++));
	return i;
}
