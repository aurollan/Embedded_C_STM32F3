/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/13 14:38:48 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "spi.h"


/* 1. Write proper GPIO register */
void	enable_GPIOE_int(void)
{
	RCC->AHBENR |= (1 << 21);
	/* Those pin are in input mode and speed_3  and NOPULL*/
	GPIOE->OSPEEDR |= (3 << (0 * 2));      /*!< GPIO port output speed register to maximum speed */
	GPIOE->OSPEEDR |= (3 << (1 * 2));      /*!< GPIO port output speed register to maximum speed */
	GPIOE->OSPEEDR |= (3 << (3 * 2));      /*!< GPIO port output speed register to maximum speed */
	GPIOE->MODER |= (1 << 0);
	GPIOE->MODER |= (1 << 2);
	GPIOE->MODER |= (1 << 6);
}

void	enable_GPIOA_af(void)
{
	/* Enable GPIOA */
	RCC->AHBENR |= (1 << 17);
	/* AF mode for GPIOA 5/6/7 */
	GPIOA->MODER |= (2 << 10) | (2 << 12) | (2 << 14);
	/* hight speed mode for GPIOA 5/6/7 */
	// GPIOA->OSPEEDR |= (3 << 10) | (3 << 12) | (3 << 14);
	GPIOA->OSPEEDR |= 0x00005400; //10MHz for port A
	/* Pull-up mode for GPIOA 5/6/7 */
	// GPIOA->PUPDR |= (1 << 10) | (1 << 12) | (1 << 14);
	/* Reseting pin 5/6/7 AF */
	GPIOA->AFR[0] &= 0x000FFFFF;
	/* AF5 SPI Alternate function */
	GPIOA->AFR[0] |= (5 << 20) | (5 << 24) | (5 << 28);
	GPIOE->BSRR |= (1 << 3); //do not communicate while config
}

void	enable_SPI1(void)
{
	RCC->APB2ENR |= (1 << 12);
}


void	L3GD20Gyro_Init(void)
{
	enable_GPIOE_int();
	enable_SPI1();
	enable_GPIOA_af();
	// 30.5.7 Configuration of SPI
	// 1. Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins.
	/* 2. Write to the SPI_CR1 register */
	// a) Configure the serial clock baud rate using the BR[2:0] bits (Note: 4).
	//SPI1->CR1 |= ((uint16_t)(0x0 << 3));
	SPI1->CR1 |= ((uint16_t)(0x2 << 3));
	// b) Configure the CPOL and CPHA bits combination to define one of the four
	// relationships between the data transfer and the serial clock
	/* CPOL 0 when idle */
	// SPI1->CR1 &= ~((uint16_t)(1 << 1));
	/* CPHA 1 when idle */
	// SPI1->CR1 |= (uint16_t)(1 << 1);
	// SPI1->CR1 |= (uint16_t)(1 << 0);

	// c) Select simplex or half-duplex mode by configuring RXONLY or BIDIMODE and
	// We don't need to bother with or GPIOE availability wire for now so we
	// will keep full duplex
	/* BIDIMODE output disable */
	SPI1->CR1 &= ~((uint16_t)(1 << 15));
	/* BIDIOE output disable in BIDIMODE 1 transmit; 0 receive */
	SPI1->CR1 &= ~((uint16_t)(1 << 14));

	// d) Configure the LSBFIRST bit to define the frame format (Note: 2).
	// We are in little endian so we want to read our data that way
//	SPI1->CR1 |= (uint16_t)(1 << 7);
	
	// e) Configure the CRCL and CRCEN bits if CRC is needed (while SCK clock signal is
	// at idle state).
	// No need for now
	// f) Configure SSM and SSI (Notes: 2 & 3).
	// No nedd for now

	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	// g) Configure the MSTR bit (in multimaster NSS configuration, avoid
	// conflict state on NSS if master is configured to prevent MODF error).
	/* Master mode selected but useless because there is only one master */
	SPI1->CR1 |= (uint16_t)(1 << 2);

	/* 3. write to SPI_CR2 register: */
	// a) Configure the DS[3:0] bits to select the data length for the transfer.
	/* Clearing data size value */
	SPI1->CR2 &= ~((uint16_t)(0xF << 8));
	/* Seting data size to 8 bits */
	SPI1->CR2 |= (uint16_t)(0x7 << 8);


	// b) Configure SSOE (Notes: 1 & 2 & 3).
	// We set CPHA to 1

	// c) Set the FRF bit if the TI protocol is required (keep NSSP bit cleared in TI mode).
	// Make sur we stay in Motorola mode
	SPI1->CR2 &= ~((uint16_t)(1 << 4));

	// d) Set the NSSP bit if the NSS pulse mode between two data units is required (keep
	// CHPA and TI bits cleared in NSSP mode).
	// Not needed for now

	// e) Configure the FRXTH bit. The RXFIFO threshold must be aligned to the read
	// access size for the SPIx_DR register.
	// We ask for 8 bits read (1 byte)
	SPI1->CR2 |= (uint16_t)(1 << 12);
	
	// f) Initialize LDMA_TX and LDMA_RX bits if DMA is used in packed mode.
	// Not needed

	// 4. Write to SPI_CRCPR register: Configure the CRC polynomial if needed.
	// Not needed

	// 5. Write proper DMA registers: Configure DMA streams dedicated for SPI Tx and Rx in
	// DMA registers if the DMA streams are used.
	// Not needed

	// 30.5.8
	// Procedure for enabling SPI
	// The master at full-duplex (or in any transmit-only mode) starts to communicate when the
	// SPI is enabled and TXFIFO is not empty, or with the next write to TXFIFO.
	/* Enable SPI AFTER configuration */
	SPI1->CR1 |= (uint16_t)(1 << 6);
	GPIOE->BSRR |= 1 << 19;
}

