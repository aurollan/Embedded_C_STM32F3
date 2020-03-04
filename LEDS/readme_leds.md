GPIO ports are connected on AHB bus, using BSRR and BRR registers one cycle is
required to set a pin and another one cycle to reset it. So GPIO pins can toggle
at AHB clock divided by 2.
 - Set PE14 and PE15 by setting corresponding bits in BSRR register
 - Reset PE14 and PE15 by setting corresponding bits in BRR register
In this example, HCLK is configured at 72 MHz so PE14 and PE15 toggles at 36MHz.
To achieve the maximum IO toggling frequency, you have to configure your compiler
options for high speed optimization.
  - GPIO/GPIO_Toggle/stm32f30x_conf.h    Library Configuration file
  - GPIO/GPIO_Toggle/stm32f30x_it.c      Interrupt handlers
  - GPIO/GPIO_Toggle/stm32f30x_it.h      Interrupt handlers header file
  - GPIO/GPIO_Toggle/main.c              Main program
  - GPIO/GPIO_Toggle/system_stm32f30x.c  STM32F30x system source file


GPIO ports are connected on AHB bus, using BSRR and BRR registers one cycle is
required to set a pin and another one cycle to reset it. So GPIO pins can toggle
at AHB clock divided by 2.

This example describes how to use BSRR and BRR registers for maximum IO toggling.

PE14 and PE15 when using the STM32F3-Discovery kit (configured in output pushpull 
mode) toggles in a forever loop:
 - Set PE14 and PE15 by setting corresponding bits in BSRR register
 - Reset PE14 and PE15 by setting corresponding bits in BRR register

In this example, HCLK is configured at 72 MHz so PE14 and PE15 toggles at 36MHz.
To achieve the maximum IO toggling frequency, you have to configure your compiler
options for high speed optimization.
 
To use the serial wire DP to release some GPIOs, the user software must change the GPIO
(PA15, PB3 and PB4) configuration mode in the GPIO_MODER register.This releases
PA15, PB3 and PB4 which now become available as GPIOs.

  // 	 (		0x07		) << ((						0x09		  &   0x07		  ) * 4)
  temp = ((uint32_t)(GPIO_AF) << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4));
  //temp = (	0x07		) << ((						0x01						  ) * 4)
  //temp = (	0x07		) << (							0x04						   )
  //temp = 0x070

  // GPIOA->AFR[		1		   ] &= ~(	0xF			<< ((		0x09						&		0x07	) * 4)
  GPIOx->AFR[GPIO_PinSource >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4));
  // GPIOA->AFR[		1		   ] &= ~(	0xF			<< ((					0x01							) * 4)
  // GPIOA->AFR[		1		   ] &= ~(	0xF			<< ( 					0x04								))
  // GPIOA->AFR[		1		   ] &= ~(								0x0F0										 )
  // GPIOA->AFR[		1		   ] &= 0xFFFFFF0F

  // GPIOA->AFR		 [ 			1			] | temp
  temp_2 = GPIOx->AFR[GPIO_PinSource >> 0x03] | temp;
  // GPIOA->AFR		 [ 			1			] | 0x070
  // temp2 = GPIOA->AFR		 [ 			1			] | 0x070

  // GPIOA->AFR		 [ 			1			] = temp2
  GPIOx->AFR[GPIO_PinSource >> 0x03] = temp_2;
  // GPIOA->AFR		 [ 			1			] = temp2
