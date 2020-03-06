# [WORK IN PROGRESS] Embedded C development  WITHOUT IDE but using Makefile and VIM (Or any Text editor you want) and CMSIS Library (optionnal but reliable).
This repository contains all the resources I used to learn how to develop on micro controller.
I explain all choices made to build my project.

## Equipment:
- STM32F303VC
https://fr.farnell.com/stmicroelectronics/stm32f3discovery/evaluation-f3-cortex-m4-discovery/dp/2215108

- Mini USB cable:
https://www.amazon.fr/gp/product/B00P0GI68M?ref=ppx_pt2_dt_b_prod_image

- Bluetooth 2.0:
https://www.amazon.fr/gp/product/B074GMQ6G3?ref=ppx_pt2_dt_b_prod_image

- FTDI:
https://www.amazon.fr/gp/product/B06ZZ65R3H/ref=ppx_od_dt_b_asin_title_s00?ie=UTF8&psc=1

- Male/Male, Male/Female, Female/Female wiring:
https://www.amazon.fr/dp/B00OR3LWLM/ref=pe_3044141_189395771_TE_3p_dp_1

## Why make another embedded resource ?
Because a lot of resources use an IDE which hide the complexity and logic behind 
embedded development.
Because I wanted to understand how and why ?
Because you can learn software development on embedded system only using
Makefile and text editor (with of course an appropriate tool chain).
Because i think it's better to understand what you do and i wanted to make my
work useful for people like me :).

## Why use CMSIS Library ?
CMSIS for ARM Cortex Microcontroller Software Interface Standard is a Lower Layer Abstraction library (hardware). It's constructor independent for Micro controller using Cortex M series processor. 
https://developer.arm.com/tools-and-software/embedded/cmsis

## Is there everything I need to know on this page ?
No. Just what i think is mandatory to know/understand for 
embedded software development. No electronics skills are needed
and i don't cover this part (yet).

## How is your project build ?
A readme.md file explains all choices made for each subject/file.
I tell what I'm doing and why. Sometimes (or usually) i add a lot of other 
sources to read, to go further/deeply.

## Is there an order for reading ?
No. Each little project is independent and can work alone.
You have to mix projects in order to see results.

For example:
- Use TIMER to delay leds blink
- Use ITM/UART to display sensor data (I2C)

## Something else ?
If you need more information about a part of this resources, just ask me.
If something in here helped you in some way, let me know :).

Enjoy your embedded development journey :D

## Resources

USER MANUAL:
https://www.st.com/content/ccc/resource/technical/document/user_manual/8a/56/97/63/8d/56/41/73/DM00063382.pdf/files/DM00063382.pdf/jcr:content/translations/en.DM00063382.pdf

DATA SHEET:
https://www.st.com/resource/en/datasheet/stm32f303vc.pdf

REFERENCE MANUAL:
https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf

ECOMPASS MODULE: 3D ACCELEROMETER AND 3D MAGNETOMETER:
https://www.st.com/resource/en/datasheet/lsm303dlhc.pdf

3-AXIS DIGITAL OUTPUT GYROSCOPE:
https://www.st.com/content/ccc/resource/technical/document/application_note/2c/d9/a7/f8/43/48/48/64/DM00119036.pdf/files/DM00119036.pdf/jcr:content/translations/en.DM00119036.pdf

## One particularly good resources i started with
https://docs.rust-embedded.org/discovery/index.html
It's in Rust because i wanted to learn Embedded and Rust at the same time (bad idea).
It's a well written tutorial you can refer too even if it's not in C.
But there is just one thing missing:
"I've taken care of initializing everything" => This is the part covered here.
