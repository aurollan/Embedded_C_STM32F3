/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   drone.h                                          .::    .:/ .      .::   */ /*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 12:53:25 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/18 13:11:40 by aurollan    ###    #+. /#+    ###.fr     */
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

/*
** i2c.c
*/
void	I2C_enable();
int	I2C_communicate();
void i2c1_initp(void);
uint8_t i2c_read(uint8_t DeviceAddr, uint8_t* data);
uint8_t i2c_set(uint8_t DeviceAddr, uint8_t RegAddr);
uint8_t BSP_ACCELERO_Init(void);
void BSP_ACCELERO_GetXYZ(uint8_t *pDataXYZ);
void  COMPASSACCELERO_WRITE_REGISTER(uint16_t DeviceAddr, uint16_t RegisterAddr, uint8_t RegisterConfig);
uint8_t  COMPASSACCELERO_READ_REGISTER(uint16_t DeviceAddr, uint16_t RegisterAddr);
void HAL_I2C_Init();
void I2Cx_MspInit();
void I2Cx_Init(void);
void COMPASSACCELERO_IO_Init(void);

/*
** main.c
*/
void			ft_print_hexa(uint8_t data);

#endif
