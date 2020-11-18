# A C translation of the startup file
DISCLAIMER: untested C code in example.

## Reset_Handler label
ldr   sp, =_estack
_estack is just a empty label. Loading a label is like loading an address.
ldr` just load (store) the end of stack address in the stack pointer register.
Note that this is a decreasing stack this is why we gave it the highest address.

`ldr` instruction information:

	https://developer.arm.com/documentation/dui0041/c/assembler/arm-pseudo-instructions/ldr-arm-pseudo-instruction


movs	r1, #0
`movs` simply mov specified value in specified register. like `mov` but it gave 
a status according to result of operation (we retrieve the status in a 
specific register, check the link below).

`mov` instruction informations:

	https://developer.arm.com/documentation/dui0204/j/arm-and-thumb-instructions/general-data-processing-instructions/mov-and-mvn

and the status update:

	https://developer.arm.com/documentation/dui0204/j/writing-arm-assembly-language/conditional-execution?lang=en


b	LoopCopyDataInit
branch to the LoopCopyData "function".
We simply call this "function" code and we skip everything in between.
TO be more accurate we branch/jump to the label.

## LoopCopyDataInit label (Start)
ldr	r0, =_sdata
Same instruction as before. Register 0 store the start address of data section.

ldr	r3, =_edata
Same instruction as before. Register 3 store the end address of data section.

adds	r2, r0, r1
add r0 and r1 and store result in r2. The `S` is for update the status register 
according to result of operation.
Note that r0 contains the start address of data section and r1 the offset

`add` instruction informations:

	https://developer.arm.com/documentation/dui0204/j/arm-and-thumb-instructions/general-data-processing-instructions/add--sub--rsb--adc--sbc--and-rsc

cmp	r2, r3
Compare value in Register 2 and value in Register 3 and update the status 
register according to result of operation.
Note that r3 contains end address of data section and r2 start address of data 
section  + offset (r1)

`cmp` instruction informations:

	https://developer.arm.com/documentation/dui0068/b/arm-instruction-reference/arm-general-data-processing-instructions/cmp-and-cmn

bcc	CopyDataInit
Branch if Carry is Clear. Symply go to "function" CopyDataInit if condition is met.
This is a tought one.
First of all this is a specific thumb instruction:

	https://developer.arm.com/documentation/ddi0210/c/Introduction/Instruction-set-summary/Thumb-instruction-summary?lang=en

And the best ressource I found was this

	https://community.arm.com/developer/ip-products/processors/f/cortex-m-forum/5941/could-you-explain-bcc-command-to-me
	https://community.arm.com/developer/ip-products/processors/b/processors-ip-blog/posts/condition-codes-1-condition-flags-and-codes

CMP is simply a SUBS instruction with result discarded. In our case CMP does 

	SUBS r2,  r3
	r2 = r0 + r1
	r0 = data section start address
	r1 = offset of data to copy

	r2 = address to copy data growing towards data section end address
	r3 = data section end address

We copy data to the heap which is ascendant.
Until we reach the End of section address, start of section + offset will be 
lower. 
So while start address + offset is lower, operation will result in 
unsigned overflow and set the carry flag.

Example:
	
	uint8_t a = 0x05;
	uint8_t b = 0x01;

	SUBS b, a  == 0xFB
	
	This operation cause a unsigned overflow and the cary is set.

As long as the condition is met we jump to the "function"  CopyDataInit.


## CopyDataInit label
ldr	r3, =_sidata
Load (store) in register 3 the address of _sidata.
_sidata is the start address of the section in FLASH. Check `02.LINKER_SCRIPT/` 
project for more details.

/* WARNINGI DON'T GET WHY THEY ARE DOING THIS */
ldr	r3, [r3, r1]
This one is differents than previous `ldr` instruction.
As you can see there is no '=' sign. We just see two regiser in '[]' and that's
 important. The `ldr` instruction have different behavior according to following 
 "arguments".
 Here we ask to load the register with the value at address `[r3 + r1]` 
 r3 is _sidata (start of data section in FLASH), r is still the offset.
 This means copy value at flash address and load it into r3.

`ldr` instruction informations:

	https://developer.arm.com/documentation/dui0068/b/thumb-instruction-reference/thumb-memory-access-instructions/ldr-and-str--register-offset
/* WARNINGI DON'T GET WHY THEY ARE DOING THIS */

str	r3, [r0, r1]
`str` instruction informations:
	
	https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-instruction-set/memory-access-instructions/ldr-and-str--register-offset

Store the value of r3 to address r0 (_sdata) + r1 (offset). 
r3 store the data copied from flash. Here we 
ask to copy value of r3 register to `[r0 + r1]` address (_sdata + offset).

Note: `str` and `ldr` are the same but copy data in opposite direction.

adds	r1, r1, #4
Finally add 4 to r1 register (offset) for the next check and copy.



## LoopCopyDataInit label (end)
If the Bcc condition is not met, we execute the next instruction.

ldr	r2, =_sbss
As we saw earlier, load (store) direct address _sbss in r2. _sbss is only the
 address of start of the bss section generated by linker. 
See `02.LINKER_SCRIPT`project.


b	LoopFillZerobss
As seen earlier, this is simply a branch/jump without any conditional check 
to the named "function"/label. 


## LoopFillZerobss
ldr	r3, = _ebss
store end of bss section address/label into r3

cmp	r2, r3
As we seen previously compare r2 and r3 offset and if cary is clear we jump to 
FillZerobss "function"/label.

bcc	FillZerobss

## FillZerobss:
movs	r3, #0

`str` instruction informations:

	https://developer.arm.com/documentation/dui0552/a/the-cortex-m3-instruction-set/memory-access-instructions/ldr-and-str--immediate-offset

simply mov or copy the value in the register. Now r3 value is 0

str	r3, [r2], #4
Like previously this instruction copy the value in r3 to address stored on r2.
Then increment r2 by 4.
Note that register store 4 byte value, and memory is align on 4 byte by linker.
This is why we can safely copy 4 byte each time.

## Calling you function
Then we just call SystemInit function and main.

Your Main function should NEVER return. But if it does, you fall in the 
next instruction:

bx lr

  https://developer.arm.com/documentation/dui0204/j/arm-and-thumb-instructions/branch-and-control-instructions/b--bl--bx--blx--and-bxj

Which is a branch and exchange instruction set. According to lr we will switch 
to/from Thumb to/fron ARM.
Here is a nice answer explaining this:

	https://stackoverflow.com/questions/27084857/what-does-bx-lr-do-in-arm-assembly-language

And then we simply continue our execution to then Default_Handler which 
contains and infinite loop.

b  Infinite_Loop
branch/jump to Infinite_Loop label again and again...

## .isr_vector...
Finally we see a list of weird instruction with its own section, *.o and size.
This is an array of pointer to function called by hardware like interrupt or 
exception handler.

If your hardware trigger an interrupt, it will get the function pointed by the 
address in this array. We can't change this, all places are defined by hardware.

## ... and weak aliases
And finally we define weak function all redirecting to Default_Handler (an infinite loop). The `weak` alias make sure that if you don't define a function there is a function that will be called. But if you define a function, your function will erase the weak function and YOUR function will be called.
Of course your function must have the same name.

And now you have all you need to build your own startup file. Enjoy :)




