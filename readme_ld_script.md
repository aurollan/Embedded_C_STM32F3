# LD SCRIPT
This is an obscure part of embedded development.
First of all it's not mandatory to know how to build a linker script
but i think it is to understand it. I will go from general to detailed 
information.

##### ld ?
ld is the name of the linker executable.
After building object files, you have to link them together to create an 
executable file. This where ld is called.

## Where do your file come from ?
You can find them in Std_Library (deprecated but still easy to understand and
work with) available in manufacturer website:
https://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html
PATH: STM32F30x_DSP_StdPeriph_Lib_V1.2.3/Projects/STM32F30x_StdPeriph_Templates/TrueSTUDIO/STM32F303xC

Or you can find this file in STM32Cube3D for F3:
https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef3.html
PATH: STM32Cube_FW_F3_V1.11.0/Projects/STM32F3-Discovery/Demonstrations/SW4STM32/STM32F3-Discovery_Demo

## This looks really complicated. Do I need to know this ?
It's seems complicated and you don't have to understand it right know if you need/want to dive into code quicly.
