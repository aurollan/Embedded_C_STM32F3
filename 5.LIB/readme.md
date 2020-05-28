# How to deal with library
You certainly want to use sime function to write code and you don't need to
 code eveything from scratch.\
 This is were a library is useful but in embedded 
 development ressources are limited and we have to deal with specific 
 architecture.

Please note that the easiest way to deal with nano library is using GCC as a 
compilation driver and add "--specs=nano.specs" as argument.\
This file is in "/usr/lib/arm-none-eabi/newlib/your_device_architecture" and I 
have put a copy in this repository.
 
# Is there a good library for embedded development ?
 Yes there are. You will find a lot of library but here we only cover the
 Newlib-nano library.
 
## Why focus on Newlib-nano library ?

Newlib is  an embedded library maintained by Redhat developpers. 
Newlib-nano is a lightweight newlib library tweaked by ARM for embedded 
devices with very few ressources.
Both are bundle with the ARM GCC toolchain.
If you look in /usr/lib/arm-none-eabi/ you should find a newlib repository 
containing the appropriate library for your device.

# Understand how to link nano library
As the name suggest it you need to link the library at the linking step.

### Why at the linkng process?
When GCC compile your code from C to assembly and from assemby to object file,
it doesn't check if each symbol (function, global variable etc...) exist. It's 
just a translation from C to binary.

In our case we make a call to printf in the main.c file but no printf function 
is in our project files. But when we arrive to linking phase "ld" will llok 
for a function called printf and won't find it anywhere... unless we give it a 
library contained printf.

### How do I tell ld to use newlib-nano library.
At the linking phase we use two option to tell "ld" what we need:

	-L or --library-path= 
	-l or --library=

Reference: https://sourceware.org/binutils/docs/ld/Options.html \

#### How to find the right pass ??!
We need to look our device configuration.\
Fisrt of all we use the thumb instruction set for our code (see Makefile 
readme.md):

	/usr/lib/arm-none-eabi/newlib/thumb/

Then we need to know which architecture we use.

	https://en.wikipedia.org/wiki/ARM_Cortex-M#Cortex-M4

On this link we find that the Cortex M4 has an ARMv7E-M architecture.

	/usr/lib/arm-none-eabi/newlib/thumb/v7e-m/

Then we need to find how we manage floating point. This one is easy because we 
have already did this when creating the Makefile(see Makefile readme.md)

	/usr/lib/arm-none-eabi/newlib/thumb/v7e-m/fpv4-sp/hard/

Here we are this is the most adapted library for our device.
I didn't try every Newlib-nano library I find, maybe some other nano library 
works with our device but we this one we don't have to worry 
about compatibility issues.

##### Add this line to your linker script

	--library-path=/usr/lib/arm-none-eabi/newlib/thumb/v7e-m/fpv4-sp/hard/

#### How do i find the right name ??!
Again: https://sourceware.org/binutils/docs/ld/Options.html \

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
In our case we don't have OS or kernel in our device so we have to implement 
system call we need.


Mandatory for printf:

	int _write(int32_t file, char* ptr, int32_t len);
	caddr_t _sbrk(int incr);

Optionnal unless you use that function that call them:

	int		_isatty(int file);
	int		_fstat(int file, struct stat *st);
	int		_lseek(int file, int ptr, int dir);
	int		_read(int file, char *ptr, int len);
	int		_close(int fd);

## Do I have to rewrite all of them ?
If you call function that NEED them: Yes.

But in our case we just want to make printf work and it need only 2 syscalls:

	- write
	- sbrk

Fortunatly newlib provide examples that implement the minimal functionality 
required to allow libc(_nano) to link, and fail gracefully where OS services 
are not available.\

Reference: https://sourceware.org/newlib/libc.html#Syscalls

This is what we are looking for our optionnal system calls but not for our 2 
mandatory systemcall. 

### write
We used write a lot of time to output data to our terminal from the begining.
We don't have anything else to do. Printf will just ouput the string with a 
call to write.

### sbrk
We can use the sbrk function almost as it. But this function use a variable 
named "stack_ptr" that is undeclared. So we have to declare this variable and 
retrive the value. Fortunatly ARM provide a function (core_cmFunc.h) to 
retrieve the stack pointer value and we can use it.

#### It compiles but how sbrk works ??!
sbrk is a simple memory allocation function. You ask for space and it give you 
a pointer followed by the size of memory you asked for.
Then it keeps in memory the pointer to the next free space available.

##### Where is the heap memory ?
You can find a lot of ressources explaining memory layout, this is well 
documented. \
Here is an example: \
https://www.geeksforgeeks.org/memory-layout-of-c-program/

There is an heap pointer and a stack pointer.
You can see that the heap grows up like our heap_end is incremented in our 
function. \
It's easy to see that a problem occur if the heap or the stack keep growing.
They can both use the other memory space and read/write/execute it.
This is why in our sbrk we check if there is memory to allocate before 
returning anything.
There is a lot of way to handle such a situation, here the program get catched 
in an infinite loop. 
Keep in mind that if this situation occur, this is a big issue because it 
leads to undefined behavior. And this mean that your program use too much heap 
or stack.

#### How do i know when heap starts ?
This is an information provided by the linked script. In our case there is an 
_end at the bottom of the linker script, this is the address just after the 
last data section variable has been copied. This is ere our heap start.

#### How do i use linker script symbol ?
A linker script symbol is like a variable storing nothing. You just get it's 
address corresponding to the checkpoint you call.

Reference: https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html

# Now you can use the library and printf but is it a good idea ?
Printf is a useful function for formating output, but it comes with a cost: it 
takes a lot of memory.

	as a function in your prgram
	as a function using stacks to store data
	as a function using heap for dynamic allocation

Most of time we use simple output formating that we can code for only few byte 
of memory, using few stack and without dynamic allocation. If you need to 
shrink your code to size the flash memory of your device, you shouldn't use 
printf.
You will find below some code comparison with size.

## Just adding printf to the project
### With Newlib-nano and printf and no sections flags
Linker Optimization flag:

	LO_FLAG = 
	
Compiler Optimization flag:

	CO_FLAGS = 
	
Size:

	   text    data     bss     dec     hex filename
	   8388     120    1552   10060    274c LIB-CM4

### With Newlib-nano and printf and section flags
Linker Optimization flag:

	LO_FLAGS = --gc-sections

Compiler Optimization flag:

	O_FLAGS = -ffunction-sections -fdata-sections

Size:

	   text    data     bss     dec     hex filename
	   7796     100    1552    9448    24e8 LIB-CM4

### Without printf and section flags
Linker Optimization flag:

	LO_FLAGS =

Compiler optimization flag:

	O_FLAGS =

Size:

	   text	   data	    bss	    dec	    hex	filename
	   1764	     20	   1540	   3324	    cfc	LIB-CM4

### Without printf 
Linker Optimization flag
	LO_FLAGS = --gc-sections
Compiler optimixation flag
	O_FLAGS = -ffunction-sections -fdata-sections

	   text    data     bss     dec     hex filename
	   1176       0    1536    2712     a98 LIB-CM4

### Comparison
As you can see printf is an heavy function adding a lot of code to our program.
In our case we have a lot of memory so you can add it if you want to but keep 
in mind its cost.

Note that we didn't used the compilation flag "-Os" in our comparison. But you 
can shrink a little more your executable size using it at compile time.
 
