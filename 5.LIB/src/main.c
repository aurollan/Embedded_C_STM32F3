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

#include "itm.h"
#include <stdio.h>

int puts(const char *s)
{
	while (*s)
	{
		ITM_SendChar(*s);
		s++;
	}
	return (1);
}


int main(void)
{
	ITM_init();
	printf("Hello World %d\n", 0x56);
	while(1)
	{
	}
	return 1;
}

