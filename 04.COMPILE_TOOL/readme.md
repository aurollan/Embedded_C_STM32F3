# Compiling toolchain
## What you can use
As you certianly know, developing software for embedded device require a 
specific toolchain. And most of the time these tools are packaged in an IDE:

	IAR: https://www.iar.com/iar-embedded-workbench/
	Keil (ARM): https://www.keil.com/
	Atollic (ST): https://atollic.com/truestudio/
	MPLAB (Microchip): https://www.microchip.com/mplab/mplab-x-ide

Those IDE offer great environment to code and build project. But they hide too 
much information for the purpose of this tutorial. And I don't want this 
tutorial to be plateform dependent.

## What we use
So is this possible to develop embedded application without using those IDE ?
Yes it is.
If you are familiar with the GNU compilation toolchain you will feel like home.

We will use the ARM-GCC toolchain which is simply (quoting the documentation):
	
	Bare metal C and C++ compiler for embedded ARM chips using Cortex-M, 
	and Cortex-R processors. This package is based on the GNU ARM toolchain 
	provided by ARM. 
	https://packages.debian.org/buster/gcc-arm-none-eabi

And this is perfectly fine for us because we are going to use Cortex-M4 
microcontroller.

Install it using

	sudo              : execute as root
	-y                : accept memory space used by download
	gcc-arm-none-eabi : package name

	sudo apt-get install -y gcc-arm-none-eabi

Now you can compile your project like any C project using `arm-none-eabi-gcc` 
as your compiler and use the text editor or IDE you want.
But we have done only one step. We can compile our project but how can we flash
 it on our device ? This is the next step.

## arm-none-eabi
 The toolchain used to build our executable. 

### Why arm-none-eabi-gcc ?
- arm: target architecture
- none: no vendor
- No OS is specified (bare metal programming)
- eabi: complies embedded-application binary interface
- gcc: call the compiler in this case

##### Why eabi is specified ?
Because we target an Embedded abi we want to use this specific optimized binary
interface for our project.\
Reference: https://en.wikipedia.org/wiki/Application_binary_interface

##### How is the command line build ?
The name is build with a target triplet convention not only used in embedded 
development.\
Reference: https://wiki.osdev.org/Target_Triplet
