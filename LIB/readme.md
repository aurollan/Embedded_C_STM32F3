# How to deal with library
You certainly want to use sime function to write code and you don't need to
 code eveything from scratch. This is were a library is useful but in embedded 
 development ressources are limited and we have to deal with specific 
 architecture.

Please note that the easiest way to deal with nano library is using GCC as a compilation driver and add "--specs=nano.specs" as argument. This file is in "/usr/lib/arm-none-eabi/newlib" and I have put a copy in this repository.
 
# Is there a good library for embedded development ?
 Yes there are. You will find a lot of library but here we only cover the
 Newlib-nano library.
 
## Why focus on Newlib-nano library ?

Newlib is  an embedded library maintained by Redhat developpers. 
Newlib-nano is a lightweight newlib library tweaked by ARM for embedded devices with very few ressources.
Both are bundle with the ARM GCC toolchain.
If you look in /usr/lib/arm-none-eabi/ you should find a newlib repository containing the appropriate library for your device.

# Understand how to link nano library
As the name suggest it you need to link the library at the linking process.

### Why at the linkng process?
When GCC compile your code from C to assembly and from assemby to object file,
it doesn't check if each symbol (function, global variable etc...) exist. It's just a translation from C to binary.

In our case we make a call to printf in the main.c file but no printf function is in our project files. But when we arrive to linking phase "ld" will llok for a function called printf and won't find it anywhere... unless we give it a library contained printf.

### How do I tell ld to use newlib-nano library.
Reference: https://sourceware.org/binutils/docs/ld/Options.html
At the linking phase we use two option to tell "ld" what we need:
	-L or --library-path= 
	-l or --library=

#### How to find the right pass ??!
We need to look our device configuration.
Fisrt of allm we use the thumb instruction set for our code:
=> /usr/lib/arm-none-eabi/newlib/thumb/

Then we need to know which architecture we use.
https://en.wikipedia.org/wiki/ARM_Cortex-M#Cortex-M4
On this link we find that the Cortex M4 has an ARMv7E-M architecture.
=> /usr/lib/arm-none-eabi/newlib/thumb/v7e-m/

Then we need to find how we manage floating point. This one is easy because we 
have already did this when creating the Makefile.
=> /usr/lib/arm-none-eabi/newlib/thumb/v7e-m/fpv4-sp/hard/

Here we are this is the most adapted library for our device.
I didn't try every Newlib-nano library I find maybe some other nano library works but we this one we don't have to worry about compatibility.

##### Add this line to your linker script
--library-path=/usr/lib/arm-none-eabi/newlib/thumb/v7e-m/fpv4-sp/hard/

#### How do i find the right name ??!
Again: https://sourceware.org/binutils/docs/ld/Options.html
We see that we just have to provide the name of the library.
The name is surround by "lib" and ".a".
Here we use libc_nano.a so we just have to provide the name "c_nano".
##### Add this line to your linker script
--library=c_nano

# I have done everything right but it doesn't work!
## This is expected
When you use those library they call some special function named system call.
Those function are managed by the kernel/OS which will execute them.
Our code request a service to the kernel/OS and wait it to execute and return.
In our case we don't have OS or kernel in our device so we have to implement system call we need.


int _write(int32_t file, char* ptr, int32_t len);
caddr_t _sbrk(int incr);

int		_isatty(int file);
int		_fstat(int file, struct stat *st);
int		_lseek(int file, int ptr, int dir);
int		_read(int file, char *ptr, int len);
int		_close(int fd);

## Do I have to rewrite all of them ?
If you NEED function that uses them: Yes.

But in our case we just want to make printf work and it need only 2 syscalls:
- write
- sbrk

Fortunatly newlib provide examples that implement the minimal functionality required to allow libc(_nano) to link, and fail gracefully where OS services are not available.
https://sourceware.org/newlib/libc.html#Syscalls

This is what we are looking for except for our 2 syscalls.

### write
We used write a lot of time to output data to our terminal from the begining.
We don't have anything else to do. Printf will just ouput the string with a call to write.

### sbrk
We can use the sbrk function almost as it. But this function use a variable named "stack_ptr" that is undeclared. So we have to declare this variable and retrive the value. Fortunatly ARM provide a function to retrive the stack pointer value and we use it.

#### It compiles but how it works ??!
sbrk is a simple memory allocation function. You ask for space and it give you a pointer followed by the size of memory you asked for.

##### Where is the heap memory ?
You can find a lot of ressources explaining memory layout, this is well documented.
Here is an example:
https://www.geeksforgeeks.org/memory-layout-of-c-program/

There is an heap pointer and a stack pointer.
You can see that the heap grows up as our heap_end is incremented.
It's easy to see that a problem occur if the heap or the stack keep growing.
They can both use the other memory space and read/write/execute it.
This is why in our sbrk we check if there is memory to allocate before returning anything.
There is a lot of way to handle such a situationm here the program get catched in an infinite loop. But keep in mind that is this situation occurm this is a big issue.

#### How do i know when heap starts ?
This is an information provided by the linked script. In our case there is an _end at the bottom of the linker script, this is the address just after the last data section variable has been copied.
#### How do i use linker script symbol ?
https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html

# Now you can use the library and printf but is it a good idea ?
Printf is a useful function for formating outputm but it comes with a cost: it takes a lot of memory.
Most of time we use simple output formating that we can code for only few byte of memory. If you need to shrink your code to size the flash memory of your devicem you shouldn't use printf.
You will find below some code comparison with size.

# Just adding printf to the project
## With Newlib-nano and printf and no sectiosn flags
Linker Optimization flag
	LO_FLAG = 
Compiler optimixation flag
	O_FLAGS = 
Compiler spec flag
	SP_FLAGS = --specs=nosys.specs

   text    data     bss     dec     hex filename
   8388     120    1552   10060    274c LIB-CM4

## With Newlib-nano and printf and section flags
Linker Optimization flag
	LO_FLAGS = --gc-sections
Compiler optimixation flag
	O_FLAGS = -ffunction-sections -fdata-sections
Compiler spec flag
	SP_FLAGS = --specs=nosys.specs

   text    data     bss     dec     hex filename
   7796     100    1552    9448    24e8 LIB-CM4

## Without printf and section flags
Linker Optimization flag
	LO_FLAGS =
Compiler optimixation flag
	O_FLAGS =

   text	   data	    bss	    dec	    hex	filename
   1764	     20	   1540	   3324	    cfc	LIB-CM4

## Without printf 
Linker Optimization flag
	LO_FLAGS = --gc-sections
Compiler optimixation flag
	O_FLAGS = -ffunction-sections -fdata-sections

   text    data     bss     dec     hex filename
   1176       0    1536    2712     a98 LIB-CM4
