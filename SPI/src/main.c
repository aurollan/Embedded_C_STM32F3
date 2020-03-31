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

	ITM_init();
	TIM6_enable();
	L3GD20Gyro_Init();

	L3GD20Gyro_write_register(0x20, 0x0F);
	L3GD20Gyro_write_register(0x23, 0x30);

	while(1)
	{
		data[0] = L3GD20Gyro_read_register(0x28); //2000 DPS
		data[1] = L3GD20Gyro_read_register(0x29); //2000 DPS
		data[2] = L3GD20Gyro_read_register(0x2a); //2000 DPS
		data[3] = L3GD20Gyro_read_register(0x2b); //2000 DPS
		data[4] = L3GD20Gyro_read_register(0x2c); //2000 DPS
		data[5] = L3GD20Gyro_read_register(0x2d); //2000 DPS
		print_data(&data[0], 6);
		delay(60000);
	}
	return 1;
}

