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

void TIM6_enable(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->CR1 |= (uint16_t)(1 << 3);
}

void delay(uint16_t ms)
{
	TIM6->PSC = (uint16_t)799;
	TIM6->ARR = ms;
	TIM6->CR1 |= (uint16_t)1;
	while (TIM6->SR == 0);
	TIM6->SR = 0;
}

void ITM_init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	TPI->ACPR = 72000000 / 2000000 - 1;
	TPI->FFCR |= 0x100;
	TPI->SPPR = 2;
	DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
	ITM->TPR = ITM_TPR_PRIVMASK_Msk;
	ITM->LAR = 0xC5ACCE55;
	TPI->CSPSR |= (1 << 0);
	ITM->TCR |= ITM_TCR_ITMENA_Msk | (1 << 3) | (1 << 16);
	ITM->TER |= (1UL << 0);
}

int _write(uint32_t file, char* ptr, int len)
{
	(void)file;
	int i=0;
	for(i=0 ; i < len ; i++)
		ITM_SendChar((*ptr++));
	return i;
}

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
	while (1)
	{
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

