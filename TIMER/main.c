/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/26 17:53:44 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "stm32f30x_it.h"
#include "stm32f30x.h"
#include "stm32f30x_conf.h"
#include "drone.h"

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
	BSP_ACCELERO_Init();
	while (1)
	{
		BSP_ACCELERO_GetXYZ(&data[0]);
		_write(0, "DATA\n", 5);
		int a = 0;
		while (a < 6)
		{
			ft_print_hexa(data[a]);	
			a++;
		}
		_write(0, "\n----\n", 6);
		delay(60000);
	}
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
