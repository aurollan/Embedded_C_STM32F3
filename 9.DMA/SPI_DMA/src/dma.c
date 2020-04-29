#include "dma.h"

extern uint8_t data[6];

void setup_dma1_SPI1_rx(uint32_t array_addr)
{
	// Enable sysCFG that set DMA channel  ?
	RCC->APB2ENR |= 0x1;
	// Enable DMA clock before configuring
	RCC->AHBENR |= 0x1;
	// SPI1_RX is on channel 2
	// Reset DMA configuration (keeping reserved bits value)
	DMA1_Channel2->CCR &= ~(uint32_t)(0x00007FFF);
	DMA1_Channel2->CNDTR &= ~(uint32_t)(0x0000FFFF);
	DMA1_Channel2->CPAR = 0x0;
	DMA1_Channel2->CMAR = 0x0;

	// Now we set only the bit we need
	// All other values are default values
	
	// peripheral base address
	DMA1_Channel2->CPAR = (uint32_t)(&(SPI1->DR));

	// memory address
	DMA1_Channel2->CMAR = array_addr;

	// 6 byte to transfert each time
	DMA1_Channel2->CNDTR |= 0x6;

	// Channel priority level medium
	DMA1_Channel2->CCR |= (1 << 12);

	// Memory increment mode enable
	DMA1_Channel2->CCR |= (1 << 7);
	// Circular mode enable
	DMA1_Channel2->CCR |= (1 << 5);
	// Transfert complete interrupt enable
	DMA1_Channel2->CCR |= (1 << 1);

	// Enable Interruption
 	NVIC_EnableIRQ(DMA1_Channel2_IRQn);

	// Enable the channel when configuration is done
	DMA1_Channel2->CCR |= 1;

}

void DMA1_Channel2_IRQHandler(void)
{
	// Write collected data
	_write(0, "DATA\n", 5);
	int a = 0;
	while (a < 6)
	{
		ft_print_hexa(data[a]);	
		a++;
	}
	_write(0, "\n----\n", 6);
	
	// Clear Transfert Complete Interrupt
	DMA1->IFCR |= (1 << 5);

	// OR clear all interrupt
	DMA1->IFCR |= (1 << 4);
}
