# Understand Systick timer
Working in embedded software development usually imply time sensitive task.
We need an accurate measure of time and as we saw it in a previous project
there is TIMERs for this.

##### What is the difference between TIMER and Systick ?
Systick is a part of the ARM core. It's a basic timer measuring elapsed time.
TIMER are vendor implemented timers, we can use them for measuring elapsed 
time, but they have more features so we can achive more complex task with them.

Note :
If you need to use time using day/weeks/minute/second/month/year 
you should take a look to RTC (Real Time Clock) not basic timers we use here.

## Objective
###	What we want to do ?
In our previous project `3.TIMER` we make the led blink using a basic TIMER
and a polling (waiting until it finish) method to blink the led.
Now we want to use systik and interrupt to blink the led.
We want to be able to choose the delay in millisecond.

Note that Systick is a particular "interrupt" function: an exception.

### How it works ?
Systick is hardware counter that use a clock signal to decrement.
For example if your device run at 48 Mhz:
- 48 000 000 hertz per second
- 1 / 48 000 000 = 0.00000002083 second
- 0.00000002083 second = 0.00002083 millisecond
- 0.00002083 millisecond = 20.83 nanosecond

Then the systicks' counter is decremented every 20.83 nanosecond.
It's really fast.

There is no way to reduce this speed  as we seen in the `3.TIMER` project 
and our counter variable is 24 bits.
So the maximum value is 16,777,215.
If your device run at 8 000 000 Hz (8Mhz), interruption will occurs
every `16 777 215 / 8 000 000 = 2,097 second` maximum.
If your device run at 72 000 000 Hz (72 Mhz), interruption will occurs
every `16 777 215 / 72 000 000 = 0,233 s` maximum.
I think you get the point.

Note:
We don't HAVE TO use interrupt with systick, we can use polling like in 
`3.TIMER` project.


### Why use Timer/Systick ?
You have two ways to implement a delay function:
	- A for/while loop and a variable as counter
	- A timer with an even signal

#### Software for/while delay loop
Perks:
	- easy to implement

Cons:
	- not as accurate as TIMER
	- could be optimized out by compiler
	- unreliable and useless if an interupt occurs
	- software implementation, program is locked while measuring time
	- difficult to implement to count with millisecond

#### Hardware timer
Perks:
	- really accurate
	- no risk to be optimized out by compiler
	- realiable even if an interrupt occurs
	- works with hardware implementation, does not stop program flow
	- easy to implement to count with millisecond
Cons:
	- need to understand how it works

Obviously the best option is to use timer if we can.

## Finding the right information
### Exploring device documentation
#### DataSheet
On the first page

	page 1
	Features

We learn that our device has (up to) 13 timers. 
And the one we are looking for: SysTick timer.

Then we find

	page 25
	3.17.6 SysTick timer

We know which feature Systick has now, but we need more detailled informations.

#### Reference manual
Here we are supposed to find everything we need.
A quick look to the Content table give us no information.
Except

	page 8
	14.1.2 SysTick calibration value register

but finally there is nothing for us here.
Searching for the `systick` keyword doesn't give us much more information.
But there is a reason for this, remember `Systick is an ARM core 
functionnality`.

Fortunatly, there is documentation provided by ARM.

#### Cortex M4 Devices Generic User Guide
In the content section (page 4, chapter 4) we finally find our system
 timer `SysTick`.

	page 249
	4.4 System timer, SysTick

Information about how to use it are pretty explicit, so I suggest you to read
 everything

	page 249
	4.4.1 SysTick Control and Status Register

	page 250
	4.4.2 SysTick Reload Value Register

	page 251
	4.4.3 SysTick Current Value Register
	4.4.4 SysTick Calibration Value Register

### Selecting information
We need all information provided by the Generic User Guide.
Plus the function is already provided by ARM int the header files.


## Finding the right configuration
### If you want to do it by yourself
Check

	page 252
	4.4.5 SysTick usage hints and tips

And follow the initialization sequence.
You should end up with the right configuration but don't forget to setup 
interrupt priority if you need to.
Beware of integer overflow, make sure your value fit in 24 bits.

And note that:
SYST_CALIB 	=> SysTick->CALIB
SYST_CVR	=> SysTick->VAL
SYST_RVR	=> SysTick->LOAD
SYST_CSR	=> SysTick->CTRL

This should be enough for the Systick Configuration part.

#### I can't enable the systick interrupt
Of course, there is a function to enable called 
` __STATIC_INLINE void NVIC_EnableIRQ(IRQn_Type IRQn)` and it doesn't work.
If you check `inc/core_cm4.h` file we see that the `Value cannot be negative.`
and our `SysTick_IRQn = -1`.
I tried and failed, getting stuck in an exception handler and infinite loop.
Your code should work fine simply by configuring the systick.

### If you simply want to make it work
Use the `__STATIC_INLINE uint32_t SysTick_Config(uint32_t ticks)` from 
`inc/core_cm4.h` file.
Note that some vendor provide their own `systick_config` function, but it's 
not the case in our project (CMSIS library). If you want to verify this you 
can check the`inc/stm32f303xc.h` file.

#### How to use interrupt function ?
Interrupt functions are already implemented by the `asm/startup_stm32f030x8.s`.
But the file provide weak aliases.

	asm/startup_stm32f030x8.s
	line 176

This mean that you can override it simply by implementing your own version of 
target function.
Even if you can use your own version you must use expected definition:

- return nothing (void)
- no parameter (void)

You MUST use exactly the same name for a simple reason:
- The interrupt function call is implemented with that name and you can't 
change it

Except the 3 points above, you can do whatever you want in this function.
In this project, we just switch on the led on for 500 ms to actually see the 
systick interrupt before returning to our main program which turn it off.

### What is the speed of our clock ?
Our device max clock speed is 72 Mhz but in this project we use 
the `src/system_stm32f3xx.c` from STM32 cube repository, setting up 
our clock speed to 8 Mhz.
Now you have enought information to make Systick interrupting your program
flow every x ms.

Enjoy :)


