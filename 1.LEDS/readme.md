# Understand LEDS
Usually the first thing we learn in embedded development is blinking a led. \
It's easy, we can actually see the result and it's pretty cool.
Here we are going to light up one or more leds.

# How LEDS works ?
Here is a good explaination about how LEDS works:\
https://learn.sparkfun.com/tutorials/light-emitting-diodes-leds/all

If you read carefully this article you should now be comfortable with the
principle. \
You can refer to this article if you are lost. \
Now let's start the programming part!

# How to program it ?
We need to find information on the device documentation.

## Exploring device documentation
### Datasheet
First step is always taking a look at the datasheet. \
Do we have a device with at least a LEDS to light ? \
Unfortunatly there is no information about LEDS for us in this file.

### User manual
Second step is looking in the User_Manual.\
There is a part called LEDs to read.

	page 18
	6.4 LEDs

We can leave the following LEDS:

	- LD1 PWR
	- LD2 COM

They are used by the microcontroller to give us important information.

And focus on the following LEDS:

	- User LD3 I/O PE9
	- User LD4 I/O PE10
	- User LD5 I/O PE11
	- User LD6 I/O PE12
	- User LD7 I/O PE13
	- User LD8 I/O PE14
	- User LD9 I/O PE15

As said LEDs from 3 to 10 are connected to IO PE 8 to 15.
And that's all.

So now let's take a look to reference manual to get more information.

### Reference manual
#### Finding detailled information
This is were we are going to find all detailed information we need to program 
our device. \
Lets search for this "IO" things we found earlier.

First things we find in "Contents" is GPIO (General Purpose I/O). 
That seems pretty relevant. 

Click to get redirected to 

	p228
	11.1 Introduction
	11.2 GPIO main features

We see that they are used for Input/Output operation: that's what we are 
looking for. \
We know that we have to ouput current flow to light up a LED.

They have an "x" at the end of their name and we are looking for IO PE8 to 
PE15.

Let's drop the number and look for something using IOPE.

	GPIOPE ? GPIOE ? IOPE ? IOE ?

There is only 1 match with GPIOx that we see in GPIO `11.1 Introduction`.

If we look for GPIOE we twice the same information 

	page 51 
	page 54

Both of those pages redirect us to:

	page 243
	Section 11.4.12
	Table 73
	GPIO register map and reset values

So what are those register for ? \
Fortunatly for us their name are self-exlpaining. \
Just scroll up to read it.

###### Do I have to read everything ?
No there is a lot of information and we are going to keep it really simple and 
stick the basic principle:

	OUTPUT a (DATA) signal to switch on a LED.

## How to code
Read `0.TOOLS/readme_understand_memory_map.md` if you have trouble to find the 
register addresses.

#### Finding the right configuration
There is a lot to learn about GPIO but for now we need to focus on our goal.\
We need to make sur that GPIOE is configured to do what we expect. \
There is 2 steps to do it:

- GPIO have input and output MODE, so let's put all to output MODE.
- Let's OUTPUT DATA to each pin now to see them light up.

If you gather all information you should know which register to configure, but 
you should end up without any signal output and no LEDS lightning. \
What are we missing ?

#### Not working ? Enable peripherals
That normal, just look for IOPE in your Reference Manual.
You should end up 

	page 148

with a helpful note.

Get it ?
Peripheral clock is not enable. \
Just enable it before doing any work on GPIOE and that's it. \
Look at 

	page 166
	9.4.14 RCC register map

Leds should light up. \
If you have trouble for the coding part just take your time and try or keep 
reading.

### Coding function
#### GPIOE enable
Read `0.TOOLS/readme_understand_memory_map.md`.
register addresses.
#### Build switch on function
If you read carefully the GPIO register yu should have find 2 way to switch on 
LEDS:

- Writing to the Output Data Regiser
- Writing to the 0...15 bit of the Bit Set Reset Register

#### Build switch off function
And 3 way to switch off the LEDs:

- Clearing the bit in the Output Data Regiser
- Writing to the 16...31 bit of the Bit Set Reset Register
- Writing to the 0...15 bit of the Bit Reset Register

You just need to use the right bitwise operator to set or reset the 
appropriate bits.

Welcome to this beautifull world of embedded programming :)
