/*
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
*/
