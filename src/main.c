/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/30 16:56:04 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "stm32f30x_it.h"
#include "stm32f30x.h"
#include "stm32f30x_conf.h"
#include "drone.h"

void OutString(char *s)
{
	while(*s)
	{
		while(!(USART1->ISR & USART_ISR_TXE)); // Wait for Empty
		USART1->TDR = *s++;
	}
}

int main(void)
{
	ITM_init();
	TIM6_enable();
	//USART_enable();

	_write(0, "Hello World!\n", 13);
	//OutString("Bonjour aurollan");
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
