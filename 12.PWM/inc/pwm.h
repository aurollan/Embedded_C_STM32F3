#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f303xc.h"
#include "leds.h"

void pwm(uint16_t frequency, uint16_t duty_cycle);
void TIM16_enable(void);

#endif