void	L3GD20Gyro_write_register(uint8_t register_address, uint8_t config)
{
	uint16_t data;

	// 30.5.9 Data transmission and reception procedures

	/* waiting transmitter buffer is empty */
	while (!(SPI1->SR & (uint16_t)(1 << 1))) {};
	SPI1->DR = register_address;
	/* Sending config value to get the data */
	while (!(SPI1->SR & (uint16_t)(1 << 0))) {};
	/* Reading value from Data Register */
	data = SPI1->DR;
	while (!(SPI1->SR & (uint16_t)(1 << 1))) {};
	SPI1->DR = config;
	/* waiting Received buffer is empty */
	while (!(SPI1->SR & (uint16_t)(1 << 0))) {};
	/* Reading value from Data Register */
	data = SPI1->DR;
	(void)data;
}

uint8_t	L3GD20Gyro_read_register(uint8_t register_address)
{
	uint16_t data;
	uint8_t datasend;

	// 30.5.9 Data transmission and reception procedures
	register_address |= 0x80; 

	GPIOE->BSRR |= 1 << 19;
	/* waiting transmitter buffer is empty */
	while (!(SPI1->SR & (uint16_t)(1 << 1))) {};
	SPI1->DR = register_address;
	/* Sending dummy value to get the data */
	while (!(SPI1->SR & (uint16_t)(1 << 0))) {};
	/* Reading value from Data Register */
	data = SPI1->DR;
	while (!(SPI1->SR & (uint16_t)(1 << 1))) {};
	SPI1->DR = 0x00;
	/* waiting Received buffer is empty */
	while (!(SPI1->SR & (uint16_t)(1 << 0))) {};
	/* Reading value from Data Register */
	data = SPI1->DR;
	datasend = (uint8_t)data;
	GPIOE->BSRR |= (1 << 3); //stop communication
	return (datasend);
}

void	setup_gyro()
{
	uint8_t data[6];

	data[0] = L3GD20Gyro_read_register(0x0F);
	data[1] = L3GD20Gyro_read_register(0x20);
	L3GD20Gyro_write_register(0x20, 0x0F);
	delay(1000);
	data[2] = L3GD20Gyro_read_register(0x0F);

	// 1.Write CTRL_REG2
	data[3] = L3GD20Gyro_read_register(0x21);
	// 2.Write     CTRL_REG3
	data[4] = L3GD20Gyro_read_register(0x22);
	// 3.Write     CTRL_REG4
	data[5] = L3GD20Gyro_read_register(0x23);
	// 4.Write     CTRL_REG6
	// 5.Write     REFERENCE
	// data[5] = L3GD20Gyro_read_register(0x25);
	// 6.Write     INT1_THS
	// 7.Write     INT1_DUR
	// 8.Write     INT1_CFG
	// 9.Write     CTRL_REG5
	// data[5] = L3GD20Gyro_read_register(0x24);
	// 10.Write   CTRL_REG1
	print_data(&data[0], 6);
}

void	L3GD20Gyro_GetData(uint8_t *data)
{
	data[0] = L3GD20Gyro_read_register(0x28);
	data[1] = L3GD20Gyro_read_register(0x29);
	data[2] = L3GD20Gyro_read_register(0x2a);
	data[3] = L3GD20Gyro_read_register(0x2b);
	data[4] = L3GD20Gyro_read_register(0x2c);
	data[5] = L3GD20Gyro_read_register(0x2d);
}
