# MAKEFILE FLAGS

Building an embedded executable require following files from CMSIS ARM library:
- header files:
	stm32f30x.h			=>	Contains all defined MACRO we will use
	stm32f30x_it.h		=>	Contains interrupt funcion declaration needed by the interruption table
	system_stm32f30x.h	=>	Contains the system init function declaration
	core_cm4.h			=>	Used by stm32f30x.h
	core_cmFunc.h		=>	Used by core_cm4.h
	core_cmInstr.h		=>	Used by core_cm4.h
	core_cmSimd.h		=>	Used by core_cm4.h
	
- sources files:
	system_stm32f30x.c	=>	Contains the syste; init function definition

- a linker script file (used during linking phase with ld):
	STM32F303VC_FLASH.ld

- a startup fonction file (not necessarily a file but usually)
	startup_stm32f30x.s	

There is a readme file for each part.

# PROJECT CONFIG
STM32F3 use an ARM cortex M4 (not M3 as the name suggest).

/!\ Warning
I use arm-gcc and arm-ld separately but using only GCC may avoid you some
trouble if you are not comfortable with compilation process.
If you need more information about the toolchain, please refer to
readme_toolchain.md. /!\

# GCC COMPILATION FLAGS

## ARM ARCH
Link to find all option below:
https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html

##### Why do i have to set all this options ?
It's called cross-compilation.
Because we will use arm-gcc toolchain to compile an executable for a specific target. 
We have to tell arm-gcc information so it can made an appropriate executable.

### -mthumb
Mixing ARM and THUMB Instruction Sets.
In most low-end ARM microcontrollers the 16-bit THUMB instruction set offers
both better code density and actually better performance when executed from
ROM, even though the 16-bit THUMB instruction set is less powerful than the
32-bit ARM instruction set. This allows gcc to use any combination of ARM
and THUMB instruction sets for optimal performance.

-mthumb / -marm
Select between generating code that executes in ARM and Thumb states. The
default for most configurations is to generate code that executes in ARM state,
but the default can be changed by configuring GCC with the --with-mode=state
configure option.
You can also override the ARM and Thumb mode for each function by using the
target("thumb") and target("arm") function attributes (see ARM Function
Attributes) or pragmas (see Function Specific Option Pragmas).

##### What is Thumb ?
Thumb is simply 16-bits instruction sets used by your Micro_processor to
execute your program.
##### Why do we use the thumb set instruction if there is an ARM ?
A good answer:
https://www.embedded.com/introduction-to-arm-thumb/

### -mcpu=cortex-m
This specifies the name of the target ARM processor. GCC uses this name to
derive the name of the target ARM architecture (as if specified by -march) and
the ARM processor type for which to tune for performance (as if specified by
-mtune). Where this option is used in conjunction with -march or -mtune, those
options take precedence over the appropriate part of this option.

##### How do i know Cortex reference ?
On every documentation concerning your microcontroller like:
https://www.st.com/content/ccc/resource/technical/document/user_manual/8a/56/97/63/8d/56/41/73/DM00063382.pdf/files/DM00063382.pdf/jcr:content/translations/en.DM00063382.pdf


### -mlittle-endian
Generate code for a processor running in little-endian mode. This is the
default for all standard configurations.

##### How do i know endianness ?
On your reference manual.
Page 51 - Memory organization - Introduction
https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf

##### What "little endian" mean ?
Simply a memory organisation policy when storing value. For more information:
https://en.wikipedia.org/wiki/Endianness

##### Why use "little endian" vs "big endian" ?
According to ARM you can use both.
https://developer.arm.com/docs/dui0553/a/the-cortex-m4-processor/memory-model/memory-endianness
And here is why we tend to use little-endian.
https://www.quora.com/Is-ARM-big-endian-or-little-endian


### -mthumb-interwork
https://github.com/gcc-mirror/gcc/blob/master/gcc/config/arm/README-interworking
Generate code that supports calling between the ARM and Thumb instruction sets.
Without this option, on pre-v5 architectures, the two instruction sets cannot
be reliably used inside one program. The default is -mno-thumb-interwork, since
slightly larger code is generated when -mthumb-interwork is specified. In AAPCS
configurations this option is meaningless.

##### Why use mthumb-interwork ?
From gcc 's github page:
https://github.com/gcc-mirror/gcc/blob/master/gcc/config/arm/README-interworking
Maybe useless for us because we don't use extern code....

## DEBUG
https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html

