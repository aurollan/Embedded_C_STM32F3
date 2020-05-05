#include "dma.h"

extern uint8_t data[6];

void setup_dma1_i2c1_rx(uint32_t array_addr)
{
	// Enable sysCFG that set DMA channel  ?
	RCC->APB2ENR |= 0x1;
	// Enable DMA clock before configuring
	RCC->AHBENR |= 0x1;
	// I2C1_RX is on channel 7
	// Reset DMA configuration (keeping reserved bits value)
	DMA1_Channel7->CCR &= ~(uint32_t)(0x00007FFF);
	DMA1_Channel7->CNDTR &= ~(uint32_t)(0x0000FFFF);
	DMA1_Channel7->CPAR = 0x0;
	DMA1_Channel7->CMAR = 0x0;

	// Now we set only the bit we need
	// All other values are default values
	
	// peripheral base address
	DMA1_Channel7->CPAR = (uint32_t)(&(I2C1->RXDR));

	// memory address
	DMA1_Channel7->CMAR = array_addr;

	// 6 byte to transfert each time
	DMA1_Channel7->CNDTR |= 0x6;

	// Channel priority level medium
	DMA1_Channel7->CCR |= (1 << 12);

	// Memory increment mode enable
	DMA1_Channel7->CCR |= (1 << 7);
	// Circular mode enable
	DMA1_Channel7->CCR |= (1 << 5);
	// Transfert complete interrupt enable
	DMA1_Channel7->CCR |= (1 << 1);

	// Enable Interruption
 	NVIC_EnableIRQ(DMA1_Channel7_IRQn);

	// Enable the channel when configuration is done
	DMA1_Channel7->CCR |= 1;

}

void DMA1_Channel7_IRQHandler(void)
{
	// Write collected data
	_write(0, "HELLO FROM INTERRUPT\n", 21);
	_write(0, "DATA\n", 5);
	int a = 0;
	while (a < 6)
	{
		ft_print_hexa(data[a]);	
		a++;
	}
	_write(0, "\n----\n", 6);
	
	// Clear Transfert Complete Interrupt
	DMA1->IFCR |= (1 << 25);
}


void I2C_DMARX_init()
{
	I2C1->CR1 |= (1 << 15);
}

void LSM303DLHC_GetData_MR_DMA2( uint8_t device_addr, 
						   uint8_t register_addr_start)
{
	uint32_t timeout = I2C_TIMEOUT;


	// Set the auto-increment bit as expected by the device
	register_addr_start |= 0x80;

	// Check if line is used by another master, but usefull for debug purpose
	while ((I2C1->ISR & I2C_ISR_BUSY) != 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRBUSY\n", 14);
			return ;
		}
	}

	/*************************************************************************/
	/*  							Frame 1			 						 */
	/*************************************************************************/
	// Reinit CR2
	// Send device addr (Accelerometer or Magnetometer)
	// 1 byte len
	// Software en mode wait for me to set the stop bit
	// Ask for write (0x00)
	// Send the start signal
	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) \
				| ((uint32_t)(((uint32_t)device_addr & I2C_CR2_SADD) \
				| (((uint32_t)1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) \
				| (uint32_t)I2C_SOFTEND_MODE \
				| (uint32_t)I2C_GENERATE_START_WRITE));

	// Wait for the transmission buffer to be ready (empty)
	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TXIS) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRTXIS\n", 14);
			return ;
		}
	}

	// Write the register address to want to read from
	I2C1->TXDR = ((uint8_t)((uint16_t)((register_addr_start) & (uint16_t)(0x00FFU))));

	// Wait the transfer to complete
	timeout = I2C_TIMEOUT;
	while ((I2C1->ISR & I2C_ISR_TC) == 0)
	{
		if((timeout--) == 0)
		{
			_write(0, "ERROR ISRTC\n", 12);
			return ;
		}
	}

	/*************************************************************************/
	/*  							Frame 2			 						 */
	/*************************************************************************/
	// Reinit CR2
	// Send register addr with the auto 0increment byte set
	// 6 byte len
	// Autoend modem hard send STOP after last byte read
	// Ask for read (0x01)
	// Send the start signal
	I2C1->CR2 = (I2C1->CR2 & 0xF8000000U) \
				| ((uint32_t)(((uint32_t)device_addr & I2C_CR2_SADD) \
				| (((uint32_t)6 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) \
				|  (uint32_t)I2C_AUTOEND_MODE \
				| (uint32_t)I2C_GENERATE_START_READ));
}
