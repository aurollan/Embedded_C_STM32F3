# Understanding I2C
Here is a good explaination about how SPI works:
https://learn.sparkfun.com/tutorials/i2c?_ga=2.195885420.157008010.1585558187-952891226.1585558187

If you read carefully this article you should now be comfortable with the
principle. You can refer to this article if you are lost.
Now let's start the programming part!

# How to program it ?
## What are we looking for ?
We are looking for informations to configure our I2C communication bus.We need 
to configure (write) and get data (read) from our LS303HLDC sensor.
We manage the communication (Master).

## Exploring device documentation
### User manual
First of all, let's take a look at the User Manual. \
No information about I2C but some information abour our sensor

	page 19
	6.7 ST MEMS E-compass (ST MEMS LSM303DLHC)

We have no choice to use I2C to communicate with the sensor.
We have to find which I2C "channel" is used by the sensor.

	page 22
	6.12 Extension connectors

According to the User Manual

GPIOB
- PB6 => SCL
- PB7 => SDA

GPIOE
- PE0 => INT1
- PE1 => DRDY/INT2
- PE2 => DRDY
- PE4 => INT1
- PE5 => INT2


### DataSheet
Do we have a I2C bus inteface in our device ?
According to the DataSheet, yes. We have 2 bus interfaces with various 
operation mode and configuration possibilities.
As for UART we need to use our GPIOx as alternate function to manage I2C communication.

	page 47
	Table 15. Alternate functions for port B

For I2C and GPIOB corresponding alternate function is AF4.
We need to configure GPIOB register accordingly. This is a part we have done many time before so let's jump on the new part.

### Reference manual
### Finding the right informations
I2C documentation is

	page 816
	28 Inter-integrated circuit (I2C) interface

There is a lot of documentation, but we will focus on our goals:

	Configure I2C
	Configure Sensor
	Retrieve data from Sensor

We focus on master(our device)-slave(the sensor) communication.

Take your time and read

	page 819
	28.4.3 Mode selection

It's important to understand that the communication is managed by the software.
 We start communication by stting the start bit and end it by setting the stop 
 bit.

Let's start with the initialization part.

#### How to initialize I2C

	page 821
	28.4.4 I2C initialization

Here there are important information.
Don;4t forget to enable the I2C clock.
Software reset by clearing the PE bit.
I2C is enabled zhen PE bit is set.

I will pass on noise filterm just keeping the analog filter as suggestion by 
default.

And here is my nemesis

	page 822
	I2C timings

I2C timing configuration, if you have an oscilloscope or if you want to 
understand this part, I encourage you. But for now I pass, don't worry they 
give us the right configuration for our device in another part of the 
documentation.

Anf finally

	page 825
	Figure 295. I2C initialization flowchart

This is what we are looking for.
Now let's focus the communication flow.

#### How to transmit/receive data
We are lucky with this onem it's on the next page

	page 826
	28.4.6 Data transfer

##### Reception
We use the I2C_RXDR to receive data.
We learn two important things:

- We have to wait the RXNE bit to be set (Not Empty=yes => there is something 
to read)
- Then we have to read the I2C_RXDR register to get the data.

##### transmission
We use the I2C_TXDR to transmit data.
We learn two important things:

- We have to wait the TXE bit to be set (Empty=yes => previous data has been 
send, we can write data to the register)
- Then we have to read the I2C_TXDR register to get the data.

##### Hardware transfert
There is a nice feature with the I2C

	page 827
	Hardware transfer management

The hardware can count how many bytes we transfert and stop when it's done. 
We will use this feature later so take your time to read this part.

Now there is still 2 important part to understand.
- I2C master mode transmitter
- I2C master mode receiver


#### I2C master mode
I2C use a specific communication flow we have to follow if we want to get our 
data.
Here we focus on the Master-slave communication flow as the master.
You should find

	page 837
	I2C master mode

Here again a bunch of informations about clock, as I said, we use the value 
given by our Reference Manual later. You could customize it later.
Scroll down to 

	page 839
	Master communication initialization (address phase)

This is an IMPORTANT part, read it carefully this is what we use for the 
coding part. And the `Note` too.
We don't use 10 bits addressing for now so we skip this part.
Let focus on the 2 last important parts before coding

- Master Transmitter
- Master Receiver

##### Master Transmitter (write)
We have to know how to implement transmission function because before using 
our peripheral we have to configure it.

	page 841
	Master transmitter

Two important things here:

