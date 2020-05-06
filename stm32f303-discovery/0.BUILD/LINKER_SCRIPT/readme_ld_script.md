# LD SCRIPT
This is an obscure part of embedded development and software development in
 general. \
First of all it's not mandatory to know how to build a linker script
but I think it is to understand it. \
I will go from general to detailed informations.

### ld ? Never heard about.
ld is the name of the linker executable. \
You find it on every GNU/Linux distribution usually we use it 
without knowing. \
It's a tool used (called) by the GCC toolchain to build your executable. \
But why we use it ?

#### Step to build an executable
Executable file is the translation of your program in binary code used by your 
microprocessor to read instruction.

Let's assume you use header `.h` and code file `.c` this way:
- You put `function declaration`, `constant`, `macro`... in header file
- You put `function definition` and `#include` in code file

Here is briefly how your program will go from your file to your executable.

1. The first step is the `preprocessor`. \
It replace all your preprocessor 
directive by their actual meaning: macro are replaced by value, included 
header files (\*.h) are copied into the code file asking for them (*.c) etc...

2. The second step is the `compiler`. \
It translate your C code to Assembly 
language. Each of your line of code (to keep it simple) is translated to one 
or more assembly instructions. Each hardware has his own instruction set. This 
is why we have to tell which hardware will executing our program. Note that 
the software handling the next part need to have instruction it can recognize 
to do its work.


3. The third step is the `assembler`. \
It translate each instruction to it's 
binary equivalent: . This is were we produce our `*.o` files. This is 
literally what the processor will read and execute.

Now we got each `*.c` files translated from human readable C, to human 
readable assembly  `*.s` to machine readable instruction to execute `*.o`.

4. The final step is the `linker`. \
It link together all the `*.o` file into 
your final executable that will be flash on your device. Ld use a layout to 
produce an executable suited to our device architecture.  This is why we need 
a linker script.

## Where do your file come from ?
Currently the linker script in each project come from:

STM32Cube3D for F3 available in manufacturer website: \
	
	https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef3.html

Folder path:

	STM32Cube_FW_F3_V1.11.0/Projects/

It comes from manufacturer website so we can rely on them. \
Note that most of the STM32Fx ld_script are the same expect for the MEMORY 
part which change according to your device FLASH and RAM memory available. \
For know you can use it until we build our own, don't worry you will have 
enough trouble with your code later ;). \

## This looks really complicated. Do I need to know this ?
It's seems complicated and you don't have to understand it right know if you
need/want to dive into code quickly. \
But I think it's important to understand at least some generality because it 
highlights some of the most important idea/notion in software. This ld script 
is heavily commented so you can undestand what it does with a carefull reading.

## What is important to understand (in my opininion) ?
### Entrypoint
The first information is the entrypoint with a value in `().` \
It simply tell the linker the first function to run, the first function read 
by your device when starting.

	Source:
	https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_node/ld_24.html

This is where your program REALLY start.

##### Why Reset_Handler and not main ?
Reset_Handler is the function called when reseting your device. \
This function will call your main after performing some code. \
Check `readme_startup.md` file for more information. \
Note that most of time main is NOT the first function called in your program. \
Even on your computer while building your executable GCC add some startup 
code is injected in your program. \
This is not reserved to embedded development.

### Memory
As said in `STM32F303VC_FLASH_STM32CUBE.ld`

	line 40 "Specify the memory areas".

When you buy your device you always see some information like:

	Datasheet
	page 1
	256‑Kbyte Flash memory and 48‑Kbyte RAM

This is were this memory is available from ORIGIN to ORIGIN + LENGTH.

To put it simply:
- FLASH MEMORY: Dead memory that keep value even the device is not suplied with
 power. Used to store all your code and value. Code is read from here but 
 values are copied to RAM at startup and read from RAM.

- RAM: volatile memory that lost value when the device is not supplied with 
power. This is where your variable will be read/write when your code run.
Constant, global ans static are copied to RAM at startup. And this is why we 
have to copy those variable when the device is starting.

- MEMORY_B1: label for external RAM memory (Useless for us now).

- CCMRAM: Core Coupled Memory is a RAM memory space where you can store your 
code to run it faster than in flash memory(Useless for us now). It takes 
advantage of the Havard architecture of our Cortex M4. Detailed information 
about CCMRAM in "I want to go further section".

Just remember FLASH and RAM, we will use only them.


##### How do I know my device memory layout ?
In the device reference manual (see DOC repository):

	Reference Manual
	page 51
	3.2.2 Memory map and register boundary addresses

Or in the device Datasheet (see DOC repository):

	Datasheet
	page 53
	5 Memory mapping

We just have to find SRAM and Main Flash memory. \
You should see that both adresses correspond to our linker file. \
Note that you can find another main flash memory but it's used in another 
configuration than the default one.

### How an executable is organized ?
An executable is organized as follow with more or less section according to 
your program:

	.text:
		Contains your code
	.rotdata:
		Constant data
	.data:
		Initialized global and static variable
	.bss:
		Uinitialized global and static variable (?)

The linker script will tell the linker executable how to organize each section
this is why we find all this information in the script.

To put it simply:
- We put all .text insection from our `*.o` files in one `text` section
- We put all .rotdata insection from our `*.o` files in one `rotdata` section
- We put all .data insection from our `*.o` files in one `data` section
- We put all .bss insection from our `*.o` files in one `bss` section

Then we mesure their size putting a `start` symbol before and an `end` symbol 
after. \
start - end = section_len

All symbol are variable without value.
And that's enought because we just need to get their addresses.

Key for reading the script:
- _s = start
- _e = end
- ALIGN(x) = insert padding bytes until current location becomes aligned on 
			 x-byte boundary.
- \>FLASH/RAM/etc... = Put those data in this memory declared earlier.
- . = last used address

#### Linker script paricularities
##### ARM to THUMB
 The .glue_7t and.glue_7 sections are synthesized by the compiler when you
 specify the ARM-THUMB interworking option. The sections contain the
 “call veneers” between THUMB and ARM code and are accessed frequently by every
 call between ARM and THUMB. It's typically advantageous to place this
 smallamount of hot-spot code in RAM.

##### C++ .preinit/init/finit array
You can use function pointer array to call those functions at startup.
There are used by C++ compiler or by newlib and libc library which provide the 
function you need to work with this feature.


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