##### Why use debugging option ?
With this flags set, debugging symbol will be added to executable.
We will use them for step by step debugging with GDB when we encounter bugs or unexpected
behavior.

### -g or -g1/2/3
Debug option and level of debug symbol
Produce debugging information in the operating system’s native format (stabs,
COFF, XCOFF, or DWARF). GDB can work with this debugging information.
On most systems that use stabs format, -g enables use of extra debugging
information that only GDB can use; this extra information makes debugging work
better in GDB but probably makes other debuggers crash or refuse to read the
program. If you want to control for certain whether to generate the extra
information, use -gstabs+, -gstabs, -gxcoff+, -gxcoff, or -gvms (see below).

### ggdb
Produce debugging information for use by GDB. This means to use the most
expressive format available (DWARF, stabs, or the native format if neither of
those are supported), including GDB extensions if at all possible.

## FLOATING POINT
https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html

##### Why use floating point flags ?
We have to tell gcc how we will handle floating point operation.
There is a lot of way to handle floating point operation for Microcontroller
so we have to tell gcc how our microcontroller will handle this.
Maybe useless for know because we don't do floating point operation yet.

### -mfloat-abi=name
Specifies which floating-point ABI to use. Permissible values are: ‘soft’,
‘softfp’ and ‘hard’.

Specifying ‘soft’ causes GCC to generate output containing library calls for
floating-point operations. ‘softfp’ allows the generation of code using
hardware floating-point instructions, but still uses the soft-float calling
conventions. ‘hard’ allows generation of floating-point instructions and uses
FPU-specific calling conventions.

The default depends on the specific target configuration. Note that the
hard-float and soft-float ABIs are not link-compatible; you must compile your
entire program with the same ABI, and link with a compatible set of libraries.

##### How do you know the mfloat-abi value is 'hard' ?
'hard' or 'softfp' if you have an FPU, 'soft' if you don't.
'hard' use FPU-specific calling convention so you need to know if your device
has an FPU unit. You can find this information on the company website or in the
device manual:

https://www.st.com/en/microcontrollers-microprocessors/stm32f3-series.html

https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf

Or simply in the device Datasheet (See Documentation).

### -mfpu=name (-mfpu=fpv4-sp-d16)
This specifies what floating-point hardware (or hardware emulation) is
available on the target. Permissible names are: ‘auto’, ‘vfpv2’, ‘vfpv3’,
‘vfpv3-fp16’, ‘vfpv3-d16’, ‘vfpv3-d16-fp16’, ‘vfpv3xd’, ‘vfpv3xd-fp16’,
‘neon-vfpv3’, ‘neon-fp16’, ‘vfpv4’, ‘vfpv4-d16’, ‘fpv4-sp-d16’, ‘neon-vfpv4’,
‘fpv5-d16’, ‘fpv5-sp-d16’, ‘fp-armv8’, ‘neon-fp-armv8’ and
‘crypto-neon-fp-armv8’. Note that ‘neon’ is an alias for ‘neon-vfpv3’ and ‘vfp’
is an alias for ‘vfpv2’.

The setting ‘auto’ is the default and is special. It causes the compiler to
select the floating-point and Advanced SIMD instructions based on the settings
of -mcpu and -march.

If the selected floating-point hardware includes the NEON extension
(e.g. -mfpu=neon), note that floating-point operations are not generated by
GCC’s auto-vectorization pass unless -funsafe-math-optimizations is also
specified. This is because NEON hardware does not fully implement the IEEE 754
standard for floating-point arithmetic (in particular denormal values are
treated as zero), so the use of NEON instructions may lead to a loss of
precision.

You can also set the fpu name at function level by using the target("fpu=")
function attributes (see ARM Function Attributes) or pragmas (see Function
Specific Option Pragmas).

##### How do you know the mfpu value is 'fpv4-sp-d16' ?
Here is a post where it's well explained.
https://stackoverflow.com/questions/19464556/how-to-link-gcc-options-to-the-arm-mcu-fpu-datasheet

##### Floating-point is a mess ?
Check this link:
https://embeddedartistry.com/blog/2017/10/11/demystifying-arm-floating-point-compiler-options/

Or this one from ST:
https://www.st.com/content/ccc/resource/technical/document/user_manual/group1/cd/29/43/c5/36/c0/40/bb/Newlib_nano_readme/files/newlib-nano_readme.pdf/jcr:content/translations/en.newlib-nano_readme.pdf


