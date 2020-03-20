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


