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
We simply call this "function" code.

ldr	r0, =_sdata
Same instruction as before. Register 0 store the start of data section address.

ldr	r3, =_edata
Same instruction as before. Register 0 store the end of data section address.

adds	r2, r0, r1
add r0 and r1 and store result in r2. The `S` is for update of status register 
after operation.

	https://developer.arm.com/documentation/dui0204/j/arm-and-thumb-instructions/general-data-processing-instructions/add--sub--rsb--adc--sbc--and-rsc

cmp	r2, r3
Compare value in Register 2 and value in Register 3.
Here we try to 
	https://developer.arm.com/documentation/dui0068/b/arm-instruction-reference/arm-general-data-processing-instructions/cmp-and-cmn


	bcc	CopyDataInit
	ldr	r2, =_sbss
	b	LoopFillZerobss


