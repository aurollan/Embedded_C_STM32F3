#include "i2c.h"


void  I2C_Init_Mag()
{
	I2C1->CR1 &= ~I2C_CR1_PE;

	// I2C1->TIMINGR = 0xC062121F & TIMING_CLEAR_MASK;
	I2C1->TIMINGR = 0xE0000303 & TIMING_CLEAR_MASK;
	I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
	I2C1->OAR1 = (I2C_OAR1_OA1EN | ACCELERO_I2C_ADDRESS);
	I2C1->CR2 |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);
	I2C1->OAR2 &= ~I2C_DUALADDRESS_ENABLE;
	I2C1->OAR2 = (I2C_DUALADDRESS_DISABLE | 0x00 | (0x00 << 8));
	I2C1->CR1 = (I2C_GENERALCALL_DISABLE | I2C_NOSTRETCH_DISABLE);
	I2C1->CR1 |= I2C_CR1_PE;
}

void ENABLE_GPIOB_SCA_SCL_MAG(void)
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
	I2C_Init_Mag();
}

void ENABLE_LSM303DLHC_INT_MAG(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;

	/* Those pin are in input mode and speed_3  and NOPULL*/
	GPIOE->OSPEEDR |= GPIO_SPEED_FREQ_HIGH << 5 * 2;      /*!< GPIO port output speed register to maximum speed */
	GPIOE->OSPEEDR |= GPIO_SPEED_FREQ_HIGH << 4 * 2;      /*!< GPIO port output speed register to maximum speed */
	ENABLE_GPIOB_SCA_SCL_MAG();
}

uint8_t LSM303DLHCMag_Init(void)
{
	ENABLE_LSM303DLHC_INT_MAG();

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

void LSM303DLHCMag_GetData(uint8_t *pDataXYZ)
{
	pDataXYZ[0] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_L_M);
	pDataXYZ[1] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M);
	pDataXYZ[2] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_M);
	pDataXYZ[3] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_M);
	pDataXYZ[4] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_M);
	pDataXYZ[5] = READ_REGISTER(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_M);
}
