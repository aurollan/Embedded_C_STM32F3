/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/13 14:38:48 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "spi.h"

int main(void)
{
	uint8_t data[6];
	t_pos_data value;
	t_pos_data offset;
	t_angle_data angle;

	angle.x = 0.0f;
	angle.y = 0.0f;
	angle.z = 0.0f;


	ITM_init();
	TIM6_enable();
	L3GD20Gyro_Init();

	// normal mode
	// enable x/y/z
	// 95 Hz ODR 12.5 cut-off
	L3GD20Gyro_write_register(0x20, 0x0F);
	// continuous update
	// little indian
	// 250 dps (more accurate)
	// SPI 4 wire interface
	L3GD20Gyro_write_register(0x23, 0x00);

	calibrate_gyr(&offset);
	while(1)
	{
		// get data
		data[0] = L3GD20Gyro_read_register(0x28); 
		data[1] = L3GD20Gyro_read_register(0x29); 
		data[2] = L3GD20Gyro_read_register(0x2a); 
		data[3] = L3GD20Gyro_read_register(0x2b); 
		data[4] = L3GD20Gyro_read_register(0x2c); 
		data[5] = L3GD20Gyro_read_register(0x2d); 
		
		// read data
		read_data_gyr(data, offset, &value);
		convert_to_angle_gyr(&angle, value);
		(void)angle;
	}
	return 1;
}

