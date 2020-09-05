# Understand PWM
Tutorial explaining how it works:
https://learn.sparkfun.com/tutorials/pulse-width-modulation/all

Great video tutorial
https://www.youtube.com/watch?v=qAZjdx71ePc


## Objective
###	What we want to do ?
We want to control the motor speed using PWM.

### Do we need specific component ?
Here we are using a brushless motor for a drone project.
OUr brushless motor needs an ESC (Electronic Speed Controller) which manage 
our PWM signal and translate it to our motor.
ESC are electronic component that you can build by yourself.
Understanding how a Brushless motor and how an ESC manage it is really 
interesting but it's a lot of work to make it work and with efficiency.
So we just buy it.


### How it works ?  We use timer to set a signal hight for a brief period.
The more the signal stay hight the more speed you ask.

## Finding the right information
### Exploring device documentation
#### User Manual
Usually a good start to search for information but there is nothing about PWM.

#### DataSheet
According to the Datasheet

	page 1
	Features

We have a total of 6 timers we can use to produce PWM.

A quick look to `Contents` and we find

	page 23
	3.17 Timers and watchdogs

We have 3 types of timers described in 

	page 24
	3.17.1 Advanced timers (TIM1, TIM8)
	3.17.2 General-purpose timers (TIM2, TIM3, TIM4, TIM15, TIM16, TIM17)
	3.17.3 Basic timers (TIM6, TIM7)

But only two can be used to generate PWM:

- Advanced Timers
- General-purpose timers

For now we just want to understand and make it works so let use the simplest 
timer possible in our case: `General-purpose Timers`.
And more specificaly `general-purpose timers with mid-range features` 
timers 15/16/17. They have enought feature for our use case.
Now let's jump to the reference manual to see how to use it.

#### Reference manual
Again a quick look on the `contents` section and we find our timers.

	page 683
	General-purpose timers (TIM15/TIM16/TIM17)

Here we arem now let's find the information we need.
There is a lot of feature for Timer even for the general purpose one, as usual 
we will focus on what we need: use timer to generate a PWM signal.
We used timer in our previous project `3.TIMER` so we know some 
useful feature already. And how to configure them to count time.
It's seems that there is a difference between TIM15 and TIM16/17:

	page 683
	23.2 TIM15 main features
	page 684
	23.3 TIM16/TIM17 main features

We will use TIM16/TIM17 because they use less feature so they are simpler to 
use in our use-case.

	page 687
	23.4 TIM15/TIM16/TIM17 functional description
	23.4.1 Time-base unit

We see that this general purpose timers are similare to the basic timer we used

 in `3.TIMER` project, except for the repetition counter.
 Even if there is a lot of information, I recommand you to read at least the 
 following parts:

	page 687
	23.4.1 Time-base unit
	
	page 689
	23.4.2 Counter modes

	page 693
	23.4.3 Repetition counter

	page 694
	23.4.4 Clock selection

And finally what we are here for

	page 702/703
	23.4.10 PWM mode

A small part but we have to know how timers works to understand it.

### Selecting information
Of course for our part, if you are confortable with timer you can focus only on

	page 702/703
	23.4.10 PWM mode

## Finding the right configuration

Enjoy :)

