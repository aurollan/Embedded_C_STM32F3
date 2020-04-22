#include "dma.h"


void setup_dma1_channel(uint32_t channel, uint32_t periph_addr, uint32_t array_addr)
{
	/* Peripheral register  */
	DMA1->CCR[channel] |= (1 << 12) | (1 << 7) | (1 << 6) | (1 << 5);
	/* 6 byte to transfert each time */
	DMA1->CNDTR[channel] = 6;
	/* peripheral base address */
	DMA1->CPAR[channel] = periph_addr;
	/* memory address */
	DMA1->CMAR[channel] = array_addr;
	/* Enable the channel when configuration is done */
	DMA1->CCR[channel] |= 1;

}

