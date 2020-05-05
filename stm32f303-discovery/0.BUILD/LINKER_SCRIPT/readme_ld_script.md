# LD SCRIPT
This is an obscure part of embedded development and software develoment in
 general. First of all it's not mandatory to know how to build a linker script
but I think it is to understand it. I will go from general to detailed 
informations.

### ld ?
ld is the name of the linker executable.
After building object files from assembly (and assembly from C in our case),
you have to link them together to create an executable file. This where ld is
called. Ld will search all reference from your code (function, global etc...) 
and assemble them in specific executable section.
Ld use a layout to produce an executable suited to our device architecture.
This is why we need a linker script.

## Where do your file come from ?
Currently linker script in each project come from:

Std_Library (deprecated but still easy to understand and work with) available
in manufacturer website:
https://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html
PATH: STM32F30x_DSP_StdPeriph_Lib_V1.2.3/Projects/STM32F30x_StdPeriph_Templates/TrueSTUDIO/STM32F303xC

STM32Cube3D for F3 available in manufacturer website too:
https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef3.html
PATH: STM32Cube_FW_F3_V1.11.0/Projects/STM32F3-Discovery/Demonstrations/SW4STM32/STM32F3-Discovery_Demo

They both come from manufacturer website so we can rely on them.
For know you can use them, don't worry you will have enough trouble with your
code later ;).
The last linker script releqse come from the STM32 cube Mx package.
I will build a simpler script later but for now we will keep one of them.

## This looks really complicated. Do I need to know this ?
It's seems complicated and you don't have to understand it right know if you
need/want to dive into code quickly. But i think it's important to understand
at least some generality because it highlights some of the most important
idea/notion in software. This ld script is heavily commented so you can 
undestand what it does with a carefull reading.

## What is important to understand (in my opininion) ?
### Entrypoint
The first information is the entrypoint with a value in ().
It simply tell the linker the first function of the output file.
https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_node/ld_24.html
Here is where the program you will flash on your device later start.

##### Why Reset_Handler and not main ?
Reset_Handler is the function called when reseting your device.
This function will call your main after performing some code.
Check readme_startup.md file for more information.
Note that most of time main is NOT the first function called in your program.
While building your executable GCC add some startup code is injected in your
 program. This is not reserved to embedded development.

### Memory
As said in "STM32F303VC_FLASH_Std.ld" line 40 "Specify the memory areas".
When you buy your device you always see some information like:
`STM32F303VCT6 microcontroller featuring 256‑Kbyte Flash memory and 48‑Kbyte RAM`
This is were this memory is available from ORIGIN to ORIGIN + LENGTH.

To put it simply:
- FLASH MEMORY: Dead memory that keep value even the device is not suplied with
 power. Used to store all your code and value. Code is read from here but 
 values are copied to RAM at startup.

- RAM: volatile memory that lost value when the device is not supplied with 
power. This is where your variable will be read/write when your code run.
Constant, global ans static are copied to RAM at startup.

- MEMORY_B1: label for external RAM memory (Useless for us now).

- CCMRAM: Core Coupled Memory is a RAM memory space where you can store your 
code to run it faster than in flash memory(Useless for us now). It takes 
advantage of the Havard architecture of our Cortex M4. Detailed information 
about CCMRAM in "I want to go further section".

Just remember FLASH and RAM, we will use only them.


##### How do i know my device memory layout ?
In the device reference manual (see Documentation repository):
https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf

Or in the device Datasheet:
www.st.com/resource/en/datasheet/stm32f303vc.pdf

For example we are looking for a memory map (layout) and we found it page 51 in
the refence manual or page 53 in the datasheet.
We just have to find SRAM and Main Flash memory. You should see that both
adresses correspond to our linker file.

### Executable organization
An executable is organized as follow with more or less section according to 
your program:
.text:
	Contains your code
.rotdata:
	Constant data
.data:
	Initialized global and static variable
.bss:
	Uinitialized global and static variable

The linker script will tell the linker executable how to organize each section
this is why we find all this information in the script.

 The .glue_7t and.glue_7 sections are synthesized by the compiler when you
 specify the ARM-THUMB interworking option. The sections contain the
 “call veneers” between THUMB and ARM code and are accessed frequently by every
 call between ARM and THUMB. It's typically advantageous to place this
 smallamount of hot-spot code in RAM.

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

The DIY way:
https://jacobmossberg.se/posts/2018/08/11/run-c-program-bare-metal-on-arm-cortex-m3.html

More information on ALIGN():
https://stackoverflow.com/questions/9827157/what-does-keep-mean-in-a-linker-script

Official information about KEEP:
https://sourceware.org/binutils/docs/ld/Input-Section-Keep.html#Input-Section-Keep

Some redondant information, but more precise than me on some point:
http://developers.stf12.net/cpp-demo/gcc-linker-script-and-stm32-a-tutorial

Use CCM SRAM on STM32:
https://www.st.com/content/ccc/resource/technical/document/application_note/bb/09/ca/83/14/e9/44/c5/DM00083249.pdf/files/DM00083249.pdf/jcr:content/translations/en.DM00083249.pdf


Use lnker script symbol:
https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html

RFRM page 64 - 4.2.1 Flash memory organization
The Flash memory is organized as 64-bit wide memory cells that can be used for storing
both code and data constants.
