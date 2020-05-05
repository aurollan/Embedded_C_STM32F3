#include "uart.h"

extern uint8_t data[6];

void setup_dma1_UART_rx(uint32_t array_addr)
{
	// Enable sysCFG that set DMA channel  ?
	RCC->APB2ENR |= 0x1;
	// Enable DMA clock before configuring
	RCC->AHBENR |= 0x1;
	// I2C1_RX is on channel 7
	// Reset DMA configuration (keeping reserved bits value)
	DMA1_Channel5->CCR &= ~(uint32_t)(0x00007FFF);
	DMA1_Channel5->CNDTR &= ~(uint32_t)(0x0000FFFF);
	DMA1_Channel5->CPAR = 0x0;
	DMA1_Channel5->CMAR = 0x0;

	// Now we set only the bit we need
	// All other values are default values
	
	// peripheral base address
	DMA1_Channel5->CPAR = (uint32_t)(&(USART1->RDR));

	// memory address
	DMA1_Channel5->CMAR = array_addr;

	// 5 byte to transfert each time
	DMA1_Channel5->CNDTR |= 0x5;

	// Channel priority level medium
	DMA1_Channel5->CCR |= (1 << 12);

	// Memory increment mode enable
	DMA1_Channel5->CCR |= (1 << 7);
	// Circular mode enable
	DMA1_Channel5->CCR |= (1 << 5);
	// Transfert complete interrupt enable
	DMA1_Channel5->CCR |= (1 << 1);

	// Enable Interruption
 	NVIC_EnableIRQ(DMA1_Channel5_IRQn);

	// Enable the channel when configuration is done
	DMA1_Channel5->CCR |= 1;

}

void DMA1_Channel5_IRQHandler(void)
{
	// Write collected data
	USART_output("\n----\n");
	USART_output("HELLO FROM INTERRUPT RX\n");
	USART_output((char *)&data[0]);
	USART_output("\n----\n");
	
	// Clear Transfert Complete Interrupt
	DMA1->IFCR |= (1 << 17);
}

