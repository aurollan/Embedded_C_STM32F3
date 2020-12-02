DISCLAIMER: this is a much harder part for me and I still don't understand
everything. For I just tell what I'm using. I will come back later on this 
part to master it.
# Flashing your device
Now that we have compiled our program, we need to load it on our device.

## What you can use
### Software
First of all there IS a software in your device "waiting" you to flash
your program. This is called a bootloader.
This software in your microcontroller will write in memory 
data you send throught a specific communication bus: UART/I2C/SPI/CAN.
Note that you may have to use a particular protocole to communicate with 
the bootloader.


### Hardware
According to your communication bus you will need an interface between your 
flash tool and your microcontroller, like simple wires or an FTDI for example.
You may have to setup some "boot pin" on your device in order to make it works.

You can flash your microcontroller this way and you will find a lot of 
tutorial on it. But it's not covered in this one.

## What we use
### Software
We use another tool named OpenOCD.
As it name suggest it it is an On Chip Debugger that we will use for debugging 
purpose, but also for flashing. This software will handle everything for us if 
we gave it accurate informations.
It can handle JTAG and SWI interface, and we will use SWI.

### Hardware
# Debugging your device
For this tutorial I use a simple st-link V2, there is only 4 wire to connect 
and we can find it easily. This is perfect.
