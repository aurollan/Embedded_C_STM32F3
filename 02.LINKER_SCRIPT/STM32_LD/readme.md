# ARM specific in linker script attributes
In `02.LINKER_SCRIPT/OWN/readme.md` we have cover all linker script 
generalities.
Now we are going to focus on specific part related to 
- our ARM cortex microcontroller
- some language feature implementation

#### Linker script paricularities
##### ARM to THUMB and THUMB to ARM
ARM and THUMB are just 2 different instruction set that your microcontroller 
can understand.
They are assembly instruction coded on 16 bits (Thumb) or 32 bits (ARM).
Thumb instruction are smaller so your can generate a smaller size executable.
ARM set are wider so you have much more differents instruction and one 
instruction can do more than a thumb intruction.

Of course this is not as simple as I putted it here but yu can get a good 
understanding of why we could use 2 different instructions sets.

Now we have microcontroller using both and switching between 
them. We can use `-mthumb-interwork` compiler option to generate code 
handling switch between both instruction sets.
This is why we "need" those sections. As said in 

	https://gcc-help.gcc.gnu.narkive.com/N7q1WzLT/glue-7-and-glue-7t-sections

And 

 	https://www.embedded.com/building-bare-metal-arm-systems-with-gnu-part-3/

	The .glue_7t and.glue_7 sections contain the “call veneers” between THUMB and 
	ARM code and are accessed frequently by every call between ARM and THUMB. We 
	use code containe in this section to "glue" ARM and Thumb instruction set 
	switch.

And you can understand how this is implemented in this documentation from ARM

	https://developer.arm.com/documentation/dui0204/j/arm-and-thumb-instructions/branch-and-control-instructions/b--bl--bx--blx--and-bxj

##### eh_frame
This is a particular section used for DWARF-based stack unwiding features.
Unwinding stack is usefull for debug purpose and exception handling.
In C the `backtrace` function use it to unwind stack for example.
In C++ runtime exception handling use this section in the same way.

	https://stackoverflow.com/questions/26300819/why-gcc-compiled-c-program-needs-eh-frame-section

Note that in embedded device we use ressources in the most efficiency way, 
this is "why" we don't have a frame pointer traditionally used for unwinding 
features. In order to support this feature we store stack unwinding 
informations in flash data. Those informations are compressed and located in 
`eh_frame`.

Easy link to understand

	https://stackoverflow.com/questions/63146854/how-does-glibc-backtrace-determine-which-stack-memory-are-the-return-addresses

More detailled informations

	https://fzn.fr/projects/frdwarf/frdwarf-oopsla19.pdf

##### C++ .preinit/init/finit array
Library provide some generic function called before and after main.
You can use function pointer array to call those functions at startup.
There are used by C++ compiler or by newlib and libc library which provide the 
function you need to work with this feature.
More informations here

	https://stackoverflow.com/questions/15265295/understanding-the-libc-init-array

##### arm ex table
`.ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH`
This is a section dedicated to plateform/language with exception handling 
management.
We code in C language with "limited out of the box exception handling 
possibilities". We won't use it.
But if you code a program with C++ and exception handling functionnality 
you will have some code in this section.

If you want more detailled information:

	https://developer.arm.com/documentation/ihi0038/c/?lang=en#design-principles

Note: a good exercice would be to test a project in C++ with exception 
handling and see executable's sections to look for the `.ARM.exidx`. Of course 
you will have to understand precisly how exception handling is implemented in 
C++.
