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

#include "leds.h"
#include "stm32f1xx.h"

int main(void)
{
	init_leds();
	switch_on_leds();
	/* Uncomment function to test but keep in mind that you won't see
	 * any blinking because it's too fast for our eyes */
	/* switch_off_leds(); */
	while (1) {};
	return (0);
}
