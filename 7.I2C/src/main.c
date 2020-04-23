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

#include "i2c.h"
#include "itm.h"
#include "timer.h"

void			ft_print_hexa(uint8_t data)
{
	char	hex_char;

	if (((data & 0b11110000) >> 4) > 9)
		hex_char = ((data & 0b11110000) >> 4) + 55;
	else
		hex_char = ((data & 0b11110000) >> 4) + 48;
	_write(0, &hex_char, 1);
	if ((data & 0b1111) > 9)
		hex_char = (data & 0b1111) + 55;
	else
		hex_char = (data & 0b1111) + 48;
	_write(0, &hex_char, 1);
}

int main(void)
{
	uint8_t data[6] = {0};

	ITM_init();
	TIM6_enable();
	LSM303DLHCMag_Init();
	LSM303DLHCAcc_Init();
	while (1)
	{
		LSM303DLHC_GetData_Acc(&data[0]);
		//LSM303DLHC_GetData_Mag(&data[0]);
		_write(0, "DATA\n", 5);
		int a = 0;
		while (a < 6)
		{
			ft_print_hexa(data[a]);	
			a++;
		}
		_write(0, "\n----\n", 6);
		delay(10000);
	}
	return (0);
}

