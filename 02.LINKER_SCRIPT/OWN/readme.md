DISCLAIMER: This linker script is an untested example, but a good start if you 
want to build your own linker script.

# Build your own linker script
Here we want to build our own linker script to use in our project.
For now I have just cherry picked the most important parts of the official 
linker script and I'm going to explain how it works.
For parts I don't cover here you can find them in 

	02.LINKER_SCRIPT/STM32_LD/readme.md



## What is important to understand (in my opininion) ?
### Entrypoint
The first information is the entrypoint with a value in `().` \
It simply tell the linker the first function to run, the first function read 
by your device when starting.

This is where your program REALLY start.

##### Why Reset_Handler and not main ?
`Reset_Handler` is the function called when reseting your device. \
This function is defined in the startup file. It does some work before calling 
the main function but this part is covered in `03.STARTUP_FILE/`.
development there is a LOT of function called before your main.
Even on your computer while building your executable GCC, some startup 
code is injected in your program. \
This is not reserved to embedded development.

##### Entrypoint Source
	
	https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_node/ld_16.html

### Memory
As said in `STM32F303VC_FLASH_STM32CUBE.ld`

	line 40
	"Specify the memory areas".

This is were this memory is available from ORIGIN to ORIGIN + LENGTH.
When you buy your device you always see some information like:

	Datasheet
	page 1
	256‑Kbyte Flash memory and 48‑Kbyte RAM

To put it simply:
- FLASH MEMORY: Dead memory that keep value even when the device is not 
suplied with power. Used to store all your code and values.
Code and constant values are read from there but runtime values are copied to 
RAM at startup and read from RAM.

- RAM: volatile memory that lost all values when the device is not supplied 
with power. This is where your variable will be read/write when your code run.
Global and static are copied to RAM at startup. And this is why we have to 
copy those variable when the device is starting.

- MEMORY_B1: label for external RAM memory (Useless for us now).

- CCMRAM: Core Coupled Memory is a RAM memory space where you can store your 
code to run it faster than in flash memory(Useless for us now). It takes 
advantage of the Havard architecture of our Cortex M4. Detailed information 
about CCMRAM in "I want to go further section".

Just remember FLASH and RAM, we will use only them.

##### How do I know my device memory layout ?
You can find the ORIGIN addresses in 

	Reference Manual
	page 53
	Table 2. STM32F303xB/C and STM32F358xC peripheral register boundary 
	addresses


Or in the device Datasheet

	Datasheet
	page 53
	5 Memory mapping


We just have to find SRAM and Main Flash memory. \
You should see that both adresses correspond to our linker file. \

Note: You may find 2 flash memory region in memory map, there is a good reason 
for this. I will explain this in a specific part.

##### What are those character in parenthese ?
They are also attribute for the linker. But for this the following source is 
clear enough.

##### Memory source

	https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_node/ld_16.html

### How an executable is organized ?
An executable is organized with more or less section according to 
your program:

	.text:
		code
	.rotdata:
		Constant data
	.data:
		Initialized global and static variable
	.bss:
		Uinitialized global and static variable (?)

The linker script will tell the linker executable how to organize each section
this is why we find all this information in the script.

To put it simply:
- We put all .text section from our `*.o` files in one `text` section
- We put all .rotdata section from our `*.o` files in one `rotdata` section
- We put all .data section from our `*.o` files in one `data` section
- We put all .bss section from our `*.o` files in one `bss` section

##### You missed isr_vector
Yes I wanted to keep it simple in the previous part.
isr_vector is an array containg pointer to all function required by hardware.
It is mandatory if you want to use hardware related function like 
interruption and exception handlers.

##### Undertanding ld language
`ld` create symbol (addresses) that our code can use to know sections size and 
addresses.
All symbol are variable without value.

We can mesure their size putting a `start` symbol before and an `end` symbol 
after:
start_section - end_section = len_section

Or simply take he section symbol as the start symbol:
section - end_section = len_section

This is why you find only "end" symbols in the linker script.

And that's enought because we just need to get their addresses.

Key for reading the script:
- `_s*` = start address of section
- `_e*` = end address of section
- `ALIGN(x)` = insert padding bytes until current location becomes aligned on 
  x-byte boundary. For stm32 we are aligned on 32 bits => 4 bytes.
- `.` = last used address
- `\>FLASH/RAM/etc...` = Put those data in this memory declared earlier.

- LOADADDR(section) Return the absolute load address of the named section. This is normally the same as ADDR, but it may be different if the AT keyword is used in the section definition (see section Optional Section Attributes).

	Source for instruction
	https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html


##### You missed `AT` attribute
To put it simply [RAM]`AT`[FLASH] means:
Load it to [FLASH] address but will be copied to [RAM] address at startup.

A nicely explained answer with sources:

	https://stackoverflow.com/questions/28809372/what-does-region1-at-region2-mean-in-an-ld-linker-script

And direct sources:

	https://sourceware.org/binutils/docs/ld/Output-Section-LMA.html#Output-Section-LMA

### Heap and Stack
This section is special, no code is loaded here.
This is just linker code to check at the end if there is enought stack or heap.
You can chose the value you want, they are defined in the beginning of the 
file.
I choosed to keep it even if it's not mandatory because it can be very useless 
when you hit hardware RAM limitation.

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


