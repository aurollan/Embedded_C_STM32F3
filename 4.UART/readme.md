# Understand UART
Universal Asynchronous Receiver Transmitter is often the first serial 
communication protocol we learn. And there is a good reason for this:

	- it's a great introduction to serial communication protocol

UART was widely used on computer with the RS-232 COM port but it has been 
replaced by USB in the early 2000s. \
Now we need an external UART to USB converter (see general readme.md) to 
communicate and a software called "minicom" to interact.

# How UART bus works ?
Here is a good explaination about how UART works:\
https://learn.sparkfun.com/tutorials/serial-communication?_ga=2.264250122.426862361.1586869334-952891226.1585558187#uarts

If you read carefully this article you should now be comfortable with the
principle.\
You can refer to this article if you are lost.\
Now let's start the programming part!

# How to programm it ?
## Exploring device peripherals
### Datasheet
Do we have a device with at least an UART bus to use ?\
First step is taking a look at the datasheet.\
 You should end up on page 27 of your datasheet.\
 According to the datasheet we have:

 	- 3 USART (Synchronous/Asynchronous) receiver transmitter
		- USART1
		- USART2
		- USART3

	- 2 UART (Asynchronous only) receiver transmitter
		- UART4
		- UART5

USART can do both so we use any USART bus.

### User Manual
We need to know which GPIO is used to manage communication.\
Let's check the User Manual for Extension connectors.\
You should find the following information for the UART1 bus:

User Manual page 23-24:

	- PA8 => USART_CK
	- PA9 => USART_TX
	- PA10 => USART_RX
	- PA11 => USART_CTS
	- PA12 => USART_RTS

You should find those pins too:

	- PB6 => USART1_TX
	- PB7 => USART1_RX

But we don't use them because of the LSM303DLHC needs.

And again:

	- PC4 => USART1_TX
	- PC5 => USART1_RX

But we don't use them because of the VCP needs.

We take the USART 1 bus, USART can do both so we won't get any trouble.\
Of course you can use anyone you want to and. If you have 3 other 
peripheral that need an USART for synchronous communication you should use 
UART4 or UART5 but here we don't. 

## Configuring device peripherals
### Reference Manual
#### Undertsanding what you have to do
Before coding anything we need to understand how UART communication is 
implemented in our device.\
This is were the Reference Manual is mandatory. \
There is a lot to read for USART communication and the best way to enjoy our 
journey in the Reference Manual is knowing what we are looking for.\
 You will be able to implement anything you want if you 
 understand how to look for information.\
We want to implement a simple Asynchronous communication without any particular
 standart or feature.\
If you check the Reference Manual for USART, you should end up 

	page 885 
	"29 Universal synchronous asynchronous receiver transmitter (USART)"

Even if we are not going to read all the documentation but at least the general
 subject:

 	- 29.1 Introduction
	- 29.2 USART main features
	- 29.3 USART extended features
	- 29.4 USART implementation 
	- 29.5 USART functional description => what we are looking for

Now we just have to retrieve information we need.\
Remember for now we don't need particular feature or standart, we will stick 
with what we have seen in the link in the "How UART bus works ?" section.

If you have fully understand what we need you should be able to implement an 
UART communication protocol by yourself from that point. If you don't it's ok 
just continue reading.

#### Undertsanding what you need
According to 

	page 888
	29.5 USART functional description

We need:

	- 2 pins for Transmission/Reception
		- provided by the device.
	- use the right frame configuration
		- our part

We don't use:

	- Smartcard modem 
	- synchronous mode 
	- RS232 Hardware flow control mode
	- RS485 Hardware control mode


#### Finding the right configuration
If you read carefully the link in the "How UART bus works ?" section, you
 should already know how a frame is composed, but to be clear I will repeat it:

	- Data bits
	- Synchronization bits
	- Parity bits

And we need a baudrate to transmit data.

Let's dive in and find what we need to configure our UART protocol.

	29.5.1 USART character description
	29.5.9 USART parity control
	29.5.4 USART baud rate generation

	29.5.2 USART transmitter
	29.5.3 USART receiver

