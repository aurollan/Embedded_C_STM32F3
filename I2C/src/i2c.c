#include "i2c.h"
#include "stm32f30x.h"
#include "stm32f30x_it.h"

void  I2C_Init()
{
	I2C1->CR1 &= ~I2C_CR1_PE;

	I2C1->TIMINGR = 0xC062121F & TIMING_CLEAR_MASK;
	// I2C1->TIMINGR = 0xE0000303 & TIMING_CLEAR_MASK;
	I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
	I2C1->OAR1 = (I2C_OAR1_OA1EN | ACCELERO_I2C_ADDRESS);
	I2C1->CR2 |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);
	I2C1->OAR2 &= ~I2C_DUALADDRESS_ENABLE;
	I2C1->OAR2 = (I2C_DUALADDRESS_DISABLE | 0x00 | (0x00 << 8));
	I2C1->CR1 = (I2C_GENERALCALL_DISABLE | I2C_NOSTRETCH_DISABLE);
	I2C1->CR1 |= I2C_CR1_PE;
}

void ENABLE_GPIOB_SCA_SCL(void)
{
	/* Init the I2C */
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->AFR[0] &= ~(0xF << ((6 % 8) * 4)); // le mask est sur 4 bits
	GPIOB->AFR[0] &= ~(0xF << ((7 % 8) * 4)); // le mask est sur 4 bits
	/* Setting bits */
	GPIOB->AFR[0] |= (GPIO_AF_4 << (6 % 8) * 4);
	GPIOB->AFR[0] |= (GPIO_AF_4 << (7 % 8) * 4);
	GPIOB->MODER |= GPIO_Mode_AF << 6 * 2; // alternate function mode
	GPIOB->MODER |= GPIO_Mode_AF << 7 * 2; // alternate function mode
	GPIOB->OTYPER |= GPIO_OType_PP << 6;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOB->OTYPER |= GPIO_OType_PP << 7;       /*!< GPIO port output type register in push pull mode  0x00 */
	GPIOB->OSPEEDR |= GPIO_Speed_Level_3 << 6 * 2;      /*!< GPIO port output speed register to maximum speed */
	GPIOB->OSPEEDR |= GPIO_Speed_Level_3 << 7 * 2;      /*!< GPIO port output speed register to maximum speed */
	// PUPD_UP ? A VERIFIER
	GPIOB->PUPDR |= GPIO_PuPd_DOWN << 6 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
	GPIOB->PUPDR |= GPIO_PuPd_DOWN << 7 * 2;        /*!< GPIO port pull-up/pull-down register,                      */

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;            /*!< I2C clock enable */
	I2C_Init();
}


uint8_t  READ_REGISTER(uint16_t DeviceAddr, uint16_t RegisterAddr)
{
	// TODO: WARNING REGADDR OPTIMIZED OUT WITH OPTI FLAG SET IN GCC
	uint32_t timeout = I2C_TIMEOUT;
	uint8_t data;

	while ((I2C1->ISR & I2C_ISR_BUSY) != 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRBUSY\n", 14);
			return (0);
		}
	}

	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) | ((uint32_t)(((uint32_t)DeviceAddr & I2C_CR2_SADD) | (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_SOFTEND_MODE | (uint32_t)I2C_GENERATE_START_WRITE));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TXIS) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRTXIS\n", 14);
			return (0);
		}
	}

	I2C1->TXDR = ((uint8_t)((uint16_t)((RegisterAddr) & (uint16_t)(0x00FFU))));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TC) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRTC\n", 12);
			return (0);
		}
	}

	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) | ((uint32_t)(((uint32_t)DeviceAddr & I2C_CR2_SADD) | (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)I2C_GENERATE_START_READ));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_RXNE) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRRXNE\n", 14);
			return (0);
		}
	}
	data = (uint8_t)I2C1->RXDR;

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_STOPF) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRSTOPF\n", 15);
			return (0);
		}
	}
	I2C1->ICR &= (uint32_t)~((uint32_t)(1 << 5));
	I2C1->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));
	return data;
}

void  WRITE_REGISTER(uint16_t DeviceAddr, uint16_t RegisterAddr, uint8_t RegisterConfig)
{
	// TODO: WARNING REGADDR OPTIMIZED OUT WITH OPTI FLAG SET IN GCC
	uint32_t timeout = I2C_TIMEOUT;

	while ((I2C1->ISR & I2C_ISR_BUSY) != 0)
	{
		if((timeout--) == 0)
		{
			return ;
		}
	}
	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) | ((uint32_t)(((uint32_t)DeviceAddr & I2C_CR2_SADD) | (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_RELOAD_MODE | (uint32_t)I2C_GENERATE_START_WRITE));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TXIS) == 0)
	{
		if((timeout--) == 0)
		{
			return ;
		}
	}

	I2C1->TXDR = ((uint8_t)((uint16_t)((RegisterAddr) & (uint16_t)(0x00FFU))));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TCR) == 0)
	{
		if((timeout--) == 0)
		{
			return ;
		}
	}

	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) | ((uint32_t)(((uint32_t)DeviceAddr & I2C_CR2_SADD) | (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)I2C_NO_STARTSTOP));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TXIS) == 0)
	{
		if((timeout--) == 0)
		{
			return ;
		}
	}
	I2C1->TXDR = RegisterConfig;

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_STOPF) == 0)
	{
		if((timeout--) == 0)
		{
			return ;
		}
	}
	I2C1->ICR &= (uint32_t)~((uint32_t)(1 << 5));
	I2C1->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));
}


