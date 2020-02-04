/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   drone.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 12:53:25 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/04 17:55:18 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef DRONE_H
#define DRONE_H

#include "stm32f30x.h"
#include "stm32f30x_it.h"
#include "stm32f30x_conf.h"

/*
** leds.c
*/
void	GPIOE_full_init(void);
void	GPIOE_enable(void);
void	leds(void);

/*
** delay.c
*/
void	delay(uint16_t delay);
void	TIM6_enable(void);

/*
** itm.c
*/
void	ITM_init(void);
int		_write(int32_t file, char* ptr, int32_t len);

/*
** usart.c
*/
void	USART_enable();
void	GPIOA_connect_PIN9_PIN10();
void	GPIOA_config();
void	RCC_USART1_enable();
void	USART_output(char *s);
void	echo_back();

#endif
