# MAKEFILE FLAGS

# GCC COMPILER
## MACRO
https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
### DEF PREPROCESSOR MACRO
DEF: Custom macros (-DMACRO equals to #define MACRO)
-D name
Predefine name as a macro, with definition 1.

### UNDEF PREPROCESSOR MACRO
-U name
Cancel any previous definition of name, either built in or provided with a -D option.

## DEBUG
https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
### -g -g1/2/3
Debug option and level of debug symbol
Produce debugging information in the operating system’s native format (stabs, COFF, XCOFF, or DWARF). GDB can work with this debugging information.
On most systems that use stabs format, -g enables use of extra debugging information that only GDB can use; this extra information makes debugging work better in GDB but probably makes other debuggers crash or refuse to read the program. If you want to control for certain whether to generate the extra information, use -gstabs+, -gstabs, -gxcoff+, -gxcoff, or -gvms (see below).

### ggdb
Produce debugging information for use by GDB. This means to use the most expressive format available (DWARF, stabs, or the native format if neither of those are supported), including GDB extensions if at all possible.

## OPTIMIZATION
https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
### -Os
Optimize for size. -Os enables all -O2 optimizations except those that often increase code size:

-falign-functions  -falign-jumps
-falign-labels  -falign-loops
-fprefetch-loop-arrays  -freorder-blocks-algorithm=stc
It also enables -finline-functions, causes the compiler to tune for code size rather than execution speed, and performs further optimizations designed to reduce code size.

### -ffunction-sections (gcc), -fdata-sections (gcc) and --gc-sections (ld)
Place each function or data item into its own section in the output file if the target supports arbitrary sections. The name of the function or the name of the data item determines the section’s name in the output file.

Use these options on systems where the linker can perform optimizations to improve locality of reference in the instruction space. Most systems using the ELF object format have linkers with such optimizations. On AIX, the linker rearranges sections (CSECTs) based on the call graph. The performance impact varies.

Together with a linker garbage collection (linker --gc-sections option) these options may lead to smaller statically-linked executables (after stripping).

On ELF/DWARF systems these options do not degenerate the quality of the debug information. There could be issues with other object files/debug info formats.

Only use these options when there are significant benefits from doing so. When you specify these options, the assembler and linker create larger object and executable files and are also slower. These options affect code generation. They prevent optimizations by the compiler and assembler using relative locations inside a translation unit since the locations are unknown until link time. An example of such an optimization is relaxing calls to short call instructions.

## WARNING FLAGS
https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
### -Werror
Make all warnings into errors.
### -Wall
This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros. This also enables some language-specific warnings described in C++ Dialect Options and Objective-C and Objective-C++ Dialect Options.
### -Wextra
This enables some extra warning flags that are not enabled by -Wall. (This option used to be called -W. The older name is still supported, but the newer name is more descriptive.)

## ARM FLOATING POINT
https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
### -mfloat-abi=name (-mfloat-abi=softfp)
Specifies which floating-point ABI to use. Permissible values are: ‘soft’, ‘softfp’ and ‘hard’.
Specifying ‘soft’ causes GCC to generate output containing library calls for floating-point operations. ‘softfp’ allows the generation of code using hardware floating-point instructions, but still uses the soft-float calling conventions. ‘hard’ allows generation of floating-point instructions and uses FPU-specific calling conventions.
The default depends on the specific target configuration. Note that the hard-float and soft-float ABIs are not link-compatible; you must compile your entire program with the same ABI, and link with a compatible set of libraries.
### -mfpu=name (-mfpu=fpv4-sp-d16)
This specifies what floating-point hardware (or hardware emulation) is available on the target. Permissible names are: ‘auto’, ‘vfpv2’, ‘vfpv3’, ‘vfpv3-fp16’, ‘vfpv3-d16’, ‘vfpv3-d16-fp16’, ‘vfpv3xd’, ‘vfpv3xd-fp16’, ‘neon-vfpv3’, ‘neon-fp16’, ‘vfpv4’, ‘vfpv4-d16’, ‘fpv4-sp-d16’, ‘neon-vfpv4’, ‘fpv5-d16’, ‘fpv5-sp-d16’, ‘fp-armv8’, ‘neon-fp-armv8’ and ‘crypto-neon-fp-armv8’. Note that ‘neon’ is an alias for ‘neon-vfpv3’ and ‘vfp’ is an alias for ‘vfpv2’.

The setting ‘auto’ is the default and is special. It causes the compiler to select the floating-point and Advanced SIMD instructions based on the settings of -mcpu and -march.

If the selected floating-point hardware includes the NEON extension (e.g. -mfpu=neon), note that floating-point operations are not generated by GCC’s auto-vectorization pass unless -funsafe-math-optimizations is also specified. This is because NEON hardware does not fully implement the IEEE 754 standard for floating-point arithmetic (in particular denormal values are treated as zero), so the use of NEON instructions may lead to a loss of precision.

You can also set the fpu name at function level by using the target("fpu=") function attributes (see ARM Function Attributes) or pragmas (see Function Specific Option Pragmas).

## ARM ARCH
https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
### -mthumb
-mthumb / -marm
Select between generating code that executes in ARM and Thumb states. The default for most configurations is to generate code that executes in ARM state, but the default can be changed by configuring GCC with the --with-mode=state configure option.

You can also override the ARM and Thumb mode for each function by using the target("thumb") and target("arm") function attributes (see ARM Function Attributes) or pragmas (see Function Specific Option Pragmas).

### -mcpu=cortex-m
This specifies the name of the target ARM processor. GCC uses this name to derive the name of the target ARM architecture (as if specified by -march) and the ARM processor type for which to tune for performance (as if specified by -mtune). Where this option is used in conjunction with -march or -mtune, those options take precedence over the appropriate part of this option.

### -mthumb-interwork
Generate code that supports calling between the ARM and Thumb instruction sets. Without this option, on pre-v5 architectures, the two instruction sets cannot be reliably used inside one program. The default is -mno-thumb-interwork, since slightly larger code is generated when -mthumb-interwork is specified. In AAPCS configurations this option is meaningless.

### -mlittle-endian
Generate code for a processor running in little-endian mode. This is the default for all standard configurations.

# LINKER

## SCRIPTS
https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
### -T script
Use script as the linker script. This option is supported by most systems using the GNU linker. On some targets, such as bare-board targets without an operating system, the -T option may be required when linking to avoid references to undefined symbols.

### -L library
-llibrary
-l library
Search the library named library when linking. (The second alternative with the library as a separate argument is only for POSIX compliance and is not recommended.)
The -l option is passed directly to the linker by GCC. Refer to your linker documentation for exact details. The general description below applies to the GNU linker.
The linker searches a standard list of directories for the library. The directories searched include several standard system directories plus any that you specify with -L.
Static libraries are archives of object files, and have file names like liblibrary.a. Some targets also support shared libraries, which typically have names like liblibrary.so. If both static and shared libraries are found, the linker gives preference to linking with the shared library unless the -static option is used.
It makes a difference where in the command you write this option; the linker searches and processes libraries and object files in the order they are specified. Thus, ‘foo.o -lz bar.o’ searches library ‘z’ after file foo.o but before bar.o. If bar.o refers to functions in ‘z’, those functions may not be loaded.

## OPTIONS
### -Wl,option
### -Wl,-Map=$(NAME).map
### -Wl,--gc-sections
Pass option as an option to the linker. If option contains commas, it is split into multiple options at the commas. You can use this syntax to pass an argument to the option. For example, -Wl,-Map,output.map passes -Map output.map to the linker. When using the GNU linker, you can also get the same effect with -Wl,-Map=output.map.
