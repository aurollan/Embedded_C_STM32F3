#ifndef __DMA_UART_H__
#define __DMA_UART_H__

void DMA1_Channel5_IRQHandler(void);
void setup_dma1_UART_tx(uint32_t array_addr);
void setup_dma1_UART_rx(uint32_t array_addr);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);

#endif
