/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/31 18:16:31 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "stm32f30x_it.h"
#include "stm32f30x.h"
#include "stm32f30x_conf.h"
#include "drone.h"

void echo_back()
{
	while(1) // Don't want to exit
	{
		uint16_t ch;
		while(!(USART1->ISR & USART_ISR_RXNE)); // Wait for Empty
		ch = (USART1->RDR & (uint16_t)0x01FF);
		while(!(USART1->ISR & USART_ISR_TXE)); // Wait for Empty
		USART1->TDR = ch & 0x01FF;
	}
}

void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	/* USART resources configuration (Clock, GPIO pins and USART registers) ----*/
	/* USART configured as follow:
	   - BaudRate = 9600 baud
	   - Word Length = 8 Bits
	   - One Stop Bit
	   - No parity
	   - Hardware flow control disabled (RTS and CTS signals)
	   - Receive and transmit enabled
	   */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* USART configuration */
	USART_Init(USART1, &USART_InitStructure);
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}

void USART_output(char *s)
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
	RCC_USART1_enable();
	GPIOA_connect_PIN9_PIN10();
	GPIOA_config();
	USART_enable();
	// USART1_Configuration();

	_write(0, "Hello World\n", 13);
	USART_output("bonjour monsieur");
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
