# GPIOE example
If we need to work on GPIOE for any reason.

## GPIOE enable clock
We have to enable its clock before configuring and using it.

Tips: the peripheral managing clock is the RCC (Reset and Clock Control)

Let's take a look at the register taking care of clock control (RCC)
Go to the memory map page again
	
	page 51


A quick search and we find it 

	page 52

We find that RCC in the AHB1 bus with the following boundaries 

			(start)		   (end)
	AHB1: 0x4002 4000 - 0x4002 03FF

RCC is inside the memory area and has the following boundaries

		   (start)		  (end)
	RCC: 0x4002 1000 - 0x4002 13FF

Click on the peripheral register map and go to 

	Section 9.4.14 on page 166

Find the register that ENable gPIOE

	IOPEEN in RCC_AHBENR at offset 0x14

If we want to enable GPIOE clock we need to rebuild register's address
Add RCC_AHBENR offset to our RCC peripheral start address

	RCC start address = 0x4002 1000
	RCC_AHBENR offset = 0x14
	register address  = 0x4002 1014

In the code you will get the address like this

	RCC.AHBENR = 0x4002 1014 (register address)

And the register pointed by this address like this

	RCC->AHBENR = register value

Now that we have the right register address pointing to 32 bits value (uint_32).
We just have to change value at address like this:
Enable GPIOE clock:
`*((uint32_t *)0x40021014) |= (1 << 21);`
Set the 21th bit of uint32_t value pointed by address to 1.


## GPIOE configure register
After enabling the clock we may want to configure our GPIO.

Tips: the peripheral managing GPIOE is ... GPIOE

Go to the memory map page again
	
	page 51


A quick search and we find it 

	page 51

We find that GPIOE in the AHB2 bus with the following boundaries 

			(start)		   (end)
	AHB2: 0x4800 1400 - 0x4800 03FF

GPIOE is inside the memory area and has the following boundaries

		   	 (start)		(end)
	GPIOE: 0x4800 1000 - 0x4800 13FF

Click on the peripheral register map and go to 

	Section 11.4.12 on page 243

Find the register you want to configure, for example

	GPIOx_BSRR on page 244

If we want to configure GPIOE ODR we need to rebuild register's address
Add GPIOx_BSRR offset to our GPIOE peripheral start address

	GPIOE start address = 0x4800 1000
	GPIOx_BSRR offset 	= 0x18
	register address	= 0x4800 1018

In the code you will get the address like this

	RCC.AHBENR = 0x4800 1018 (register address)

And the register pointed by this address like this

	RCC->AHBENR = register value

If we want to configure another register we just need to choose another offset.

Now that we have the right register address pointing to 32 bits value (uint_32).
We just have to change value at address like this:


Configure GPIOE 8 as output:
`*((uint32_t *)0x48001018) |= (1 << 8);`
Set the 8th bit of uint32_t value pointed by address to 1.

Of course there is a better way to write and use register and this is where
CMSIS header files  and libraries are useful.

