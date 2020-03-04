/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   i2c.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/04 17:52:07 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/26 17:53:23 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "drone.h"
#define CR2_CLEAR_MASK          ((uint32_t)0x07FF7FFF)  /*<! I2C CR2 clear register Mask */
#define TIMING_CLEAR_MASK       ((uint32_t)0xF0FFFFFF)  /*<! I2C TIMING clear register Mask */
#define I2C_TIMEOUT         ((uint32_t)0x3FFFF) /*!< I2C Time out */


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
	// To test
	// GPIOB->PUPDR |= GPIO_PuPd_DOWN << 6 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
	// GPIOB->PUPDR |= GPIO_PuPd_DOWN << 7 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
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
    // I2C1->TIMINGR = 0xF0001317 | ((4 & 0x0F) << 20) | ((5 & 0x0F) << 16);
	// I2C1->TIMINGR = 0xB0420F13 & TIMING_CLEAR_MASK; // for clock at 48 Mhz
	// I2C1->TIMINGR = 0x00902025 & TIMING_CLEAR_MASK; // for clock at 72 Mhz ?

	I2C1->CR1 |= I2C_CR1_PE; // ici pour la fonction init()
	I2C1->OAR1 = 0x00000000;
	I2C1->OAR2 = 0x00000000;
	I2C1->OAR1 |= I2C_OAR1_OA1EN;
	I2C1->CR2 = 0x00000000;
	// hi2c->Instance->TIMINGR = hi2c->Init.Timing & TIMING_CLEAR_MASK;
	// hi2c->Instance->OAR1 &= ~I2C_OAR1_OA1EN;
	// hi2c->Instance->OAR1 = (I2C_OAR1_OA1EN | hi2c->Init.OwnAddress1);
	// hi2c->Instance->CR2 |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);
	// hi2c->Instance->OAR2 &= ~I2C_DUALADDRESS_ENABLE;
	// hi2c->Instance->OAR2 = (hi2c->Init.DualAddressMode | hi2c->Init.OwnAddress2 | (hi2c->Init.OwnAddress2Masks << 8));
	// hi2c->Instance->CR1 = (hi2c->Init.GeneralCallMode | hi2c->Init.NoStretchMode);
}


int I2C_communicate(void)
{
	uint32_t timeout = I2C_TIMEOUT;
	const uint32_t magnetic_field_addr = 0b0011110;
	const uint8_t OUT_X_H_M = 0x03;
	//const linear_acceleration_addr = 0x28;
	
	// Wait if communication
	while ((I2C1->ISR & I2C_ISR_BUSY) != 0)
	{
		if((timeout--) == 0) 
		{
			_write(0, "ERROR READBUSY\n", 15);
			return 0;
		}
	}
	I2C1->CR2 |= (magnetic_field_addr << 1); // I2C_CR2_SADD [0;9]
	I2C1->CR2 &= ~I2C_CR2_RD_WRN; // (write)I2C_CR2_RD_WRN [10]
	I2C1->CR2 &= ~I2C_CR2_ADD10; // ensure disabled I2C_CR2_ADD10 [11]
	I2C1->CR2 |= (1 << 16); // 1 byte send with addr I2C_CR2_NBYTES [16;23]
	I2C1->CR2 |= I2C_CR2_START; // I2C_CR2_START [13]



	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TXIS) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR READTXIS\n", 15);
			return 0;
		}
	}

	I2C1->TXDR = (uint8_t)OUT_X_H_M & I2C_TXDR_TXDATA;

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TC) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR READ_TC\n", 14);
			return 0;
		}
	}

	I2C1->CR2 &= ~I2C_CR2_ADD10; // ensure disabled I2C_CR2_ADD10 [11] (default 7bits)
	I2C1->CR2 |= I2C_CR2_RD_WRN; // I2C_CR2_RD_WRN [10]
	I2C1->CR2 &= ~(0xFF << 16); // I2C_CR2_NBYTES [16;23]
	I2C1->CR2 |= 6 << 16; // I2C_CR2_NBYTES [16;23]
	I2C1->CR2 |= I2C_CR2_AUTOEND; // I2C_CR2_STOP [14]
	I2C1->CR2 |= I2C_CR2_START; // I2C_CR2_START [13]

	volatile uint8_t a;
	volatile uint8_t data[6];
	volatile char		c;

	a = 0;
	_write(0, "DATA\n", 5);
	_write(0, "0x", 2);
	while (a < 6)
	{
		while ((I2C1->ISR & I2C_ISR_RXNE) == 0) 
		{
			if((timeout--) == 0)
			{
				_write(0, "ERROR READRXNE\n", 15);
				return 0;
			}
		}
		(void)data;
		c = I2C1->RXDR;
		ft_print_hexa((uint8_t)c);
		_write(0, "\n", 1);
		a++;
	}
	_write(0, "\n", 1);
	return (1);
}


void i2c1_initp(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;

  RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_4);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  I2C_DeInit(I2C1);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = 0x2000090E;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;


  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);

  // init_register();
}

uint8_t i2c_set(uint8_t DeviceAddr, uint8_t RegAddr)
{
	uint32_t timeout;

	// Auto increment register when multiple read
	// RegAddr |= 0b10000000;
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISR BUSY\n", 15);
			return 0;
		}
	}
	// TRY I2C_CR2_RELOAD instead of  I2C_SoftEnd_mode 
	I2C_TransferHandling(I2C1, DeviceAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISR TXIS\n", 15);
			return 0;
		}
	}

	I2C_SendData(I2C1, RegAddr);

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TC) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISR TC\n", 13);
			return 0;
		}
	}
	return (1);
}

uint8_t i2c_read(uint8_t DeviceAddr, uint8_t* data)
{
	uint32_t timeout;

	I2C_TransferHandling(I2C1, DeviceAddr, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	// I2C_TransferHandling(I2C1, DeviceAddr, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_RXNE) == 0) 
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISR RXNE\n", 15);
			return 0;
		}
	}
	*data = I2C_ReceiveData(I2C1);
	I2C1->CR2 |= I2C_CR2_NACK;
	while ((I2C1->CR2 & I2C_CR2_NACK) == 0) {};
	I2C1->CR2 |= I2C_CR2_STOP;
	while ((I2C1->CR2 & I2C_CR2_STOP) == 0) {};
	return (1);
}




// void	init_register(void)
// {
// 
// }
