# Clocks and microcontroller
I tried to avoid this subject for so much time and this cause me a lot of 
trouble. I recently updated the system_stm32f3xx.c file and lot of
communication protocol don't work anymore. Of course I should have known it's
 important but now I know...

## What is a clock ? Why use a clock ?
A clock is an electronic oscillator called a clock (or clock generator) generates a sequence of repetitive pulses called the clock signal which is distributed to all the memory elements in the circuit.

In a synchronous logic circuit (like microcontoller), this clock signal is applied to every storage element, so in an ideal synchronous circuit, every change in the logical levels of its storage components is simultaneous. \

Reference: https://en.wikipedia.org/wiki/Clock_signal \
Reference: https://electronics.stackexchange.com/questions/93875/why-are-clocks-used-in-computers \
Reference: https://en.wikipedia.org/wiki/Sequential_logic#Synchronous_sequential_logic \
Reference: https://en.wikipedia.org/wiki/Synchronous_circuit \

The clock is like your device metronome/rythm.
If you want your device to run faster your have to increase the beat.

## Is it important ?
Yes it is. Some communication bus use clock signal to synchronize transfert according to your system clock. With faster clock your device can make more operation by second by it increase your device energy consumption. There maybe more importante topic related to clock signal but I don't know them (yet).

## How to detemine our device clock signal speed ?
Here is a great tutorial on TizanaMenYo Youtube channel:
https://www.youtube.com/watch?v=zkmk9uAMa04
Part 1 and 2 are important.

Here is another tutorial from Embedded Systems with ARM Cortex-M Microcontrollers in Assembly Language and C Youtube channel. It the same content but he use schematic like we see in datasheet.
https://www.youtube.com/watch?v=o6ZWD0PAoJk

PLL = phase-locked loop
