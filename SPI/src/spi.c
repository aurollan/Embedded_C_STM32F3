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
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	/* Those pin are in input mode and speed_3  and NOPULL*/
	GPIOE->OSPEEDR |= (3 << (0 * 2));      /*!< GPIO port output speed register to maximum speed */
	GPIOE->OSPEEDR |= (3 << (1 * 2));      /*!< GPIO port output speed register to maximum speed */
	GPIOE->OSPEEDR |= (3 << (3 * 2));      /*!< GPIO port output speed register to maximum speed */
	/* OUPUT */
	GPIOE->MODER |= (1 << 0);
	GPIOE->MODER |= (1 << 2);

	GPIOE->MODER |= (1 << 6);
	GPIOE->MODER |= 0x00000040; //PE3 in general purpose

	/* NO PULL */
	GPIOE->PUPDR = 0x00;
}

void	enable_GPIOA_af(void)
{
	/* Enable GPIOA */
	RCC->AHBENR |= (1 << 17);
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	/* AF mode for GPIOA 5/6/7 */
	GPIOA->MODER |= (2 << 10) | (2 << 12) | (2 << 14);
 //  GPIOA->MODER |= 0x0000A800; //PA5 PA6 PA7 in alternate function.
	/* hight speed mode for GPIOA 5/6/7 */
	// GPIOA->OSPEEDR |= (3 << 10) | (3 << 12) | (3 << 14);
	GPIOA->OSPEEDR |= 0x00005400; //10MHz for port A
	/* Pull-up mode for GPIOA 5/6/7 */
	// GPIO_InitStructure.Pull  = GPIO_NOPULL; /* or GPIO_PULLDOWN */
	// GPIOA->PUPDR |= (1 << 10) | (1 << 12) | (1 << 14);
	/* Reseting pin 5/6/7 AF */
	GPIOA->AFR[0] &= 0x000FFFFF;
	/* AF5 SPI Alternate function */
	GPIOA->AFR[0] |= (5 << 20) | (5 << 24) | (5 << 28);
 //  GPIOA->AFR[0] |= 0x55500000;
	GPIOE->BSRR |= (1 << 3); //do not communicate while config
}

void	enable_SPI1(void)
{
	RCC->APB2ENR |= (1 << 12);
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}


void	L3GD20Gyro_Init(void)
{
	enable_GPIOE_int();
	enable_SPI1();
	enable_GPIOA_af();
	GPIOE->BSRR |= (1<<3); //Brining PE3 to 1 to select the CS of the SPI. We want to bring it to 1 so that we can go though the configuration phase.


 //  SPI1->I2SCFGR &= 0xF7FF;

	// 30.5.7 Configuration of SPI
    // SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    // SpiHandle.Init.CRCPolynomial = 7;
	// 1. Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins.
	/* 2. Write to the SPI_CR1 register */
	// a) Configure the serial clock baud rate using the BR[2:0] bits (Note: 4).
	//SPI1->CR1 |= ((uint16_t)(0x0 << 3));
	SPI1->CR1 |= ((uint16_t)(0x2 << 3));
 //  SPI1->CR1 |= SPI_CR1_BR_1; //Bode rate equal to 72MHz / 8 . We do this because the gyroscope can't go faster than 10MHz.
	// b) Configure the CPOL and CPHA bits combination to define one of the four
	// relationships between the data transfer and the serial clock
	/* CPOL 0 when idle */
	/* CPHA 0 when idle */
	SPI1->CR1 &= ~((uint16_t)(1 << 1));
	SPI1->CR1 &= ~((uint16_t)(1 << 0));

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
 //  SPI1->CR1 |= SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_MSTR; //Selecting software driven and master mode.
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
 //  SPI1->CR2 |= SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; //Configuration to 8-bit  0111.


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
 //  SPI1->CR2 |= SPI_CR2_FRXTH; //RXNE event is generated ifthe FIFO level is greater than or equal to 1/4 (8-bit)
	
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
}

