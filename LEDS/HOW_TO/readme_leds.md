# LEDS

Usually the first thing we learn in embedded development is blinking a led.
Here we are going to light up one or more leds.
If you need to make it blink you need to understand how to use timer and create
a delay function.

## Where do i start ?
Read the user manual, then the reference manual. Let's start.

### User manual
https://www.st.com/content/ccc/resource/technical/document/user_manual/8a/56/97/63/8d/56/41/73/DM00063382.pdf/files/DM00063382.pdf/jcr:content/translations/en.DM00063382.pdf

There is a part called LEDs telling us which pin is connected to each led. (6.4 - page 18/37)
We work only on E-compass leds called "User LDx".
As said LEDs from 3 to 10 are connected to IO PE 8 to 15.
And that's all.

So now let's take a look to reference manual to get more information.

### Reference manual
https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf

This is were we are going to find all detailed information we need to program on this device.
Lets search for this "I/O" things we found earlier.

First things we find in "Contents" is GPIO ( General Purpose I/O). 
That seems pretty relevant. 

We see that they are used for Input/Output operation: that's what we are looking for.

They have an "x" at the end of their name and we are looking for IO PE8 to PE15.

Let's drop the number and look for IOPE.

GPIOPE ? GPIOE ? IOPE ? IOE ?

There is only 1 match with GPIOx that we see in GPIO introduction.

If we look for GPIOE we twice the same information :
- Section 11.4.12 on page 243

Table 73. GPIO register map and reset values

So what are those register for ? Just scroll up to read it.

Are we going to read all this documentation ?

No we will focus on our goal: output a led signal.

/* Initialize all led form led-compass */
/* REFERENCE MANUAL P228 General-purpose I/Os (GPIO)*/
GPIO have input and output mode, so let's put all to output mode.
Let's set each pin now to see them light up.

/* Enable led-compass led */
/* REFERENCE MANUAL P166 RCC register map */
Not working yet ?
That normal, just look for IOPE.
You should end up page 148 with a helpful note.

Get it ?
Peripheral clock is not enable. Just enable it before doing any work on GPIOE and that's it.
Leds should light up.











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
