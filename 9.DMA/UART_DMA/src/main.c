/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/05 10:53:16 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "uart.h"
#include "dma_uart.h"

uint8_t data[6] = {0};

int main(void)
{
	RCC_USART1_enable();
	USART_enable();
	GPIOA_config();
	GPIOA_connect_PIN9_PIN10();
	USART_output("USART IS WORKING");
	USART1->CR3 |= (1 << 6);
	setup_dma1_UART_rx((uint32_t)&data[0]);
	while (1) {};
	return (0);
}
