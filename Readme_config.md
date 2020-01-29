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


/* 
p1114

By default, these pin is NOT assigned. It can be assigned by setting the TRACE_IOEN and
TRACE_MODE bits in the Debug MCU configuration register (DBGMCU_CR). This
configuration has to be done by the debugger host.a

To assign the TRACE pin, the debugger host must program the bits TRACE_IOEN and
TRACE_MODE[1:0] of the Debug MCU configuration Register (DBGMCU_CR). By default
the TRACE pins are not assigned.
This register is mapped on the external PPB and is reset by the PORESET (and not by the
SYSTEM reset). It can be written by the debugger under SYSTEM reset.


p1104
The packets emitted by the ITM are output to the TPIU (Trace Port Interface Unit). The
formatter of the TPIU adds some extra packets (refer to TPIU) and then output the complete
packets sequence to the debugger host.
The bit TRCEN of the Debug Exception and Monitor Control Register must be enabled
before programming or using the ITM.

Write 0xC5ACCE55 to unlock Write Access to the other ITM
registers
Example of configuration
To output a simple value to the TPIU:
• Configure the TPIU and assign TRACE I/Os by configuring the DBGMCU_CR (refer to
Section 33.17.2: TRACE pin assignment and Section 33.16.3: Debug MCU
configuration register)
• Write 0xC5ACCE55 to the ITM Lock Access Register to unlock the write access to the
ITM registers
• Write 0x00010005 to the ITM Trace Control Register to enable the ITM with Sync
enabled and an ATB ID different from 0x00
• Write 0x1 to the ITM Trace Enable Register to enable the Stimulus Port 0
• Write 0x1 to the ITM Trace Privilege Register to unmask stimulus ports 7:0
• Write the value to output in the Stimulus Port Register 0: this can be done by software
(using a printf function)

config p1106 and p1119
*/

You must enable TRCENA of the Debug Exception and Monitor Control Register (DEMCR) before you program or use the ITM

 ITM registers are fully accessible in privileged mode. In user mode, all registers can be read,
but only the Stimulus Registers and Trace Enable Registers can be written, and only when the
corresponding Trace Privilege Register bit is set. Invalid user mode writes to the ITM registers
are discarded.
If the debugger host wants to switch to the SW-DP, it must provide a dedicated JTAG
sequence on TMS/TCK (respectively mapped to SWDIO and SWCLK) which disables the
JTAG-DP and enables the SW-DP. This way it is possible to activate the SWDP using only
the SWCLK and SWDIO pins.
This sequence is:
1. Send more than 50 TCK cycles with TMS (SWDIO) =1
2. Send the 16-bit sequence on TMS (SWDIO) = 0111100111100111 (MSB transmitted
first)
3. Send more than 50 TCK cycles with TMS (SWDIO) =1


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
