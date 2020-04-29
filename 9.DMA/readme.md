#Understanding Direct Memory Access
Here we use DMA to manage data flow from:

	- SPI: l3gd20 gyroscope
	- I2C: lsm303dlhc accelerometer 
	- I2C: lsm303dlhc magnetometer 

# How Direct Memory Access works ?
A nice video from Fastbit Embedded Brain Academy: \
https://www.youtube.com/watch?v=2cdHEbA7FgU&list=PLERTijJOmYrA6iGZaWGyEJXpjzY0lRTC3&index=8

# How to program it ?
## What we want to do ?
We need to enable DMA to handle data transfert from our Accelerometer, 
magnetometer and gyroscopte to a data array.

Gyro, accelero and magneto send 6 bytes data

	2 bytes for X
	2 bytes for Y
	2 bytes for Z

We need to store it in a simple int8_t array of len 6.
we only need to manage received data from peripherals as soon as they are 
available. And we need to do it continuously.

## Exploring device documentation
### Datasheet
As usual, we start our journey with the DataSheet. \
A quick look leads us to 

	page 20
	3.11 Direct memory access (DMA)

We learn 3 important informations:

- We can use it for peripheral to memory transfert
- there are 12 channels
- available for main peripherals including SPI and I2C
	
### User manual
There is no information about DMA in this part.

### Reference manual
#### Finding detailled information
As usual we start by finding information about DMA.
We end up

	page 263
	13 Direct memory access controller (DMA)

We learn few interesting informations about DMA capacities of our device

- We have 12 channels split in 2 DMA.
- DMA1 has 7 channels and DMA2 has 5, otherwise they are identical.

But we need to find how to configure our DMA

	page 265
	13.4.3 DMA channels
	Channel configuration procedure

Fortunatly for us it's a clear an detailled procedure we can follow easily.

We keep reading and we find an interesting information about DMA1 mapping

	page 272
	Table 78. STM32F303xB/C/D/E, STM32F358xC and STM32F398xE summary of DMA1 
	requests for each channel

And DMA2

	page 275
	Table 80. STM32F303xB/C/D/E, STM32F358xC and STM32F398xE summary of DMA2 
	requests for each channel

For our project we used SPI1 and I2C1 and we want to receive informationm so we
 use 

	DMA1 channel 2 => SPI1_RX
	DMA1 channel 7 => I2C1_RX

And it's enought for the general informations.

## How to code
### Finding the right configuration
1. Set the peripheral register address in the DMA_CPARx register. The data 
will be moved from/ to this address to/ from the memory after the peripheral 
event.

	Add the I2C/SPI Receiver Data Register
	
2. Set the memory address in the DMA_CMARx register. The data will be written to or
read from this memory after the peripheral event.

	Add our array address

3. Configure the total number of data to be transferred in the DMA_CNDTRx register.
After each peripheral event, this value will be decremented.

	We want our DMA to transfert 6 bytes in a row.

4. Configure the channel priority using the PL[1:0] bits in the DMA_CCRx register

	Set it medium priority or whatever we only have 2 DMA channels

5. Configure data transfer direction, circular mode, peripheral & memory incremented
mode, peripheral & memory data size, and interrupt after half and/or full transfer in the
DMA_CCRx register

	Set memory increment mode
	Set Enable circular mode
	Set enable Transfert Complete Interrupt
	All other values are default one

6. Activate the channel by setting the ENABLE bit in the DMA_CCRx register.

	Set Enable the DMA channel

### Coding function
#### DMA1_Channel7_IRQHandler
Here we have to implement our interrupt function to see if it works.
This is mandatory if you use interrupt because you have to clear the interrupt 
flag to resume your program flow. \
Then you can do what you need to. \
Here we just print data.
