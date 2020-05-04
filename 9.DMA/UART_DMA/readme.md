#Understanding Direct Memory Access for UART
We have already configure our DMA1 channel, now we need to configure our UART 
protocol to work with DMA.
# How to program it ?
We use the UART project and we implement the DMA management.

## Exploring device documentation
### DataSheet
For the DMA part

	page 20
	3.11 Direct memory access (DMA)

There is not much information except we can use DMA with USART and we use UART.

And for the UART part

	page 27
	3.21 Universal asynchronous receiver transmitter (UART)

We see that we can't use UART5 with DMA. Remember, USART can do asynchronous or
 synchronous communication an we stick with USART1 for our example. USART1 is 
 compatible with DMA.

### User manual
Nothing

### Reference manual
Now we need to understand how UART works with DMA.
Let's read the DMA for UART part of the reference Manual.
You should find information

	page 921
	29.5.15 USART continuous communication in DMA mode
	Transmission using DMA
	Reception using DMA

It seems to be simple, according to the documentation we just need to setup 
our DMA (already done) and let him manage data read an transfert to our array.
Note that we still have to handle communication process and DMA will just read 
data. So we will ask the peripheral to send us 6 data in a row and not 1 by 1.
It involves some modification to the UART project.

## How to code
### Finding the right configuration
We just set the DMAT bit in the USART_CR3 register.
We just set the DMAR bit in the USART_CR3 register.


### Coding function
In master mode, the initialization, the slave address, direction, number of bytes and
START bit are programmed by software. When all data are transferred using DMA, the
DMA must be initialized before setting the START bit. The end of transfer is managed
with the NBYTES counter.

#### Modifying our Multi_Read data function
We need to send the two frame of the communication protocol while DMA handle the received data and interrupt us when the work is done.
