#Understanding Direct Memory Access for I2C
We have already configure our DMA1 channel, now we nee dto configure our I2C 
protocol to work with DMA.
# How to program it ?
We use the I2C project and we implement the DMA management.

## Exploring device documentation
### DataSheet
We just learn 

	The I2C interfaces can be served by the DMA controller.

### User manual
Nothing

### Reference manual
Now we need to understand how I2C works with DMA.
Let's read the DMA for I2C part of the reference Manual.
You should find information

	page 865
	28.4.16 DMA requests
	Reception using DMA

It seems to be simple, according to the documentation we just need to setup 
our DMA (already done) and let him manage data read an transfert to our array.
Note that we still have to handle communication process and DMA will just read 
data. So we will ask the peripheral to send us 6 data in a row and not 1 by 1.
It involves some modification to the I2C project.


## How to code
### Finding the right configuration
### Coding function
