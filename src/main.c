/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/12 10:45:04 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "stm32f30x_it.h"
#include "stm32f30x.h"
#include "stm32f30x_conf.h"
#include "drone.h"

void			ft_print_hexa(uint32_t nb)
{
	int		index;
	char	hex_char;
	int		bit_offset;

	index = 0;
	bit_offset = sizeof(uint32_t) * 8 - 4;
	_write(0, "0x", 2);
	while (bit_offset >= 0)
	{
		if (((nb >> bit_offset) & 0b1111) > 9)
			hex_char = ((nb >> bit_offset) & 0b1111) + 55;
		else
			hex_char = ((nb >> bit_offset) & 0b1111) + 48;
		if (index != 0 || hex_char != '0')
		{
			_write(0, &hex_char, 1);
			index++;
		}
		bit_offset -= 4;
	}
}

int main(void)
{
	ITM_init();
	TIM6_enable();
	RCC_USART1_enable();
	GPIOA_connect_PIN9_PIN10();
	GPIOA_config();
	USART_enable();


	_write(0, "Hello World\n", 13);
	ft_print_hexa((uint32_t) 10);
	USART_output("bonjour monsieur");
	I2C_enable();
	I2C_communicate();
	echo_back();
	leds();
	return (0);
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	(void)file;
	(void)line;
	/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