- We stop the communication with setting the STOP bit when the Transfert 
Complete flag is set
- Hardware send auto stop after bytes are transfered if AUTOEND bit is set

We find the transfert sequence flowchart

	page 842
	Figure 309. Transfer sequence flowchart for I2C master transmitter for 
	N≤255 bytes


##### Master Receiver (read)
This is how we receive data from the peripheral.
We have the same information but clearer.
Here we undertsand that when byte is receive the RXNE bit is set until we read 
it.

We also have a communication flowchart

	page 846
	Figure 312. Transfer sequence flowchart for I2C master receiver for N≤255 bytes


##### Master TIMING
And finally we find how to configure the timing

	page 849
	28.4.9 I2C_TIMINGR register configuration examples
	Table 147. Examples of timings settings for f I2CCLK = 8 MHz

We have gatehered all informations we need to code. From here you should be able to code everything. If notm just read below how to do it.


### LSM303DLHC DataSheet
When we use a sensor, we need to check its documentation to.
We need to understand how to configure it and how we can communicate.
Here we know we need to communicate with I2Cm so we have to retrieve some information. Looking for information about I2C we end up

	page 19
	5.1 I2C serial interface

There is more detailled informations about I2C communication. 


## How to code
### Coding function
### Finding the right configuration
#### GPIOB
GPIOB MODER => Alternate function
GPIOB TYPER => OUtput Push Pull
GPIOB SPEED => Medium ( < 10Mhz)
GPIOB PUPDR => Pull-up (hight by default according to the bus diagram we saw earlier)

#### I2C
We have to code 3 function here:
- I2C_init
- I2C_write
- I2C_read

##### I2C_init
We already have gathered all information we need. \
Just a quick remember

	page 821
	28.4.4 I2C initialization

	page 825
	Figure 295. I2C initialization flowchart

	page 849
	28.4.9 I2C_TIMINGR register configuration examples
	Table 147. Examples of timings settings for f I2CCLK = 8 MHz


Everything is explained, just rigourously follow the steps.
For the example code i chosed to keep default value for ANOFF, DNF and NOSTRETCH

##### I2C_write
Same as pevious section, quick remember


	page 841
	Master transmitter

	page 842
	Figure 309. Transfer sequence flowchart for I2C master transmitter for 
	N≤255 bytes

Same as previous, just follow rigourously the flowchart.

##### I2C_read
Same as pevious section, quick remember


	page 845
	Master transmitter

	page 846
	Figure 312. Transfer sequence flowchart for I2C master receiver for N≤255 bytes

Same as previous, just follow rigourously the flowchart.

##### How to make them work
This is were it become more difficult. \
An I2C communication is always composed of 2 frame.
For now I advise you to use byte per byte (NBYTE = 1) transmission.
/!\ Important note: configure the CR2 register in a single assignation /!\

###### Write to peripheral memory

I2C_write
	- You ask to write to the peripheral addresses
	- You store register address in the TXDR

I2C_write
	- You ask to write to the peripheral addresses
	- You store the register configuration in the TDR

###### Read to peripheral memory

I2C_write
	- You ask to write to the peripheral addresses
	- You store register address in the TXDR

I2C_read
	- You ask to read from the peripheral addresses
	- You read the data given by previous register in the RXDR

###### Way to code it
There is a lot of way to code itmm the flowchart describe in the Reference Manual is not the only way. In my source code I have used slightly different way.
If you have an oscilloscope, it's easy to see what is wrong, but if you don't I encourage you to use debug information like in my example.

With this information you should be able to get the WHOAMI value for example.

#### LSM303DLHC


#### other ressources
After the START condition (ST) a slave address is sent, once a
slave acknowledge (SAK) has been returned, an 8-bit sub-address (SUB) is transmitted; the
7 LSBs represent the actual register address while the MSB enables address autoincrement. If the MSB of the SUB field is ‘1’, the SUB (register address) is automatically
increased to allow multiple data Read/Write.

It is also necessary to connect the power supply through a pull-up resistor. When the bus is idle, both lines on high power level.
https://www.seeedstudio.com/blog/2019/09/25/uart-vs-i2c-vs-spi-communication-protocols-and-uses/

A really good source:
https://aticleworld.com/i2c-bus-protocol-and-interface/


https://makbit.com/web/firmware/tutorial-getting-started-with-stm32f3-discovery-board/
I2C 	AF, 50MHz 	Open Drain 	NoPull 	Open Drain mode, pull-up resistors are always required
