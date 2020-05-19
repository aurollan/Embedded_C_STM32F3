#ifndef __SPI_H__
#define __SPI_H__

#include "itm.h"
#include "timer.h"
#include "stm32f3xx.h"


void	L3GD20Gyro_Init(void);
void	L3GD20Gyro_GetData(uint8_t *data);
uint8_t	L3GD20Gyro_read_register(uint8_t register_address);
void	L3GD20Gyro_write_register(uint8_t register_address, uint8_t config);
void	enable_GPIOE_int(void);
void	enable_GPIOA_af(void);
void	enable_SPI1(void);
void	print_data(uint8_t *data, uint8_t len);
void	ft_print_hexa(uint8_t data);
void	setup_gyro();

typedef struct	s_pos_data
{
	int16_t x;
	int16_t y;
	int16_t z;
}				t_pos_data;

typedef struct	s_angle_data
{
	float x;
	float y;
	float z;
}				t_angle_data;

void calibrate_gyr(t_pos_data *offset);
void read_data_gyr(uint8_t data[6], t_pos_data offset, t_pos_data *value);
void convert_to_angle_gyr(t_angle_data *angle,
								t_pos_data value);
#endif
