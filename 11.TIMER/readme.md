# Understand TIMERs

## Why use timers ?
There is a lot of reason to use timer but we will focus on a simple jobs for 
now: implement a delay function.

You have two ways to implement a delay function:
	- A for/while loop and a counter
	- A timer with an even signal

Software for/while delay loop:
Perks:
	- easy to implement

Cons:
	- should be optimized out by compiler
	- not as accurate as TIMER
	- rely on software counter variable

Hardware timer:
Perks:
	- really accurate
	- no risk to be optimiwed out by compiler
	- works with hardware implementation
	- easy to implement to count with millisecond
Cons:
	- need to understand how it works

Obviously the best option is to use timer if we can.

# How TIMER works ?
article or video
# How to program it ?
We need to find information on the device documentation.

## Exploring device documentation
### Datasheet
### User manual
### Reference manual
#### Finding detailled information
#### Finding the right configuration
## How to use them ?
We use one of the basic timer (TIM6). Like every peripheral we have to 
enable its clock.
Then ...

