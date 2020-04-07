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

#include "user_button.h"

int main(void)
{
	setup_user_button();
	init_leds();

	ligth_up_leds();
	while(1)
	{
		while ((GPIOA->IDR & ((uint32_t)0x01)) == 0) {};
		while ((GPIOA->IDR & ((uint32_t)0x01)) == 1) {};
		ligth_up_leds();
		while ((GPIOA->IDR & ((uint32_t)0x01)) == 0) {};
		while ((GPIOA->IDR & ((uint32_t)0x01)) == 1) {};
		ligth_down_leds();
	}
	return 1;
}

