#ifndef __SPI_H__
#define __SPI_H__

#include "itm.h"
#include "timer.h"
#include "stm32f30x.h"
#include "stm32f30x_it.h"


void	L3GD20Gyro_Init(void);
void	L3GD20Gyro_GetData(uint8_t *data);
uint8_t	L3GD20Gyro_read_register(uint8_t register_address);
void	L3GD20Gyro_write_register(uint8_t register_address, uint8_t config);
void	enable_GPIOE_int(void);
void	enable_GPIOA_af(void);
void	enable_SPI1(void);
void	print_data(uint8_t *data, uint8_t len);
void			ft_print_hexa(uint8_t data);
void	setup_gyro();
#endif