void LSM303DLHCAcc_GetData(uint8_t *pDataXYZ)
{

	// const uint8_t MAGNETOMETER = 0x1E; // 0x1E
	int16_t pnRawData[3];
	uint8_t ctrlx[2]={0,0};
	int8_t buffer[6];
	uint8_t i = 0;
	uint8_t sensitivity = LSM303DLHC_ACC_SENSITIVITY_2G;

	/* Read the acceleration control register content */
	ctrlx[0] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A);
	ctrlx[1] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG5_A);

	/* Read output register X, Y & Z acceleration */
	// pDataXYZ[0] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_L_M);
	// pDataXYZ[1] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M);
	// pDataXYZ[2] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_M);
	// pDataXYZ[3] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_M);
	// pDataXYZ[4] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_M);
	// pDataXYZ[5] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_M);

	pDataXYZ[0] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A);
	pDataXYZ[1] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_H_A);
	pDataXYZ[2] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_A);
	pDataXYZ[3] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_A);
	pDataXYZ[4] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_A);
	pDataXYZ[5] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_A);
	return ;
	/* Check in the control register4 the data alignment*/
	if(!(ctrlx[0] & LSM303DLHC_BLE_MSB))
	{
		for(i=0; i<3; i++)
		{
			pnRawData[i]=((int16_t)((uint16_t)buffer[2*i+1] << 8) + buffer[2*i]);
		}
	}
	else /* Big Endian Mode */
	{
		for(i=0; i<3; i++)
		{
			pnRawData[i]=((int16_t)((uint16_t)buffer[2*i] << 8) + buffer[2*i+1]);
		}
	}

	/* Normal mode */
	/* Switch the sensitivity value set in the CRTL4 */
	switch(ctrlx[0] & LSM303DLHC_FULLSCALE_16G)
	{
		case LSM303DLHC_FULLSCALE_2G:
			sensitivity = LSM303DLHC_ACC_SENSITIVITY_2G;
			break;
		case LSM303DLHC_FULLSCALE_4G:
			sensitivity = LSM303DLHC_ACC_SENSITIVITY_4G;
			break;
		case LSM303DLHC_FULLSCALE_8G:
			sensitivity = LSM303DLHC_ACC_SENSITIVITY_8G;
			break;
		case LSM303DLHC_FULLSCALE_16G:
			sensitivity = LSM303DLHC_ACC_SENSITIVITY_16G;
			break;
	}

	/* Obtain the mg value for the three axis */
	for(i=0; i<3; i++)
	{
		pDataXYZ[i]=(pnRawData[i] * sensitivity);
	}
}

void ENABLE_LSM303DLHC_INT(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;

	/* Those pin are in input mode and speed_3  and NOPULL*/
	GPIOE->OSPEEDR |= GPIO_SPEED_FREQ_HIGH << 5 * 2;      /*!< GPIO port output speed register to maximum speed */
	GPIOE->OSPEEDR |= GPIO_SPEED_FREQ_HIGH << 4 * 2;      /*!< GPIO port output speed register to maximum speed */
	ENABLE_GPIOB_SCA_SCL();
}

uint8_t LSM303DLHCAcc_Init(void)
{
	uint8_t tmpreg;
	uint16_t RegisterConfig = 0x0000;

	// Xen/Yen/Zen
	// 50hz
	RegisterConfig = 0x0847;
	ENABLE_LSM303DLHC_INT();
	WRITE_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG1_A, RegisterConfig);

	// Continuous update
	// data LSB at lower address
	// +-2g
	// hight resolution disbled
	// 4-wire interface
	RegisterConfig = (uint8_t) (RegisterConfig << 8); // same as = 0;
	WRITE_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, RegisterConfig);


	//  HPM1 enable
	//  HPIS1 enable
	RegisterConfig = 0x90;
	tmpreg = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A);
	// Keeping only FDS and HPCLICK if enabled
	tmpreg &= 0x0C;
	// adding HPM1 and HPIS1
	tmpreg |= RegisterConfig;
	// writing it
	WRITE_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A, tmpreg);
	return(1);
}
