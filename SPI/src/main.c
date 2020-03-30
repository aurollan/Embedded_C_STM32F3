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
	uint8_t data[6] = {0};

	ITM_init();
	TIM6_enable();
	L3GD20Gyro_Init();
	setup_gyro();
	while (1)
	{
		L3GD20Gyro_GetData(&data[0]);
		print_data(&data[0], 6);
		delay(60000);
	}
	return (0);
}

