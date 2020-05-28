#include "uart.h"

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
	GPIOA->AFR[1] |= (0x07 << (9 % 8) * 4);
	GPIOA->AFR[1] |= (0x07 << (10 % 8) * 4);
}

void GPIOA_config()
{
	GPIOA->MODER |= GPIO_Mode_AF  << 9 * 2; // alternate function mode
	GPIOA->MODER |= GPIO_Mode_AF  << 10 * 2; // alternate function mode
	GPIOA->OTYPER |= GPIO_OType_PP << 9;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOA->OTYPER |= GPIO_OType_PP << 10;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3 << 9 * 2;      /*!< GPIO port output speed register to maximum speed */
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3 << 10 * 2;      /*!< GPIO port output speed register to maximum speed */
	// PUPD_UP ? A VERIFIER
	GPIOA->PUPDR |= GPIO_PuPd_NOPULL << 9 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
	GPIOA->PUPDR |= GPIO_PuPd_NOPULL << 10 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
}

void USART_enable()
{

  /* Clear M, PCE, PS, TE and RE bits */
	USART1->CR1 &= (uint32_t)~((uint32_t)CR1_CLEAR_MASK);
	USART1->CR3 &= ~(1 << 6);
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
	USART1->BRR = 8000000/9600;
	/* USART enable  */
	USART1->CR1 |= USART_CR1_UE; // 0x1
	// USART1->CR2 &= ~(USART_CR2_STOP_1 | USART_CR2_STOP_0);
}

void USART_output(char *s)
{
	while(*s)
	{
		while(!(USART1->ISR & USART_ISR_TXE)); // Wait for Empty
		USART1->TDR = *s++;
	}
}

void echo_back()
{
	while(1) // Don't want to exit
	{
		uint16_t ch;
		while(!(USART1->ISR & USART_ISR_RXNE)); // Wait for Empty
		ch = (USART1->RDR & (uint16_t)0x01FF);
		while(!(USART1->ISR & USART_ISR_TXE)); // Wait for Empty
		USART1->TDR = ch & 0x01FF;
	}
}
