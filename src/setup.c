/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   setup.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:33:45 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/29 16:39:37 by aurollan    ###    #+. /#+    ###.fr     */
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
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;            /*!< USART1 clock enable */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

void GPIOA_enable()
{
	/* Connect PA10 to USART1_Tx */
	// MODER A = GPIO_Mode_AF
	GPIOA->MODER = 0xA0000;      /*!< GPIO port mode register,                                   */
	GPIOA->OTYPER = 0x00000000;       /*!< GPIO port output type register in push pull mode */
	GPIOA->OSPEEDR = 0xFFFFFFFF;      /*!< GPIO port output speed register to maximum speed */
	GPIOA->PUPDR = 0x0000;        /*!< GPIO port pull-up/pull-down register,                      */

}

// #define GPIO_AF_7            	   ((uint8_t)0x07) /* AOP2_OUT, CAN, COMP3_OUT, COMP5_OUT, COMP6_OUT, USART1 */
// #define GPIO_PinSource9            ((uint8_t)0x09)
// #define GPIO_PinSource10           ((uint8_t)0x0A)
void GPIOA_connect(uint32_t pin)
{
	uint32_t index = 0;
	uint32_t offset = 0;
	uint32_t alternate_function = 0;
	/* On regarde dans que index du tableau se trouve la led */
	if (pin > 7 )
	{
		index = 1;
	}
	/* Par rapport a son nombre on regarde sa position */
	pin = pin % 8;
	offset = pin * 4;
	alternate_function = 0x07;	
	/* On va reinitialiser les 4 bits a 0 avant de les attribuer  */
	/* Apres invesion les seuls bits a zero sont ceux de la pin  puis on fait un & pour conserver les autres et reset ceux de la pin*/
	GPIOA->AFR[index] &= ~(0xF << offset);
	/* On attribut */
	GPIOA->AFR[index] |= (alternate_function << offset);
}

void USART_enable()
{
	RCC_USART1_enable();
	GPIOA_connect((uint32_t)9);
	GPIOA_connect((uint32_t)10);
	GPIOA_enable();

	/* Clear USART1->CR1 */
  /* Clear M, PCE, PS, TE and RE bits */
	USART1->CR1= (uint32_t)~((uint32_t)CR1_CLEAR_MASK);
	/* 8 bit words, no changes needed m0 = 0 and m1 = 0  */
	/* oversampling to 16 */
	/* Parity control disabled  */
	/* tx enabled */
	USART1->CR1 |= (1 << 3);
	/* rx enabled */
	USART1->CR1 |= (1 << 2);
	USART1->BRR = 72000000/9600;
	/* USART enable  */ 
	USART1->CR1 |= (1 << 0); 
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
