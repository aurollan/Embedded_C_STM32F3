/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   i2c.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/04 17:52:07 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/04 19:00:03 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "drone.h"


void RCC_USART1_enable()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;            /*!< I2C clock enable */
}

void GPIOB_connect_PIN6_PIN7()
{
	/* Cleaning bits */
	GPIOB->AFR[0] &= ~(0xF << ((6 % 8) * 4)); // le mask est sur 4 bits
	GPIOB->AFR[0] &= ~(0xF << ((7 % 8) * 4)); // le mask est sur 4 bits
	/* Setting bits */
	GPIOB->AFR[0] |= (GPIO_AF_4 << (6 % 8) * 4);
	GPIOB->AFR[0] |= (GPIO_AF_4 << (7 % 8) * 4);
}

/* Open drain connection ? */
void GPIOB_config()
{
	GPIOB->MODER |= GPIO_Mode_AF << 6 * 2; // alternate function mode
	GPIOB->MODER |= GPIO_Mode_AF << 7 * 2; // alternate function mode
	GPIOB->OTYPER |= GPIO_OType_PP << 6;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOB->OTYPER |= GPIO_OType_PP << 7;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOB->OSPEEDR |= GPIO_Speed_Level_3 << 6 * 2;      /*!< GPIO port output speed register to maximum speed */
	GPIOB->OSPEEDR |= GPIO_Speed_Level_3 << 7 * 2;      /*!< GPIO port output speed register to maximum speed */
	// PUPD_UP ? A VERIFIER
	GPIOB->PUPDR |= GPIO_PuPd_NOPULL << 6 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
	GPIOB->PUPDR |= GPIO_PuPd_NOPULL << 7 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
}

/* TODO: A FINIR  */
void I2C_enable()
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
	USART1->BRR = 72000000/9600;
	/* USART enable  */ 
	USART1->CR1 |= USART_CR1_UE; // 0x1 
	// USART1->CR2 &= ~(USART_CR2_STOP_1 | USART_CR2_STOP_0);
}

/* 
 *	RCC->I2CCLK
 *	I2C enble by PE bit in I2C_CR1
 *	CTRL_REG1_A (ODR1/2/3/0 Lpen Z/X/Yen)
 *
 *	I2C initializtion flowchart
 *	Initial settings
 *	Clear PE bit in I2C_CR1
 *	COnfigure ANFOFF and DNF[3:0] in I2C_CR1
 *	configure  PRESC/SDADEL/SCLDEL/SCLH/SCLL in I2C_TIMINGR
 *	COnfigure NOSTRETCH in I2C_CR1
 *	Set PE bin in I2C_CR1
 *	end
 *
 *	1. Send the START bit (S).
 *  2. Send the slave address (ADDR). Usually 7 bits.
 *  3. Send the Read(R)-1 / Write(W)-0 bit.
 *  4. Wait for/Send an acknowledge bit (A).
 *  5. Send/Receive the data byte (8 bits) (DATA).
 *  6. Expect/Send acknowledge bit (A).
 *  7. Send the STOP bit (P).
 *  The sequence 5 and 6 can be repeated so that a multibyte block can be read or written.
 *	
 *	I2C_CR1 : RXIE/TXIE/PE
 */
