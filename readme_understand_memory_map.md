# Why I need to understand memory mapping ?
Working on embedded device imply manipulating a lot of register.
For example We work on register to configure peripherals, read data from them
or simply enable a peripheral clock.

# How do we work with register ?
Register are simply adresses.
And we manipulate data pointed by register.

# Where is memory mapping ?
Just search for memory map in the Reference Manual.
You should end up page 2 redirecting you to page 51.
All the memory mapping is here.

# GPIOE example
If we need to work on GPIOE for any obscur reason.

We want to configure GPIOE register:
- GPIOE is in AHB2 memory with boundaries 0x48000000 to 0x480017FF
- GPIOE has boundaries 0x48001000 to 0x480013ff
- you can say that GPIOE is 0x48000000(AHB2) + 0x1000(GPIOE offset)
- click on the section 11.4.12 to be redirected to page 243
- Here we find only offset and they are offset of your GPIOx address.
- Our GPIOE address is 0x48001000 so if we want to GPIOE MODER we need to add 
0x00: the GPIOE_MODER address is: 0x48001000 + 0x00 = 0x48001000
- If we want to configure GPIOE ODR we need to add 0x14 to our GPIOE address.
GPIOE_ODR address is 0x48001014

We want to enable GPIOE clock:
- Let's take a look at the register taking care of clock control (RCC)
- Go to the memory map page again
- We find RCC in the AHB1 bus with boundaries (0x40020000 - 0x400243FF)
- RCC has boundaries 0x40021000 - 0x400213FF
- Jump to section 9.4.14 on page 166
- Find the register that ENable gPIOE
- We find IOPEEN in AHBENR at offset 0x14
- If we want to enable GPIOE clock we need to add 0x14 to our RCC_AHBENR
address. RCC_AHBENR address is 0x40021014

Now that we have the right register address pointing to 32 bits value (uint_32).
We just have to change value at address like this:

Enable GPIOE clock:
`*((uint32_t *)0x40021014) |= (1 << 21);`
Get the uint32_t value pointed by address and set the 21th bit to 1.

Configure GPIOE 8 as output:
`*((uint32_t *)0x48001014) |= ((1 << (8 * 2));`
Get the uint32_t value pointed by address and set the 16th bit to 1.

Of course there is a better way to write and use register and this is where
CMSIS header files are useful.
