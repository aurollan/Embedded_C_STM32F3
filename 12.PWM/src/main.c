/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 11:11:06 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/04 17:16:55 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "leds.h"
#include "timer.h"
#include "systick.h"

int main(void)
{
	/* LED */
	init_leds();

	/* Delay */
	TIM6_enable();

	/* Systick interrupt */
	init_systick();

	/* LED is always of except for 1s when interrupt occurs */
	while (1)
	{
		switch_off_leds();
	}
	return (0);
}

