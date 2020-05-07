# Understanding LED
Here is a good explaination about how LEDS works:\
https://learn.sparkfun.com/tutorials/light-emitting-diodes-leds/all

If you read carefully this article you should now be comfortable with the
principle. \
You can refer to this article if you are lost. \
Now let's start the programming part!

## Objective
###	What we want to do ?
Usually the first thing we learn in embedded development is blinking a led. \
It's easy, we can actually see the result and it's pretty cool.
Here we are going to light up one or more leds. No more. \
If you want to make a led blink, you can:
- Learn how to use timer with `3.TIMER` project (Clean way)
- Create a counting loop (easy to do and useful but not reliable)

### How it works ?
Using a LED embedded in your device is like using an external LED, except 
you just avoid the building part. \
Like an external LED it is linked to a GPIO pin. \
Switch on or off a LED is simply sending current or not to it throught the 
GPIO pin. \
Setting a GPIO output will light up the LED and this is that simple.


## Finding the right information
### Exploring device documentation
#### DataSheet
First step is always taking a look at the datasheet. \
Unfortunatly there is no information about LEDS for us in this file.
But we know that, like external LED, our LED is linked to a GPIO pin.
So let's check if there is information about GPIO.
We find information

	page 20
	2.3.21 GPIOs (general-purpose inputs/outputs)

Few informations are provided in this part. \
We just know that there is a way to program GPIO by software.

There is also information about (GPIO) pins

	page 21
	3 Pinouts and pin descriptions

This is a good source of information, especially this part

	page 28
	Table 5. Medium-density STM32F103xx pin definitions

But as you can see, it's not usefull for us now.

#### User manual
There is no User Manual for our device.

#### Physical device
At least we can find what we are looking for here.
According to our board

	PC13 => led

This is not the first source of information but at least its written somewhere.

#### Reference manual
This is where we find all detailled information.
First of all we need to learn more about GPIO.
According to contents section

	page 159
	9 General-purpose and alternate-function I/Os (GPIOs and AFIOs)

There is a lot of information and the best way to find what we are looking for 
is to focus on the important part.
If you just discover GPIO I strongly advise to read the following part:

	page 159
	9.1 GPIO functional description

	page 161
	9.1.1 General-purpose I/O (GPIO)
	9.1.2 Atomic bit set or reset

	page 163
	9.1.8 Output configuration

This is quite a lot of reading so if you don't understand everything it's ok. \
If you don't get it while reading, there is more than one way to configure 
our GPIO but we will focus on the most obvious one. \
If you read the documentation further, you should have see the last part of 
the GPIO section

	page 171
	9.2 GPIO registers

Those are the register we use to configure our GPIO.
Now you have all information you need to code. If you can't find information 
you need, you can refer to the following section.

### Selecting information
This is a lot of information which part should I focus on ?
You have to set the right configuration:

	page 161
	9.1.1 General-purpose I/O (GPIO)

Then output data:

	page 161
	9.1.2 Atomic bit set or reset
	
	OR

	page 161
	9.1.1 General-purpose I/O (GPIO)

This should be enought to make it works but if you still have trouble, read 
the last part.

## Finding the right configuration
Here I share the configuration I choosed to light up the LED.

### Inititalize GPIO
According to information we find, LED is connected to PC13.
PC13 is short for GPIOC 13.
We know we have to work on GPIOC.

According to

	Reference Manual
	page 171
	9.2.1 Port configuration register low (GPIOx_CRL) (x=A..G)

There are 4 modes and we want to setup our GPIOC13 to 
`Output mode, max speed 2 Mhz`. It's easy to understand why we chosed Output 
mode but the speed ? \
Check the last part of the file to understand our choice. \
Generally we limit the speed to our bus clock. \
Here our bus clock is setup in the `src/system_stm32f1xx.c` file to 8 Mhz.

Then, there are 4 CNF configuration available and we want to output push 
(send current) or pull (don't send current).

And this is it.

### Manage communication
#### Switch on our LED
This part is really simple, if you read carefully the documentation.
There is 2 way to ask to output current:
- The BSRR register
- The ODR register

Enabling the right bit to set our GPIOC13 will switch on our LED.
#### Switch off our LED
This part too is really simple, if you read carefully the documentation.
There is 3 way to ask to output current:
- The BSRR register
- The BRR register
- The ODR register

Enabling (or clearing) the right bit to set our GPIOC13 will switch off our 
LED.

### Enable clocks
This was too easy right ? Everything is setup but it's still not working ? \
This is because we missed an essential step:

	Enable the GPIOC clock

Each peripheral/GPIO is disabled by default to avoid unecessary power 
consumption. \

We have to enable the clock before configuring and testing anything. \
But is there e Register which Control the Clock ? \
Of course, there is. The RCC for Reset and Clock Control is used to manage 
clocks in our device.
There is a lot to understand about clocks and if you want to start you can 
read `0.BUILD/SYSTEM_STM32/readme.md`. 
But for know we will focus on our goal. I give you a little shortcup

	page 157
	8.3.13 RCC register map

Here we can find what we are looking for ENable our gpIOPC.
One last line of code before configuring your GPIOC and we are done. The LED 
should light up... maybe.

### What's wrong with this LED ??!
Yeah, I know you code it right but the LED doesn't light up. \
If you have done it right you certainly see your led blink briefly. \
Your code is not wrong, this is how you handle LED but not this one. \
I know this is weird but PC13 has a particularity and of course to keep it 
simple they made it easy to spot and understand (just kidding).
Read the following note

	Datasheet
	page 33
	Table 5. Medium-density STM32F103xx pin definitions (continued)
	Note 5

PC13, PC14 and PC15 must not be used to drive LED so why did they used this 
pin to manage a LED ?
I don't know, but I know they have done it the right way and 
their implementation has a weird consequence on our LED:

	The LED turn on when current is LOW.

Note that we must stay under 2 Mhz too, so we have to set our OSPEEDR 
register accordingly.
That's all now you have everything you need to manage this LED.

