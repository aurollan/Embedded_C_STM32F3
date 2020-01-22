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

@par Example Description 

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
 
@par Directory contents 

  - GPIO_IOToggle/stm32f30x_conf.h     Library Configuration file
  - GPIO_IOToggle/stm32f30x_it.c       Interrupt handlers
  - GPIO_IOToggle/stm32f30x_it.h       Header for stm32f30x_it.c
  - GPIO_IOToggle/main.c               Main program
  - GPIO_IOToggle/main.h               Header for main.c
  - GPIO_IOToggle/system_stm32f30x.c   STM32F30x system source file
           
@note The "system_stm32f30x.c" file contains the system clock configuration for
      STM32F30x devices, and is customized for use with STM32F3-Discovery Kit. 
      The STM32F30x is configured to run at 72 MHz, following the three  
      configuration below:
        + PLL_SOURCE_HSI
           - HSI (~8 MHz) used to clock the PLL, and the PLL is used as system 
             clock source.  
        + PLL_SOURCE_HSE          
           - HSE (8 MHz) used to clock the PLL, and the PLL is used as system
             clock source.
           - The HSE crystal is not provided with the Discovery Kit, some 
             hardware modification are needed in manner to connect this crystal.
             For more details, refer to section "4.10 OSC clock" in "STM32F3 discovery kit User manual (UM1570)"
        + PLL_SOURCE_HSE_BYPASS   
           - HSE bypassed with an external clock (fixed at 8 MHz, coming from 
             ST-Link circuit) used to clock the PLL, and the PLL is used as 
             system clock source.
           - Some  hardware modification are needed in manner to bypass the HSE 
             with clock coming from the ST-Link circuit.
             For more details, refer to section "4.10 OSC clock" in "STM32F3 discovery kit User manual (UM1570)"
      User can select one of the three configuration in system_stm32f30x.c file
      (default configuration is PLL_SOURCE_HSE_BYPASS).           

