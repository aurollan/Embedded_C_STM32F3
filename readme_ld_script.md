# LD SCRIPT
This is an obscure part of embedded development.
First of all it's not mandatory to know how to build a linker script
but i think it is to understand it. I will go from general to detailed 
information.

### ld ?
ld is the name of the linker executable.
After building object files, you have to link them together to create an 
executable file. This where ld is called.

## Where do your file come from ?
Currently linker script in each project come from:

Std_Library (deprecated but still easy to understand and
work with) available in manufacturer website:
https://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html
PATH: STM32F30x_DSP_StdPeriph_Lib_V1.2.3/Projects/STM32F30x_StdPeriph_Templates/TrueSTUDIO/STM32F303xC

STM32Cube3D for F3:
https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef3.html
PATH: STM32Cube_FW_F3_V1.11.0/Projects/STM32F3-Discovery/Demonstrations/SW4STM32/STM32F3-Discovery_Demo

I will build a simple script later. For know you can use them, don't worry you
will have enough trouble with your code later ;).

## This looks really complicated. Do I need to know this ?
It's seems complicated and you don't have to understand it right know if you need/want to dive into code quickly.
But i think it's important to understand at least some generality.

## What is important to understand (in my opininion) ?

### Entrypoint
The first information is the entrypoint with a value in ().
It simply tell the linker the first executable instruction of the output file.
https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_node/ld_24.html

##### Why Reset_Handler and not main ?
Reset_Handler is the function called when reseting your device.
This function will call your main after performing some code. Check readme_startup.md for more information.

### Memory
As said in "STM32F303VC_FLASH_Std.ld" line 40 "Specify the memory areas".
When you buy your device you always see some information like:
`STM32F303VCT6 microcontroller featuring 256‑Kbyte Flash memory and 48‑Kbyte RAM`
This is were this memory is available from ORIGIN to ORIGIN + LENGTH.

##### How do i know my device memory layout ?
In the device reference manual:
https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf

For example we are looking for a memory map (layout) and we found it page 51.
We just have to find SRAM and Main Flash memory.

### Data organization
When building an executable the linker organize your code in section:
.text:
	Contains your code
.rotdata:
	Constant data
.data:
	Initialized global and static variable
.bss:
	Uinitialized global and static variable

The linker script will tell linker how to organize each section this is why
we find all this information in the script.

Key for reading:
- _s = start
- _e = end
- ALIGN(x) = insert padding bytes until current location becomes aligned on 
			 x-byte boundary.
- >FLASH/RAM/etc... = Put those data in this memory declared earlier.
- . = last used address

## I want to go further!
More explanation about linking process:
https://www.bravegnu.org/gnu-eprog/linker.html

What you were expecting to read:
https://hwengineer.github.io/linker/

A well explained tutorial:
https://embedds.com/programming-stm32-discovery-using-gnu-tools-linker-script/

More information on ALIGN():
https://stackoverflow.com/questions/9827157/what-does-keep-mean-in-a-linker-script

Official information about KEEP:
https://sourceware.org/binutils/docs/ld/Input-Section-Keep.html#Input-Section-Keep

Some redondant information, but more precise than me on some point:
http://developers.stf12.net/cpp-demo/gcc-linker-script-and-stm32-a-tutorial
