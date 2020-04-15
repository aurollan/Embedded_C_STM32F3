# Understand LEDS
Usually the first thing we learn in embedded development is blinking a led. \
It's easy and we can actually see the result and it's pretty cool.
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
Do we have a device with at least a LEDS to light ? \
First step is always taking a look at the datasheet. \
Unfortunatly there is no information about leds for us.

### User manual
Second step is looking in the User_Manual.\
There is a part called LEDs.

	page 18
	6.4 LEDs

We can leave the following LEDS:

	- LD1 PWR
	- LD2 COM

They are used by the microcontroller to give us information.

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

## Configuring device peripherals
### Reference manual
#### Undertsanding what you have to do
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
looking for. We know that we have to ouput current flow to light up a LED.

They have an "x" at the end of their name and we are looking for IO PE8 to 
PE15.

Let's drop the number and look for something using IOPE.

	GPIOPE ? GPIOE ? IOPE ? IOE ?

There is only 1 match with GPIOx that we see in GPIO introduction.

If we look for GPIOE we twice the same information page 51 and 54. \ 

Both of those pages redirect us to:

	page 243
	Section 11.4.12
	Table 73
	GPIO register map and reset values

So what are those register for ? Just scroll up to read it.
Are we going to read all this documentation ?

No we will focus on our goal: output (data) signal to switch on a LED.

#### Undertsanding what you need to do
#### Finding the right configuration
There is 2 steps:
- GPIO have input and output mode, so let's put all to output mode.
- Let's output data to each pin now to see them light up.

Not working yet ?
That normal, just look for IOPE.
You should end up page 148 with a helpful note.

Get it ?
Peripheral clock is not enable. Just enable it before doing any work on GPIOE
and that's it.
Look at p. 166 RCC register map.

Leds should light up.
Welcome to this beautifull world of embedded programming :)
## How to code
### Enable peripherals
#### Enable GPIO and USART1
#### Do we need to enable all those GPIO pins ?
### Setting up peripherals
#### Setup GPIOE pins
### Coding switch function
#### Build switch on function
#### Build switch off function
