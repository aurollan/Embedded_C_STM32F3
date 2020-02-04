/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   i2c.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aurollan <aurollan@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/04 17:52:07 by aurollan     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/04 17:52:11 by aurollan    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "drone.h"


/* 
 *	RCC->I2CCLK
 *	I2C enble by PE bit in I2C_CR1
 *	CTRL_REG1_A (ODR1/2/3/0 Lpen Z/X/Yen)
 *
 *	I2C initializtion flowchart
 *	Initial settings
 *	Clear PE bit in I2C_CR1
 *	COnfigure ANFOFF and DNF[3:0] in I2C_CR1
 *	configure  PRESC/SDADEL/SCLDEL/SCLH/SCLL in I2C_TIMINGR
 *	COnfigure NOSTRETCH in I2C_CR1
 *	Set PE bin in I2C_CR1
 *	end
 *	
 *	I2C_CR1 : RXIE/TXIE/PE
 */
