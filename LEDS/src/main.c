/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/06 16:07:13 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "stm32f30x_it.h"
#include "stm32f30x.h"

void	leds(void)
{
	/* Enable GPIOE */
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;

	/* Set all GPIOE to output mode */
	GPIOE->MODER = 0x55555555;      /*!< GPIO port mode register, set all value to 01 (output mode) */

	/* Light up all leds with ODR register */
	GPIOE->ODR = (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8);

	/* Light up all leds with BSRR register */
	GPIOE->BSRR = (1 << 15);
	GPIOE->BSRR = (1 << 14);
	GPIOE->BSRR = (1 << 13);
	GPIOE->BSRR = (1 << 12);
	GPIOE->BSRR = (1 << 11);
	GPIOE->BSRR = (1 << 10);
	GPIOE->BSRR = (1 << 9);
	GPIOE->BSRR = (1 << 8);
}

int main(void)
{
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
