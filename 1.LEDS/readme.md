# LEDS
Usually the first thing we learn in embedded development is blinking a led.
Here we are going to light up one or more leds.
If you need to make it blink you need to understand how to use timer and create
a delay function. Check the TIMER project.

##### Important note
You need to read "0.TOOLS/readme_understand_memory_map.md" to code anything 
working.

## Where do i start ?
Read the user manual, then the reference manual. They are the most accurate and
detailed source of information. But we won't read all those documentation. We
will try to find only information that we need.

## How to find information ?
### User manual
https://www.st.com/content/ccc/resource/technical/document/user_manual/8a/56/97/63/8d/56/41/73/DM00063382.pdf/files/DM00063382.pdf/jcr:content/translations/en.DM00063382.pdf

There is a part called LEDs telling us which pin is connected to each led. (6.4 - page 18/37)
We work only on E-compass leds called "User LDx".
As said LEDs from 3 to 10 are connected to IO PE 8 to 15.
And that's all.

So now let's take a look to reference manual to get more information.

### Reference manual
https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf

This is were we are going to find all detailed information we need to program on this device.
Lets search for this "IO" things we found earlier.

First things we find in "Contents" is GPIO ( General Purpose I/O). 
That seems pretty relevant. 

We get redirected to p228.
Read 11.1 Introduction and 11.2 GPIO main features
We see that they are used for Input/Output operation: that's what we are looking for.

They have an "x" at the end of their name and we are looking for IO PE8 to PE15.

Let's drop the number and look for something using IOPE.

GPIOPE ? GPIOE ? IOPE ? IOE ?

There is only 1 match with GPIOx that we see in GPIO introduction.

If we look for GPIOE we twice the same information page 51 and 54. 
Both of those pages redirect us to:
- Section 11.4.12 on page 243

Table 73 page 243. GPIO register map and reset values

So what are those register for ? Just scroll up to read it.
Are we going to read all this documentation ?

No we will focus on our goal: output a led (data) signal.

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
