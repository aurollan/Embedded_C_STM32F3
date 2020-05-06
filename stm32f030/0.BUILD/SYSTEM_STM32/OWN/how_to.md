# How to setup STM32F3 Discovery system clock

## Check the device Datasheet
Looking for clocks we find page 18 - 3.9 Clocks and Startup.

While reading we can see that the default clock used to start the device is the internal RC 8 MHz.

## Check the Reference Manual
page 125 9.2 Clocks
# Choose the derired clock
According to the datasheet and the Reference Manual we have 4 clock for our microcontroller:

	- HSI RC => 8 Mhz
	- HSE OSC => 4-32 Mhz
	- LSE OSC => 32.768 Khz
	- LSI RC => 40 Khz

But only 2 are connected to SYSCLK and preipheral buses:

	- HSI RC
	- HSE OSC

## How to read it ?
	- H => Hight
	- L => Low
	- S => Speed
	- I => Internal
	- E => External

## Peripheral max speed
Page 125 of the Reference Manual
The maximum frequency of the AHB and
APB2 domains is 72 MHz. The maximum allowed frequency of the APB1 domain is 36 MHz.
APB2 => 72 Mhz
APB1 => 36 Mhz



page 130

The HSE Crystal can be switched on and off using the HSEON bit in the Clock control
register (RCC_CR).
The HSERDY flag in the Clock control register (RCC_CR) indicates if the HSE oscillator is
stable or not.
Calibration
RC oscillator frequencies can vary from one chip to another due to manufacturing process
variations, this is why each device is factory calibrated by ST for 1% accuracy at T A =25°C.

page 131
To modify the PLL configuration, proceed as follows:
1. Disable the PLL by setting PLLON to 0.
2. Wait until PLLRDY is cleared. The PLL is now fully stopped.
3. Change the desired parameter.
4. Enable the PLL again by setting PLLON to 1.
An interrupt can be generated when the PLL is ready, if enabled in the Clock interrupt
register (RCC_CIR).
The PLL output frequency must be set in the range 16-72 MHz.
