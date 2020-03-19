# STARTUP FILE

# Assembly
This file is produced by manufacturer and he decided to write it in assembly
language. It's pretty well commented so you can read comments to understand
what is happening.

Long story short:
- Copy data section to RAM (see linker script)
- Copy bss section to RAM (see linker script)
- Create an array of Exception handler function (called when exception occurs)
- call systeminit() function to initialize device
- call your main() function

### Startup ?
When you load your program, it is stored in FLASH memory (memory that keep value
when device shutdown).
When your device is plugged, first called function is Reset_handler.
This is the function in the startup file.
When shutting down, all data stored in RAM is lost but our startup function is read from FLASH and at start it will be called.

## Where do your file come from ?
Currently startup file in each project come from:

Std_Library (deprecated but still easy to understand and
work with) available in manufacturer website:
https://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html
PATH: STM32F30x_DSP_StdPeriph_Lib_V1.2.3/Libraries/CMSIS/Device/ST/STM32F30x/Source/Templates/TrueSTUDIO

STM32Cube3D for F3:
https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef3.html
PATH: STM32Cube_FW_F3_V1.11.0/Drivers/CMSIS/Device/ST/STM32F3xx/Source/Templates/gcc

I will build a simple Reset_handler function in C. For know you can use them, don't worry you
will have enough trouble with your code later ;).

## This looks really complicated. Do I need to know this ?
Assembly language is not mandatory to work on most Micro controller.
A startup function is easy to do in C when you understand what you have to do.

## What is important to understand (in my opininion) ?
You need to understand how we start a device:
- Entry function is Reset_Handler()
- First, this function load data/bss section to RAM (those with >RAM in linker script)
- Then this function call systeminit() function for clock system initialization
- Finally it call the main function

