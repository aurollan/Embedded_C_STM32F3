#ifndef __DMA_H__
#define __DMA_H__ 

#include "stm32f3xx.h"
#include "timer.h"
#include "itm.h"
#include "spi.h"

void setup_dma1_SPI1_rx(uint32_t array_addr);
void DMA1_Channel2_IRQHandler(void);
void			ft_print_hexa(uint8_t data);

#endif
