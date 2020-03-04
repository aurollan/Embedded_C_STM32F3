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