## MACRO
https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
### DEF PREPROCESSOR MACRO
DEF: Custom macros (-DMACRO equals to #define MACRO)
-D name 
Predefine name as a macro, with definition 1.

##### Why define -STM32F303xC ?
In nost library header file use macro definition according to your hardware.
This define tells which one you use.
Another way to do this is uncomment define in header files.

##### Why define -USE_FULL_ASSERT ?
If you use STM32 library (Std or HAL), each function will check that passed
argument are actual register that the function use.
Since we use only CMSIS  library we don't need this yet. But still important to know because you will work with other libraries.

### UNDEF PREPROCESSOR MACRO
-U name
Cancel any previous definition of name, either built in or provided with
a -D option.
Unused here.


## OPTIMIZATION
https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
/!\ WARNING: enabling optimization can lead to unexpected behavior for
beginner. /!\

### -Os
Optimize for size. -Os enables all -O2 optimizations except those that often
increase code size:

-falign-functions  -falign-jumps
-falign-labels  -falign-loops
-fprefetch-loop-arrays  -freorder-blocks-algorithm=stc
It also enables -finline-functions, causes the compiler to tune for code size
rather than execution speed, and performs further optimizations designed to
reduce code size.

##### Why use this optimization ?
C is known partly because it runs fast and it creates ligtthweight executables.
A lot of optimization rely on the developper but the compiler does its jobs too
while processing your code. Fast and lightweight code is good,
especially for microcontroller with limited power and memory.
You chose what you want to optimize:
- Os: Optimize speed without increasing Size
- O 1/2/3: Optimize speed

### -ffunction-sections -fdata-sections
/!\
WARNING: use this flag with this LD FLAG:
- --gc-sections 
/!\

Place each function or data item into its own section in the output file if the
target supports arbitrary sections. The name of the function or the name of the
data item determines the section’s name in the output file.

Use these options on systems where the linker can perform optimizations to
improve locality of reference in the instruction space. Most systems using the
ELF object format have linkers with such optimizations. On AIX, the linker
rearranges sections (CSECTs) based on the call graph. The performance impact 
varies.

Together with a linker garbage collection (linker --gc-sections option) these
options may lead to smaller statically-linked executables (after stripping).

On ELF/DWARF systems these options do not degenerate the quality of the debug
information. There could be issues with other object files/debug info formats.

Only use these options when there are significant benefits from doing so. When
you specify these options, the assembler and linker create larger object and
executable files and are also slower. These options affect code generation.
They prevent optimizations by the compiler and assembler using relative
locations inside a translation unit since the locations are unknown until link
time. An example of such an optimization is relaxing calls to short call
instructions.

##### Why use this optimization ?
To reduce executable size if possible.
Most of the time you will use .c files given in libraries. Those file contains
a lot of function and we use only a few of them. In this case we can save space
with those flags to cut those functions from our executable.
These optimization are usually used for embedded development. But in our case
we will build our own function and use all of them so those optimization will
probably increase our executable size. But still you will see it everywhere so it's important to know.

You can see which functions are deleted by adding this flag to ld:
 -print-gc-sections

Here is a source with a great explenation:
https://lwn.net/Articles/741494/


## WARNING FLAGS
https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
##### Why use warning flags ?
Writing code is error prone, fortunatly some of the error checking is done by
compiler.
Enabling those strict flags help produce more secure code and take good habits.

### -Werror
Make all warnings into errors.
### -Wall
This enables all the warnings about constructions that some users consider
questionable, and that are easy to avoid (or modify to prevent the warning),
even in conjunction with macros. This also enables some language-specific
warnings described in C++ Dialect Options and Objective-C and Objective-C++
Dialect Options.
### -Wextra
This enables some extra warning flags that are not enabled by -Wall. (This
option used to be called -W. The older name is still supported, but the newer
name is more descriptive.)

## Nano-Library
##### Why use a nano library ?
Nano library offers function suited for embedded developement.

https://www.st.com/content/ccc/resource/technical/document/user_manual/group1/cd/29/43/c5/36/c0/40/bb/Newlib_nano_readme/files/newlib-nano_readme.pdf/jcr:content/translations/en.newlib-nano_readme.pdf

If you need to use nano library change the "--specs="flag in the makefile to
"--specs=nano.specs".

# LINKER

##### Why separate compiling steps and linking steps ?
For learning purpose. Linker is often a program we don't use directly and
having to use a linker script makes me understand its importance and also use 
it directly.

##### What is a gcc tool chain ?
https://stackoverflow.com/questions/50307733/what-is-a-gcc-toolchain

## SCRIPTS

https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
https://linux.die.net/man/1/ld

### --gc-sections | --no-gc-sections
Enable garbage collection of unused input sections. It is ignored on targets
that do not support this option. The default behaviour (of not performing this
garbage collection) can be restored by specifying --no-gc-sections on the
command line.

--gc-sections decides which input sections are used by examining symbols and
relocations. The section containing the entry symbol and all sections
containing symbols undefined on the command-line will be kept, as will sections
containing symbols referenced by dynamic objects. Note that when building shared
libraries, the linker must assume that any visible symbol is referenced. Once
this initial set of sections has been determined, the linker recursively marks
as used any section referenced by their relocations. See --entry and --undefined.

This option can be set when doing a partial link (enabled with option -r). In
this case the root of symbols kept must be explicitely specified either by an
--entry or --undefined option or by a "ENTRY" command in the linker script. 


##### Why use this optimization ?
/!\
WARNING: use this flag zith at least one of them:
-ffunction-sections
-fdata-sections
/!\
To reduce executable size if possible.
Most of the time you will use .c files given in libraries. Those file contains
a lot of function and we use only a few of them. In this case we can save space
with those flags to cut those functions from our executable.
These optimization are usually used for embedded development. But in our case
we will build our own function and use all of them so those optimization will
probably increase our executable size. But still you will see it everywhere so it's important to know.

You can see which functions are deleted by adding this flag to ld:
 -print-gc-sections

Here is a source with a great explenation:
https://lwn.net/Articles/741494/

##### Why use a linker script ?
To tell the ld executable the memory layout of the micro controller.
More on this on the readme_ld_script.md.

### -T script
Use script as the linker script. This option is supported by most systems using
the GNU linker. On some targets, such as bare-board targets without an
operating system, the -T option may be required when linking to avoid
references to undefined symbols.

### -L library
-llibrary
-l library
Search the library named library when linking. (The second alternative with the
library as a separate argument is only for POSIX compliance and is not
recommended.)
The -l option is passed directly to the linker by GCC. Refer to your linker
documentation for exact details. The general description below applies to the
GNU linker.
The linker searches a standard list of directories for the library. The
directories searched include several standard system directories plus any that
you specify with -L.
Static libraries are archives of object files, and have file names like
liblibrary.a. Some targets also support shared libraries, which typically have
names like liblibrary.so. If both static and shared libraries are found, the
linker gives preference to linking with the shared library unless the -static
option is used.
It makes a difference where in the command you write this option; the linker
searches and processes libraries and object files in the order they are
specified. Thus, ‘foo.o -lz bar.o’ searches library ‘z’ after file foo.o but
before bar.o. If bar.o refers to functions in ‘z’, those functions may not be
loaded.

## MAP FILE
##### Why do we need a map file ?
Human readable map of the executable use mostly for debug purposes.
It contains all information about memory mapping. We can find sectionsm variable...

### -Map=mapfile
Print a link map to the file mapfile. See the description of the -M option, above
-M
### --print-map
Print a link map to the standard output. A link map provides information about
the link, including the following:
- Where object files are mapped into memory.
- How common symbols are allocated.

- All archive members included in the link, with a mention of the symbol which
caused the archive member to be brought in.

- The values assigned to symbols.

Note - symbols whose values are computed by an expression which involves a
reference to a previous value of the same symbol may not have correct result
displayed in the link map. This is because the linker discards intermediate
results and only retains the final value of an expression. Under such
circumstances the linker will display the final value enclosed by square
brackets. Thus for example a linker script containing:
foo = 1
foo = foo * 4
foo = foo + 8
will produce the following output in the link map if the -M option is used:
 0x00000001                 foo = 0x1
[0x0000000c]                foo = (foo * 0x4)
[0x0000000c]                foo = (foo + 0x8)
See Expressions for more information about expressions in linker scripts.


## OPTIONS (IF YOU ONLY GCC FOR THE COMPILATION PROCESS)
### -Wl,option
### -Wl,-Map=$(NAME).map
### -Wl,--gc-sections
Pass option as an option to the linker. If option contains commas, it is split
into multiple options at the commas. You can use this syntax to pass an
argument to the option. For example, -Wl,-Map,output.map passes -Map output.map
to the linker. When using the GNU linker, you can also get the same effect
with -Wl,-Map=output.map.

# Library nano (--specs=nano.specs --specs=rdimon.specs --specs=nosys.specs)
SP_FLAGS = --specs=nosys.specs
