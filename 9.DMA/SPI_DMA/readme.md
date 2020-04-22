#Understanding Direct Memory Access
Here we use DMA to manage data flow from:

	- SPI: l3gd20 gyroscope
	- I2C: lsm303dlhc accelerometer 
	- I2C: lsm303dlhc magnetometer 

# How Direct Memory Access works ?
A nice video from Fastbit Embedded Brain Academy: \
https://www.youtube.com/watch?v=2cdHEbA7FgU&list=PLERTijJOmYrA6iGZaWGyEJXpjzY0lRTC3&index=8

# How to program it ?
## Exploring device documentation
### Datasheet
As usualm we start our journey with the DataSheet. \
A quick llok leads us to 

	page 20
	3.11 Direct memory access (DMA)

We learn 3 important informations:

- We can use it for peripheral to memory transfert
- there are 12 channels
- available for SPI and I2C
	
### User manual
There is no information about DMA in this part.

### Reference manual
Reference Manual

	page 263
	13 Direct memory access controller (DMA)


#### Finding detailled information
## How to code
### Finding the right configuration
### Coding function
