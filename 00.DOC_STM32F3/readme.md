# STEP 00: Documentation
First step in our project: get the right documentation.
We need at least the Reference Manual and the Datasheet of our microcontroller.

## Where do I find it ?
I assume you are using the same microcontroller, or a device from the same 
product family and manufacturer, so you should find the documentation in this 
repository.

But it's essential to know where to find this documentation for your futur 
project, so take a look at ST's website:

	https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html#documentation

Don't be afraid to look in the documentation and if you are, I encourage you 
to  follow this tutorial on microcontroller :).

Note: a documentation usually cover a whole product family especially for 
refence manuals.

## Will I need it at some point ?
Yes. Working on embedded device imply having a deep understanding of what is 
happening on your microcontroller and its peripherals. At some point nobody 
else will have the same board, with the same sensor as you. You will have 
trouble to get some relevant informations on your issue and you will be stuck.

Unless you know how to use your datasheet and your reference manual to identify
your problem and find a solution. This is our primary and most reliable source 
of information.

For this tutorial you don't need to buy an oscilloscope, multimeter or logic 
analyzer but they can make the debugging part way easier.

# First challenge
Because working on embedded device imply manipulating a lot of register.

For example:
- to configure peripherals
- to read data from them

One of the first question I had when I have started to code on embedded device 
is: How do I work with registers ? ST's libraries as many others provide 
you some header files with macro and structure already defined. But how do they
 build it ?

# Memory mapping
# How do we work with register ?
Register are simply data stored at a specific memory address.
And we manipulate data pointed (registers) by those adresses.

Need to know in which "memory area" they are are and at which address.

# Where is memory mapping ?
Just search for memory map in the Reference Manual.
You should end up 

	page 2

redirecting you to 

	page 51.

All the memory area are here. You should note that they are split by 
peripherals and each peripheral has its "memory area" where it stores its
 registers.

