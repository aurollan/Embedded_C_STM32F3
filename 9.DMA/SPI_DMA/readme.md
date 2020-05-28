#Understanding Direct Memory Access for SPI
We have already configure our DMA1 channel, now we need to configure our SPI 
protocol to work with DMA.
# How to program it ?
We use the SPI project and we implement the DMA management.

## Exploring device documentation
### DataSheet
We just learn 

	page 20
	3.11 Direct memory access (DMA)
	The DMA can be used with the main peripherals: SPI, I 2 C, USART, 
	general-purpose timers, DAC and ADC.

### User manual
Nothing

### Reference manual
Now we need to understand how SPI works with DMA.
Let's read the DMA for SPI part of the reference Manual.
You should find information

	page 966
	Communication using DMA (direct memory addressing)

We find the way to start communication using DMA.


### LSM303DLHC DataSheet
No information about DMA.

## How to code
### Finding the right configuration
We just set the RXDMAEN bit in the I2C_CR1.


### Coding function
In master mode, the initialization, the slave address, direction, number of bytes and
START bit are programmed by software. When all data are transferred using DMA, the
DMA must be initialized before setting the START bit. The end of transfer is managed
with the NBYTES counter.

#### Modifying our Multi_Read data function
We need to send the two frame of the communication protocol while DMA handle the received data and interrupt us when the work is done.



