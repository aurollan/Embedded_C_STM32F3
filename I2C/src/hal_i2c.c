#define GPIO_AF_4            ((uint8_t)0x04) /* I2C1, I2C2, TIM1, TIM8, TIM16, TIM17 */
typedef enum
{
  GPIO_PuPd_NOPULL = 0x00,
  GPIO_PuPd_UP     = 0x01,
  GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;
typedef enum
{ 
  GPIO_Speed_Level_1  = 0x01, /*!< Fast Speed     */
  GPIO_Speed_Level_2  = 0x02, /*!< Meduim Speed   */
  GPIO_Speed_Level_3  = 0x03  /*!< High Speed     */
}GPIOSpeed_TypeDef;
typedef enum
{ 
  GPIO_OType_PP = 0x00,
  GPIO_OType_OD = 0x01
}GPIOOType_TypeDef;
typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN   = 0x03  /*!< GPIO Analog In/Out Mode      */
}GPIOMode_TypeDef;
#define LSM303DLHC_OUT_X_L_A                 0x28  /* Output Register X acceleration */
#define LSM303DLHC_OUT_X_H_A                 0x29  /* Output Register X acceleration */
#define LSM303DLHC_OUT_Y_L_A                 0x2A  /* Output Register Y acceleration */
#define LSM303DLHC_OUT_Y_H_A                 0x2B  /* Output Register Y acceleration */
#define LSM303DLHC_OUT_Z_L_A                 0x2C  /* Output Register Z acceleration */
#define LSM303DLHC_OUT_Z_H_A                 0x2D  /* Output Register Z acceleration */ 
#define  GPIO_SPEED_FREQ_HIGH     (0x00000003U)  /*!< range 10 MHz to 50 MHz, please refer to the product datasheet */
#define TIMING_CLEAR_MASK       ((uint32_t)0xF0FFFFFF)  /*<! I2C TIMING clear register Mask */
#define I2C_NOSTRETCH_DISABLE           (0x00000000U)
#define I2C_GENERALCALL_DISABLE         (0x00000000U)
#define ACCELERO_I2C_ADDRESS             0x32
#define I2C_DUALADDRESS_DISABLE         (0x00000000U)
#define I2C_DUALADDRESS_ENABLE          I2C_OAR2_OA2EN
#define ACCELERO_I2C_ADDRESS             0x32
#define I2C_TIMEOUT         ((uint32_t)0x3FFFF) /*!< I2C Time out */
#define  I2C_NO_STARTSTOP               (0x00000000U)
#define  I2C_GENERATE_STOP              (uint32_t)(0x80000000U | I2C_CR2_STOP)
#define  I2C_GENERATE_START_READ        (uint32_t)(0x80000000U | I2C_CR2_START | I2C_CR2_RD_WRN)
#define  I2C_GENERATE_START_WRITE       (uint32_t)(0x80000000U | I2C_CR2_START)
#define  I2C_RELOAD_MODE                I2C_CR2_RELOAD
#define  I2C_AUTOEND_MODE               I2C_CR2_AUTOEND
#define  I2C_SOFTEND_MODE               (0x00000000U)
#define I2C_CR2_NBYTES_Pos           (16U)                                     
#define LSM303DLHC_BLE_LSB                 ((uint8_t)0x00) /*!< Little Endian: data LSB @ lower address */
#define LSM303DLHC_BLE_MSB                 ((uint8_t)0x40) /*!< Big Endian: data MSB @ lower address */
#define ACC_I2C_ADDRESS                      0x32
#define MAG_I2C_ADDRESS                      0x3C
#define LSM303DLHC_CTRL_REG1_A               0x20  /* Control register 1 acceleration */
#define LSM303DLHC_CTRL_REG2_A               0x21  /* Control register 2 acceleration */
#define LSM303DLHC_CTRL_REG3_A               0x22  /* Control register 3 acceleration */
#define LSM303DLHC_CTRL_REG4_A               0x23  /* Control register 4 acceleration */
#define LSM303DLHC_CTRL_REG5_A               0x24  /* Control register 5 acceleration */
#define LSM303DLHC_CTRL_REG6_A               0x25  /* Control register 6 acceleration */
#define LSM303DLHC_FULLSCALE_2G            ((uint8_t)0x00)  /*!< ±2 g */
#define LSM303DLHC_FULLSCALE_4G            ((uint8_t)0x10)  /*!< ±4 g */
#define LSM303DLHC_FULLSCALE_8G            ((uint8_t)0x20)  /*!< ±8 g */
#define LSM303DLHC_FULLSCALE_16G           ((uint8_t)0x30)  /*!< ±16 g */
#define LSM303DLHC_ACC_SENSITIVITY_2G     ((uint8_t)1)  /*!< accelerometer sensitivity with 2 g full scale [mg/LSB] */
#define LSM303DLHC_ACC_SENSITIVITY_4G     ((uint8_t)2)  /*!< accelerometer sensitivity with 4 g full scale [mg/LSB] */
#define LSM303DLHC_ACC_SENSITIVITY_8G     ((uint8_t)4)  /*!< accelerometer sensitivity with 8 g full scale [mg/LSB] */
#define LSM303DLHC_ACC_SENSITIVITY_16G    ((uint8_t)12) /*!< accelerometer sensitivity with 12 g full scale [mg/LSB] */

