/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   i2c.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/04 17:52:07 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/05 17:56:49 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "drone.h"

// pb6 => SCL
// pb7 => SDA

void RCC_I2C_enable()
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
	GPIOB->OTYPER |= GPIO_OType_OD << 6;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOB->OTYPER |= GPIO_OType_OD << 7;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOB->OSPEEDR |= GPIO_Speed_Level_3 << 6 * 2;      /*!< GPIO port output speed register to maximum speed */
	GPIOB->OSPEEDR |= GPIO_Speed_Level_3 << 7 * 2;      /*!< GPIO port output speed register to maximum speed */
	// PUPD_UP ? A VERIFIER
	GPIOB->PUPDR |= GPIO_PuPd_NOPULL << 6 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
	GPIOB->PUPDR |= GPIO_PuPd_NOPULL << 7 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
}

/* TODO: A FINIR  */
void I2C_enable()
{
// a trouver I2C_Mode_I2C
// enable SCL/SDA
/* 
 *	RCC->I2CCLK
 *	CTRL_REG1_A (ODR1/2/3/0 Lpen Z/X/Yen)
 *
 *	COnfigure ANFOFF and DNF[3:0] in I2C_CR1
 *	configure  PRESC/SDADEL/SCLDEL/SCLH/SCLL in I2C_TIMINGR
 *	end
 *
 *	
 *	
 *	I2C->ISR->txe
 */
	/* CLEARING */
	I2C1->CR1 &= CR1_CLEAR_MASK;
	I2C1->TIMINGR &= TIMING_CLEAR_MASK;
	// Start with disabling CR1 to conf
	I2C1->CR1 &= ~I2C_CR1_PE;
	// Enabled only in slave mode, make sure disabled
	I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;
	// No smb
	I2C1->CR1 &= ~(uint32_t)(3 << 20);

	I2C1->TIMINGR |= I2C_TIMINGR_SCLL;
	I2C1->TIMINGR |= I2C_TIMINGR_SCLH;
	I2C1->CR1 |= I2C_CR1_PE;

//  1. Send the START bit (S).
	I2C1->CR2 |= I2C_CR2_START; // I2C_CR2_START [13]
//  2. Send the slave address (ADDR). Usually 7 bits.
	I2C1->CR2->SADD |= addr; // I2C_CR2_SADD [0;9]
//  3. Send the Read(R)-1 / Write(W)-0 bit.
	I2C1->CR2->RD_WRN |= I2C_CR2_RD_WRN; // (read)I2C_CR2_RD_WRN [10]
//  4. Ensure 7bit mode
	I2C1->CR2->ADD10 &= ~I2C_CR2_ADD10; // ensure disabled I2C_CR2_ADD10 [11]
//  5. Only 1 byte send
	I2C1->CR2->Nbytes |= (1 << 16); // 1 byte send with addr I2C_CR2_NBYTES [16;23]
//  7. Send the STOP bit (P).
	I2C1->CR2 |= I2C_CR2_STOP; // (enable stop)I2C_CR2_STOP [14]

	while (I2C1->ISR->txis == 0) {};
	
	I2C1->TXDR = periph_addr & I2C_TXDR_TXDATA;

	while {I2C1->ISR->TC == 0}  {};

	I2C1->CR2->ADD10 |= 7_bits; // I2C_CR2_ADD10 [11]
	I2C1->CR2->RD_WRN |= write; // I2C_CR2_RD_WRN [10]
	I2C1->CR2->Nbytes |= number_of_bytes; // I2C_CR2_NBYTES [16;23]
	I2C1->CR2 |= start; // I2C_CR2_START [13]
	I2C1->CR2 |= stop; // I2C_CR2_STOP [14]
	// I2C_CR2_AUTOEND [25]


	while (I2C1->ISR->txis == 0) {};
	data = I2C1->RXDR & I2C_RXDR_RXDATA;
	while (I2C1->ISR->rxne == 0) {};
}

