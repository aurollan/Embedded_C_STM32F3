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

#include "dma.h"

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

// data array is global so we can print it in our interrupt function.
uint8_t data[6] = {0};

int main(void)
{
	// Debug
	ITM_init();
	TIM6_enable();

	// I2C project
	I2C_Init();
	ENABLE_GPIOB_SCA_SCL();
	LSM303DLHC_Config();

	_write(0, "DMA LAUNCHED\n", 13);
	// DMA project
	I2C_DMARX_init();
	setup_dma1_i2c1_rx((uint32_t)(&data[0]));
	// LSM303DLHC_GetData_MR_DMA(ACC_I2C_ADDRESS);
	_write(0, "WAIT FOR INT\n", 13);
	while (1)
	{
		LSM303DLHC_GetData_MR_DMA2(ACC_I2C_ADDRESS, 
						   	  LSM303DLHC_OUT_X_L_A);
		delay(10000);
	}
	return (0);
}