void	L3GD20Gyro_write_register(uint8_t data, uint8_t config)
{
	/* according to hal library we need to send addr then config */
	// 30.5.9 Data transmission and reception procedures
	// bit 0: WRITE bit. The value is 0.
	// bit 1: MS bit. When 0, do not increment address; when 1, increment address in multiple writing
	// bit 2 -7: address AD(5:0). This is the address field of the indexed register.
	// bit 8-15: data DI(7:0) (write mode). This is the data that will be written to the device (MSb first).
	uint16_t received_data;
	uint32_t spixbase = 0x00;

	spixbase = (uint32_t)&(SPI1->DR);
	received_data = 0x0000;

	GPIOE->BSRR |= 1<<19;
	/* waiting transmitter buffer is empty */
	while (!(SPI1->SR & (uint16_t)(1 << 1))) {};
	// while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}
	/* Sending package value to get the data */
	*(__IO uint8_t *)spixbase = data;
	// SPI1->DR = received_data;
	/* waiting receiver buffer is full */
	while (!(SPI1->SR & (uint16_t)(1 << 0))) {};
	// while((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE){}
	/* Reading value from Data Register */
	received_data = *(__IO uint8_t *)spixbase;
	while (!(SPI1->SR & (uint16_t)(1 << 1))) {};
	// while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}
	/* Sending package value to get the data */
	*(__IO uint8_t *)spixbase = config;
	// SPI1->DR = received_data;
	/* waiting receiver buffer is full */
	while (!(SPI1->SR & (uint16_t)(1 << 0))) {};
	// while((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE){}
	/* Reading value from Data Register */
	received_data = *(__IO uint8_t *)spixbase;
	(void)received_data;
	GPIOE->BSRR |= 1<<3;
}


uint8_t	L3GD20Gyro_read_register(uint8_t data)
{
	uint16_t received_data;

	uint32_t spixbase = 0x00;

	spixbase = (uint32_t)&(SPI1->DR);
	/* according to hal library we need to send readaddr then dummy byte */
	// 30.5.9 Data transmission and reception procedures
	// bit 0: READ bit. The value is 1.
	// bit 1: MS bit. When 0 do not increment address; when 1 increment address in multiple reading.
	// bit 2-7: address AD(5:0). This is the address field of the indexed register.
	// bit 8-15: received data
	received_data = 0x0000;
	data |= 0x80; 

	GPIOE->BSRR |= 1<<19;
	/* waiting transmitter buffer is empty */
	while (!(SPI1->SR & (uint16_t)(1 << 1))) {};
	// while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}
	/* Sending package value to get the data */
	*(__IO uint8_t *)spixbase = data;
	// SPI1->DR = received_data;
	/* waiting receiver buffer is full */
	while (!(SPI1->SR & (uint16_t)(1 << 0))) {};
	// while((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE){}
	/* Reading value from Data Register */
	received_data = SPI1->DR;
	while (!(SPI1->SR & (uint16_t)(1 << 1))) {};
	// while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}
	/* Sending package value to get the data */
	*(__IO uint8_t *)spixbase = 0x00;
	// SPI1->DR = received_data;
	/* waiting receiver buffer is full */
	while (!(SPI1->SR & (uint16_t)(1 << 0))) {};
	// while((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE){}
	/* Reading value from Data Register */
	received_data = SPI1->DR;
	GPIOE->BSRR |= 1<<3;
	return ((uint8_t)received_data);
}

void	setup_gyro()
{
	uint8_t data[6];

	/* according to hal library we need to init REG1 and REG4 */

	data[0] = L3GD20Gyro_read_register(0x0F);
	delay(1000);
	data[1] = L3GD20Gyro_read_register(0x20);
	delay(1000);
	data[2] = L3GD20Gyro_read_register(0x0F);
	delay(1000);
	data[3] = L3GD20Gyro_read_register(0x20);
	delay(1000);
	data[4] = L3GD20Gyro_read_register(0x0F);
	delay(1000);
	data[5] = L3GD20Gyro_read_register(0x20);
	delay(1000);
	L3GD20Gyro_write_register(0x20, 0x0F);
	delay(1000);
	data[6] = L3GD20Gyro_read_register(0x20);
	delay(1000);
	print_data(&data[0], 6);
	while (1) {};

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
	while (1) {};
}

void	L3GD20Gyro_GetData(uint8_t *data)
{
// GYRO_IO_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
//  GYRO_IO_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
	data[0] = L3GD20Gyro_read_register(0x28);
	data[1] = L3GD20Gyro_read_register(0x29);
	data[2] = L3GD20Gyro_read_register(0x2a);
	data[3] = L3GD20Gyro_read_register(0x2b);
	data[4] = L3GD20Gyro_read_register(0x2c);
	data[5] = L3GD20Gyro_read_register(0x2d);
}
