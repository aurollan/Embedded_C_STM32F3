/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   drone.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 12:53:25 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/30 14:27:27 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef DRONE_H
#define DRONE_H
#include "stm32f30x_it.h"
#include "stm32f30x.h"
#include "stm32f30x_conf.h"

void GPIOE_full_init(void);
void GPIOE_enable(void);
void delay(int delay);
void TIM6_enable(void);
void ITM_init(void);
void leds(void);
int _write(int32_t file, char* ptr, int32_t len);
void USART_enable();
#endif
