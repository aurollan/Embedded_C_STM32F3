# A C translation of the startup file
DISCLAIMER: untested C code.

ldr   sp, =_estack
`ldr` just load the stask pointer address in the stack pointer register.
Note that this is a decreasing stack and we gave it the highest address.

`ldr` instruction information:

	https://developer.arm.com/documentation/dui0041/c/assembler/arm-pseudo-instructions/ldr-arm-pseudo-instruction


movs	r1, #0
`movs` simply mov specified value in specified register. like `mov` but it gave 
Status according to result of operation.

`mov` instruction informations:

	https://developer.arm.com/documentation/dui0204/j/arm-and-thumb-instructions/general-data-processing-instructions/mov-and-mvn

and the status update:

	https://developer.arm.com/documentation/dui0204/j/writing-arm-assembly-language/conditional-execution?lang=en


b	LoopCopyDataInit
branch to the LoopCopyData "function".
We simply call this "function" code and we skip everything in between.

ldr	r0, =_sdata
Same instruction as before. Register 0 store the start address of data section.

ldr	r3, =_edata
Same instruction as before. Register 0 store the end address of data section.

adds	r2, r0, r1
add r0 and r1 and store result in r2. The `S` is for update the status register 
according to result of operation.

	https://developer.arm.com/documentation/dui0204/j/arm-and-thumb-instructions/general-data-processing-instructions/add--sub--rsb--adc--sbc--and-rsc

cmp	r2, r3
Compare value in Register 2 and value in Register 3 and update the status 
register according to result of operation.

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
	r1 = offset to next data to copy

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



	ldr	r2, =_sbss
	b	LoopFillZerobss