##### Character description
We use a world length of 8 bits to get 1 byte at a time. It's easier to deal 
with in our code because it fills a 'char' or an 'uint8_t'.\
By default the signal is in low state during the start bit and in hight state 
during the stop bit. This is what we want because this is the case covered in
the "How UART bus works ?" section. We will just make sure in our code that we 
get the default setup.\
Baudrate is common for transmission and reception.

##### Parity control
The parity bit is used for error checking. This is an important point when you 
build a robust software but we want to get this work first and it's not 
needed to reach our goal.
We make sur that parity bit is not added.

##### Baud rate 
We need to define a baudrate for data transmission.
According to the default "src/system_stm32f3xx.c" file, in our 
device the clock source is HSI and it is set to 8 Mhz.
There is an example in the Reference Manual and we use this one in the code.
But you need to understand how it works.

##### Transmitter/Receiver
Those part are well explained and you can use the suggested configuration flow.

Now you should be able to build the UART communication protocol you need.

If you can't that's ok, just follow the last part.

## How to code
We use a simple software flow to code this:

	- Enable peripheral
	- Configure GPIO
	- Configure GPIO Alternate Function
	- Configure UART
	- Build a transmit function
	- Build a receive function
	- Test

It's maybe not the best way but for now it's enought.

### Enable peripherals
First we need to enable clock for peripheral we use.
Here we use USART1 and GPIOA.

#### Enable GPIO and USART1
Enabling peripherals clocks should be easy for you if you started with some 
simpler projects like blinking leds. But if it's not please refer to
"readme_understand_memory_map.md".

#### Do we need to enable all those GPIO pins ?
	- PA8 => USART_CK - for synchronous communication
	- PA9 => USART_TX
	- PA10 => USART_RX
	- PA11 => USART_CTS - for RS232 standart
	- PA12 => USART_RTS - for RS232 standart
For our project we just need PA9 and PA10 because we are setting up an 
asynchronous communication without RS232 or RS485 standart.

### Setting up peripherals
#### Setup GPIOA pins
According to the Datasheet

	page 91
	Input/output AC characteristics

GPIO_medium speed (0Mhz to 10Mhz) is enought for both pins since our system 
clock run to 8 Mhz.

Transmitting pin-ouput pin\
GPIO should be: pull up push down and pull up.\
because it drive the device and when there is no data the line is up

Receiving pin-input pin\
GPIO should be open drain because another device drive the communication\
Both pin use Alternate Function mode
according to Datasheet:

	page 45
	Table 14. Alternate functions for port A

The alternate function used is AF7 for transmiter and receiver.

#### Setup USART1
software flow:

	- 8 bits data => make sur data length is 8 bits
	- configure baudrate diviser
	- configure number of stop bits (1)
	- No parity control => make sure parity control is cleared
	- configure oversampling => make sure you set your own value
	- enable reception => make sure it's enabled
	- enable reception => make sure it's enabled
	- enable USART

With this information you just have to set the bits correctly in the 
appropriate register.
It's easy to find but if you can't just check 

	page 929
	29.8 USART registers

You should find the appropriate register to modify, if you can't, please refer 
to the code in the "src" repository.

### Coding communication function
#### Build transmit function
Transmitting data using UART is simple.
We focus on a single byte communication for now.

If you read carefully 

	page 888
	29.5 USART functional description

You should have notice that there is a TDR (Transmit Data Register) and a 
status register.

Then read

	page 893
	29.5.2 USART transmitter
	Single byte communication

Now it should be obvious how to send data.\
You have to fill the buffer then wait the TXE bit is set.\
When the TXE bit is set by the hardwarem your data has been transmited.

If you get stuck read again carefully, or check the code in the "src" 
repository.
#### Build receive function
Receiving data using UART is simple.
We focus on a single byte communication for now.

If you read carefully 

	page 888
	29.5 USART functional description

You should have notice that there is a RDR (Transmit Data Register) and a 
status register.

Then read

	page 894
	29.5.3 USART receiver
	Single byte communication

Now it should be obvious how to receive data.\
You have to fill the buffer then wait the RXNE bit is set.\
When the RXNE bit is set by the hardwarem data are received in the appropriate 
register.

If you get stuck read again carefully, or check the code in the "src" 
repository.

## Communication woth your computer
download minicom.
using the FTDI connector
wiring your micricontroller
enjoy
