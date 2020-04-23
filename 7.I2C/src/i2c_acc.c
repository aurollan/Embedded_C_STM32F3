#include "i2c.h"

void  I2C_Init()
{
	I2C1->CR1 &= ~I2C_CR1_PE;

	// For 8 Mhz I2C clock
	// Standart mode 100 Khz and 8Mhz I2C clock
	// PRESC = (0x1 << 28)
	I2C1->TIMINGR |= (0x1 << 28);
	// SCLDEL =  (0x4 << 20) 
	I2C1->TIMINGR |= (0x4 << 20);
	// SDADEL = (0x2 << 16)
	I2C1->TIMINGR |= (0x2 << 16);
	// SCLH = (0xF << 8)
	I2C1->TIMINGR |= (0xF << 8);
	// SCLL = (0x13 << 0)
	I2C1->TIMINGR |= (0x13 << 0);

	I2C1->CR1 |= (I2C_NOSTRETCH_DISABLE);
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
	GPIOB->PUPDR |= GPIO_PuPd_UP << 6 * 2;        /*!< GPIO port pull-up/pull-down register,                      */
	GPIOB->PUPDR |= GPIO_PuPd_UP << 7 * 2;        /*!< GPIO port pull-up/pull-down register,                      */

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

	/* Wait for stop condition from autoend */
	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_STOPF) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRSTOPF\n", 15);
			return (0);
		}
	}
	return data;
}

void  WRITE_REGISTER(uint16_t DeviceAddr, uint16_t RegisterAddr, uint8_t RegisterConfig)
{
	// TODO: WARNING REGADDR OPTIMIZED OUT WITH OPTI FLAG SET IN GCC
	uint32_t timeout = I2C_TIMEOUT;

	/* Check if another master is using the line */
	while ((I2C1->ISR & I2C_ISR_BUSY) != 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRBUSY\n", 14);
			return ;
		}
	}
	/* Intitiate write transmission */
	// Send Device address
	// NBYTE = 1
	// 
	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) | ((uint32_t)(((uint32_t)DeviceAddr & I2C_CR2_SADD) | (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_RELOAD_MODE | (uint32_t)I2C_GENERATE_START_WRITE));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TXIS) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRTXIS\n", 14);
			return ;
		}
	}

	I2C1->TXDR = ((uint8_t)((uint16_t)((RegisterAddr) & (uint16_t)(0x00FFU))));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TCR) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRTC\n", 12);
			return ;
		}
	}

	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) | ((uint32_t)(((uint32_t)DeviceAddr & I2C_CR2_SADD) | (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)I2C_NO_STARTSTOP));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TXIS) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRTXIS\n", 14);
			return ;
		}
	}
	I2C1->TXDR = RegisterConfig;

	/* Wait for stop condition from autoend */
	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_STOPF) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRSTOPF\n", 15);
			return ;
		}
	}
}


void LSM303DLHC_GetData_Acc(uint8_t *pDataXYZ)
{
	pDataXYZ[0] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A);
	pDataXYZ[1] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_H_A);
	pDataXYZ[2] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_A);
	pDataXYZ[3] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_A);
	pDataXYZ[4] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_A);
	pDataXYZ[5] = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_A);
}

void LSM303DLHC_GetData_Mag(uint8_t *pDataXYZ)
{
	pDataXYZ[0] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_L_M);
	pDataXYZ[1] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M);
	pDataXYZ[2] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_M);
	pDataXYZ[3] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_M);
	pDataXYZ[4] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_M);
	pDataXYZ[5] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_M);
}

uint8_t LSM303DLHCMag_Init(void)
{
	ENABLE_GPIOB_SCA_SCL();

	// Enable  x/y/x axes and 400hz
	WRITE_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG1_A, 0x77);
	// Max output rate
	// Temperature sensor enable
	WRITE_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_CRA_REG_M, 0x9C);
	// gain settings ?
	WRITE_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_CRB_REG_M, 0x20);

	// Continuous stream
	WRITE_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_MR_REG_M, 0x00);
	return(1);
}

uint8_t LSM303DLHCAcc_Init(void)
{
	uint8_t tmpreg;
	uint16_t RegisterConfig = 0x0000;

	// Xen/Yen/Zen
	// 50hz
	RegisterConfig = 0x0847;
	ENABLE_GPIOB_SCA_SCL();
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
