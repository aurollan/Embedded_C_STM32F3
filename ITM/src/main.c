/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/04 16:57:56 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "stm32f30x_it.h"
#include "stm32f30x.h"

void ITM_init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	TPI->ACPR = 72000000 / 2000000 - 1;
	TPI->FFCR |= 0x100; // default value bti 8 always at 1 and bit 2 activate formatter
	TPI->SPPR = 2; // other asynchronous value : NRZ
	DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;
	ITM->TPR = ITM_TPR_PRIVMASK_Msk;
	ITM->LAR = 0xC5ACCE55;						/* ITM Lock Access Register */
	TPI->CSPSR |= (1 << 0); // 0x1 for 1 pin (default)
	ITM->TCR |= ITM_TCR_ITMENA_Msk | (1 << 3) | (1 << 16);     /* ITM enabled */
	ITM->TER |= (1UL << 0); 			/* ITM Port #0 enabled */
}

int _write(int32_t file, char* ptr, int32_t len)
{
	(void)file;
	int i=0;
	for(i=0 ; i<len ; i++)
		ITM_SendChar((*ptr++));
	return i;
}

int main(void)
{
	ITM_init();
	_write(0, "Hello Wolrd\n", 12);
	return (0);
}
