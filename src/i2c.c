/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   i2c.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/04 17:52:07 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/12 18:31:21 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "drone.h"
#define CR2_CLEAR_MASK          ((uint32_t)0x07FF7FFF)  /*<! I2C CR2 clear register Mask */
#define TIMING_CLEAR_MASK       ((uint32_t)0xF0FFFFFF)  /*<! I2C TIMING clear register Mask */

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

void I2C_enable()
{
	RCC_I2C_enable();
	GPIOB_connect_PIN6_PIN7();
	GPIOB_config();

	// I2C1SW enabled  why ?
	RCC->CFGR3 |= RCC_I2C1CLK_SYSCLK;
	/* Clearing to reset value setting PE to 0 ensure a software reset (need 3 apb clock cycle to be effective) */
	I2C1->CR1 = 0x00000000;
	/* Clearing to reset value */
	I2C1->TIMINGR = 0x00000000;

	I2C1->TIMINGR = 0xC062121F & TIMING_CLEAR_MASK; // find the right value
	I2C1->TIMINGR = 0xB0420F13 & TIMING_CLEAR_MASK; // for clock at 48 Mhz
	I2C1->TIMINGR = 0x00902025 & TIMING_CLEAR_MASK; // for clock at 72 Mhz ?

	I2C1->CR1 |= I2C_CR1_PE; // ici pour la fonction init()
	I2C1->OAR1 = 0x00000000;
	I2C1->OAR2 = 0x00000000;
	I2C1->OAR1 |= I2C_OAR1_OA1EN;
	I2C2->CR2 = 0x00000000;
}

void I2C_communicate(void)
{
// (DeviceAddr & 0x7f) << 1
// 
// tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
// /* update tmpreg */
// tmpreg |= (uint32_t)(((uint32_t)Address & I2C_CR2_SADD) | (((uint32_t)Number_Bytes << 16 ) & I2C_CR2_NBYTES) | \
//           (uint32_t)ReloadEndMode | (uint32_t)StartStopMode);

	const uint32_t magnetic_field_addr = 0x03;
	//const linear_acceleration_addr = 0x28;

	//  1. Send the START bit (S).
	I2C1->CR2 |= I2C_CR2_START; // I2C_CR2_START [13]
	//  2. Send the slave address (ADDR). Usually 7 bits.
	I2C1->CR2 |= (magnetic_field_addr << 1); // I2C_CR2_SADD [0;9]
	//  3. Send the Read(R)-1 / Write(W)-0 bit.
	I2C1->CR2 |= I2C_CR2_RD_WRN; // (read)I2C_CR2_RD_WRN [10]
	//  4. Ensure 7bit mode
	I2C1->CR2 &= ~I2C_CR2_ADD10; // ensure disabled I2C_CR2_ADD10 [11]
	//  5. Only 1 byte send
	I2C1->CR2 |= (1 << 16); // 1 byte send with addr I2C_CR2_NBYTES [16;23]
	//  7. Send the STOP bit (P).
	I2C1->CR2 &= ~I2C_CR2_AUTOEND; // I2C_CR2_STOP [14]
	// I2C1->CR2 |= I2C_CR2_STOP; // (enable stop)I2C_CR2_STOP [14]

	while ((I2C1->ISR & I2C_ISR_TXIS) == 0) {};

	I2C1->TXDR = magnetic_field_addr & I2C_TXDR_TXDATA;

	while ((I2C1->ISR & I2C_ISR_TC) == 0)  {};

	I2C1->CR2 &= ~I2C_CR2_ADD10; // ensure disabled I2C_CR2_ADD10 [11] (default 7bits)
	I2C1->CR2 &= ~I2C_CR2_RD_WRN; // I2C_CR2_RD_WRN [10]
	I2C1->CR2 |= 6 << 16; // I2C_CR2_NBYTES [16;23]
	I2C1->CR2 |= I2C_CR2_START; // I2C_CR2_START [13]
	I2C1->CR2 |= I2C_CR2_AUTOEND; // I2C_CR2_STOP [14]

	uint8_t a;
	uint8_t data[6];

	a = 0;
	while (a < 6)
	{
		while ((I2C1->ISR & I2C_ISR_TXIS) == 0) {};
		data[a] = I2C1->RXDR & I2C_RXDR_RXDATA;
		while ((I2C1->ISR & I2C_ISR_RXNE) == 0) {};
		a++;
	}
	_write(0, (char *)&data, 6);
}
