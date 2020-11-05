# LD SCRIPT
This is an obscure part of embedded development and software development in
 general. \
First of all it's not mandatory to know how to build a linker script
but I think it is to understand it. \
I will go from general to detailed informations.

### ld ? Never heard about.
ld is the name of the linker executable. \
You find it on every GNU/Linux distribution usually and we use it 
without even knowing it. \
It's a tool used (called) by the GCC toolchain to build your executable. \

### But why we use it ?
Because in embedded development you will find device with different 
architecture and memory space. You have to understand that your code will be 
place in memory according to your linker script. This is the final step of 
your program but let's find what is happening before linker step.

## Step to build an executable
Executable file is the translation of your program in binary code used by your 
microprocessor. Binary code is simply a list of intruction read and 
interpreted by your microcontroller. And the most human readable way we have 
to express this opcode is Assembly langage.

Let's assume you use header `.h` and code file `.c` this way:
- You put `function declaration`, `constant`, `macro`... in header file
- You put `function definition` and `#include` in code file

Here is briefly how your program will go from your file to your executable.

1. The first step is the `preprocessor`. \

	It replace all your preprocessor directive by their actual meaning: 
	macro are replaced by value, included header files (\*.h) are copied 
	into the code file asking for them (*.c), comments are removed etc...
	It symply replace your string by your other string and the output file 
	is the code compiled by gcc.\

2. The second step is the `compiler`. \

	It translate your C code (possibly pre modified by the preprocessor) to 
	Assembly language. Each of your line of code (to keep it simple) is 
	translated to one or more assembly instructions. Each hardware has his own 
	instruction set. This is why we have to tell which hardware will execute 
	our program.

Note that the software handling the next part need to have instruction it 
can recognize to do its work.


3. The third step is the `assembler`. \
	
	It translate each instruction to it's 
	binary equivalent: . This is were we produce our `*.o` files. This is 
	literally what the processor will read and execute.

Now we got each `*.c` files translated from human readable C to humain 
readable preprocessed C `*.i`, to human readable assembly  `*.s` to machine 
readable instruction to execute `*.o`.

4. The final step is the `linker`. \

	It link together all the `*.o` file into 
	your final executable that will be flash on your device. Ld need a layout 
	to produce an executable suited to our device architecture.  This is why 
	we need a linker script.

Note: ld is not a magical software, `*.o` are not file containing ONLY binary 
code, they contain informations to precess those file. If you want to see how 
information is structured into this file you can use `nm` or `readelf` tools 
proveded with your linux distribution.

## Where do your file come from ?
Currently the linker script in each project come from:

STM32Cube3D for F3 available in manufacturer website: \
	
	https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef3.html

Folder path:

	STM32Cube_FW_F3_V1.11.0/Projects/

It comes from manufacturer website so we can rely on them. \
Note that most of the STM32Fx ld_script fir ARM Cortex M are the same expect 
for the MEMORY part which change according to your device FLASH and RAM memory available. \
For know you can use it until we build our own, don't worry you will have 
enough trouble with your code later ;). \

Note that another linker script is extracted from stm32f3 standart Library.

	https://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html

Folder path:

	/STM32F30x_DSP_StdPeriph_Lib_V1.2.3/Projects/STM32F30x_StdPeriph_Templates/TrueSTUDIO/STM32F303xC/STM32F303VC_FLASH.ld
	

## This looks really complicated. Do I need to know this ?
It's seems complicated and you don't have to understand it right know if you
need/want to dive into code quickly. \
But I think it's important to understand at least some generality because it 
highlights some of the most important idea/notion in software. Both ld script 
are heavily commented so you can undestand what it does with a carefull 
reading.

If you need an example you can check readme.md file in `02.LINKER_SCRIPT/OWN/readme.md`

