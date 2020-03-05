# [WORK IN PROGRESS] Embedded C development  WITHOUT IDE but using Makefile and VIM (Or any Text editor you want)
This repository contains all the resources I used to learn how to develop on microcontrolleur.
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

## Is there everything I need to know on this page ?
No. Just what i think is mandatory to know/understand for 
embedded software development. No electronics skills are needed
and i don't cover this part yet.

## How is your project build ?
A readme explaining all choices made for each subject/file.
I tell what I'm doing and why. Sometimes (or usually) a lot of other sources to
read, to go further/deeply.

## Is there an order for reading ?
No. Each little project is independent and can works alone.
You have to mix projects in order to see results.

For example:
- Use TIMER to delay leds blink
- Use ITM/UART to display sensor data (I2C)

## Something else ?
If you need more information about a part of this resources, just ask me.
If something in here helped you in some way, let me know :).

Enjoy your embedded development journey :D
