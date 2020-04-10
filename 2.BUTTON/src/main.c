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
	init_leds();
	setup_user_button();

	while(1)
	{
		/* Wait User button to be pushed */
		while ((GPIOA->IDR & ((uint32_t)0x01)) == 0) {};
		/* Wait User button to be released */
		while ((GPIOA->IDR & ((uint32_t)0x01)) == 1) {};
		switch_on_leds();
		/* Wait User button to be pushed */
		while ((GPIOA->IDR & ((uint32_t)0x01)) == 0) {};
		/* Wait User button to be released */
		while ((GPIOA->IDR & ((uint32_t)0x01)) == 1) {};
		switch_off_leds();
	}
	return 1;
}

