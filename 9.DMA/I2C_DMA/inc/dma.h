#ifndef __DMA_H__
#define __DMA_H__ 

#include "stm32f3xx.h"
#include "i2c.h"
#include "timer.h"
#include "itm.h"

void setup_dma1_i2c1_rx(uint32_t array_addr);
void DMA1_Channel7_IRQHandler(void);
void			ft_print_hexa(uint8_t data);

void I2C_DMARX_init();
void LSM303DLHC_GetData_MR_DMA(uint8_t device_addr);

void LSM303DLHC_GetData_MR_DMA2( uint8_t device_addr, 
						   uint8_t register_addr_start);
#endif