#include "stm32f30x_it.h"
#include "stm32f30x.h"

void  I2C_Init()
{
	I2C1->CR1 &= ~I2C_CR1_PE;

	I2C1->TIMINGR = 0xC062121F & TIMING_CLEAR_MASK;
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
			return (0);
		}
	}

	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) | ((uint32_t)(((uint32_t)DeviceAddr & I2C_CR2_SADD) | (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_SOFTEND_MODE | (uint32_t)I2C_GENERATE_START_WRITE));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TXIS) == 0)
	{
		if((timeout--) == 0)
		{
			return (0);
		}
	}

	I2C1->TXDR = ((uint8_t)((uint16_t)((RegisterAddr) & (uint16_t)(0x00FFU))));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TC) == 0)
	{
		if((timeout--) == 0)
		{
			return (0);
		}
	}

	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) | ((uint32_t)(((uint32_t)DeviceAddr & I2C_CR2_SADD) | (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)I2C_GENERATE_START_READ));

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_RXNE) == 0)
	{
		if((timeout--) == 0)
		{
			return (0);
		}
	}
	data = (uint8_t)I2C1->RXDR;

	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_STOPF) == 0)
	{
		if((timeout--) == 0)
		{
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


void LSM303DLHC_GetData(uint8_t *pDataXYZ)
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
	// buffer[0] = READ_REGISTER(ACC_I2C_ADDRESS, OUT_X_L_A); 
	// buffer[1] = READ_REGISTER(ACC_I2C_ADDRESS, OUT_X_H_A);
	// buffer[2] = READ_REGISTER(ACC_I2C_ADDRESS, OUT_Y_L_A);
	// buffer[3] = READ_REGISTER(ACC_I2C_ADDRESS, OUT_Y_H_A);
	// buffer[4] = READ_REGISTER(ACC_I2C_ADDRESS, OUT_Z_L_A);
	// buffer[5] = READ_REGISTER(ACC_I2C_ADDRESS, OUT_Z_H_A);

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

uint8_t LSM303DLHC_Init(void)
{  
	uint8_t tmpreg;
	uint16_t RegisterConfig = 0x0000;

	RegisterConfig = 0x0847;
	ENABLE_LSM303DLHC_INT();
	WRITE_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG1_A, RegisterConfig);

	RegisterConfig = (uint8_t) (RegisterConfig << 8);
	WRITE_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, RegisterConfig);

	RegisterConfig = 0x90;
	tmpreg = READ_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A);
	tmpreg &= 0x0C;
	tmpreg |= RegisterConfig;
	WRITE_REGISTER(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG2_A, tmpreg);
	return(1);
}
