# Understand SPI
## How SPI bus works ?
Here is a good explaination about how SPI works:
https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all

If you read carefully this article you should now be comfortable with the
principle. You can refer to this article if you are lost.
Now let's start the programming part!

## How to programm it ?
## What we want to do ?
We want to retrieve data from our sensor.
Here we use a three-axis digital MEMS motion sensor called L3GD20 )gyroscope).
This sensor can send and recive data throught SPI or I2C bus but our device is 
setup to use SPI communication bus with this sensor, we don't have choice.
For this part we focus on getting raw data from sensor and print them. No more.
We want a full duplex communication and we manage communication as Master with only one slave.
The readme.md is divided as follow:
	- Find detailled informations we need in documentation
	- How to use those information to build our firmware

If you still have trouble to make your project work, you can refer to code.

## Finding detailled information
### Exploring device documentation
We explore the documentation in the following order:
1.	Device User Manual
2.	Device DataSheet
3.	Device Reference Manual
4.	Sensor DataSheet

We have to find:
- How our embedded sensor is setup
- Which GPIO we need to communicate
- How to configure those GPIO
- How to initialize our SPI
- How to send and receive data

### Device User Manual
Let's dive in the most user friendly source of information we have.
A quick look to its contents and we find our gyroscope.

	page 19
	6.8 ST MEMS Gyroscope (ST MEMS L3GD20)

We see that we have to use SPI to communicate with this sensor.
No choice left for the STM32F3 discoveyr but if you use a sensorm you could 
have the choice between I2C and SPI. \
User Manual provide a usefull source of information known as `Extension connectors`.

	page 22
	6.12 Extension connectors

We find our L3GD20 sensor and pins we have to use to communicate.
We need those information to configure the right GPIO. \
That's all for the User Manual, we find the GPIO pin we have to use, next 
step is the DataSheet.

### Device Datasheet
With a quick look to the DataSheet contents we find SPI/I2S information

	page 28
	3.22 Serial peripheral interface (SPI)/Inter-integrated 
	sound interfaces (I2S)

We see that we have 3 SPI and some other technical information.
We find nothing about our sensorm but we need a useful information to 
configure our GPIO PIN in SPI mode (Alternate Function).
Let's take a look a the pin description

	page 32
	4 Pinouts and pin description

We find information about pin definition that we already get in the User Manual 
and we also find

	page 45
	Table 14. Alternate functions for port A

There is all information about GPIO pin alernate functions.
It's enought for the DataSheet. We find our GPIO pin configuration.

### Device Reference manual
This is a big part, in the Reference Manual we find:
- How to initialize SPI
- How to handle communication

Following Reference Manual contents section we find SPI information

	page 952
	30 Serial peripheral interface / inter-IC sound (SPI/I2S)

There is two part but the one we are looking for is obviously the SPI part, I2S 
information are useless for us.
Basically all information are important you should read it, there is information we need about SPI initialization

	page 962
	30.5.7 Configuration of SPI

and some information you may need if you code the firmware by yourself

	page 965
	Data packing

But we need more information to code our SPI firmware and obviously Reference 
Manual is not detailled enought.
So let's jump on the sensor DataSheet.

### L3GD20 DataSheet
Here we find the final piece of information we need.
There is two part concerning SPI in this documentation.

	page 10
	2.4.1 SPI - serial peripheral interface

and

	page 24
	5.2 SPI bus interface

This is what we are looking for. We have the read and write protocol and right under it

	page 29
	6 Output register mapping

All the register adresses we need to read and configure our sensor.
Now we know:
- How to configure GPIO
- How to initialize SPI
- How to communicate (read/write) with our sensor
Now you have everything you need to code your own firmware.
If you have trouble to make it work, you can follow next section.

## How to code
We find everything we need to code, now let's put it in a software flow.
1. Enable our peripheral (GPIOx, SPIx)
2. Configure GPIOx
3. Configure SPIx
5. Code receive (read) function
4. Code transmit (write) function
6. Setup our sensor
7. Read and print raw sensor data

## Finding the right configuration
### Enable peripherals
According to

	User Manual
	page 22
	6.12 Extension connectors

You should find the following information for the L3GD20 gyroscope:


PA5 = SCL/SPC		=> CLOCK
PA6 = SAO/SDO		=> MOSI
PA7 = SDA/SDI/SDO	=> MISO

PE0 = INT1			=> INTERRUPT 1
PE1 = DRDY/INT2		=> DATA READY/INTERRUPT 2
PE3 = CS_I2C/SPI	=> CHIP_SELECT

#### Do we need all those GPIO pins ?
In this example we keep full-duplex communication so we have to enable MOSI and
MISO pins. Then as the master, we handle communication so we have to provide
clock. Finally we will use slave software management so we have to enable the
CHIP_SELECT pin.
We won't use interrupt for this example so PE0 and PE1 won't be initialized or
used..

#### Enable GPIO and SPI
We have to enable GPIOA, GPIOE clock to use them.
According to 

	DataSheet
	page 45
	Table 14. Alternate functions for port A

We should enable SPI1 clock to handle communication.

Enabling peripherals clocks should be easy for you if you started with some 
simpler projects like blinking leds. But if it's not please refer to
"readme_understand_memory_map.md".

#### Configure GPIOA and GPIOE pins
We have 2 GPIO:
- GPIOA
- GPIOE

And four pins used:
- PA5
- PA6
- PA7
- PE3

##### Setup GPIOA AF
PA5/6/4 are used as Alternate Function SPI1.
According to 

	Device DataSheet
	page 45
	Table 14. Alternate functions for port A

SPI1 alternate function is AF4.
So we configure our GPIOA as follow:
- MODE: AF
- AFR: AF4
- SPEED: medium (< 10Mhz)
- OTYPER: output push pull
- PUPDR: No PUPD

##### Setup GPIOE CS.
PE3 is used as chip select.
We use software to address our request to a specific peripheral (sensor).
We only need two state:
- we set chip select low to enable a communication with a selected peripheral
- we set chip select hight to close a communication with a selected peripheral 
Here we only have one SPI sensor peripheral with chip select to PE3.
We will set it up as a simple output GPIO we will drive hight or low.

So we configure our PE3 as follow:
- MODE: Output mode
- AFR: AF4
- SPEED: medium (< 10Mhz)
- OTYPER: output push pull
- PUPDR: PULL UP (by default a line is not used: high)


### Configure SPI
To initialize our SPI1 we use the procedure given by

	Reference Manual
	page 962
	30.5.7 Configuration of SPI

We used the following configuration:

1.  Already done in previous sections
2.  a) Keep default value 0x00 because our clock is 8 Mhz but if you have a 
clock > 10 Mhz you should change this value to get < 10MHz.
	b) Keep default value CPOL = 0 and CPHA = 0 used by our sensor
	c) Use full duplex mode
	d) Keep default value for now
	e) CRC is not used
	f) Software slave management and Internal slave select
	g) Select Master mode

3.  Write to SPI_CR2 register:
	a) 8 bits (1 byte)
	b) Set thsi bit if you use a multimaster environement (but we don't)
	c) Keep Motorola mode by default
	d) Keep to default 0x00 we need to make more than a single data transfert
	e) We want to tranfert byte per byte sot set this bit to 0x1
	f) We don't use DMA for nowm let those bits clear
4. Not needed for now
5. Not needed for now
6. Enable SPI when configuration is done

### Read from L3GD20 register
We use the following information to code our function:

	L3GD20 gyroscope DataSheet.
	page 24
	5.2 SPI bus interface

1. First step we choose Sofware slave management using the CHIP SELECT GPIOE pin, so we have to tell the peripheral that we start a communication putting the GPIOE PE3 output low.

2. We wait the transmit buffer to be empty by polling the TXE bit in the Status 
Register.

3. we set the read bit in our data
3. we write our data (peripheral register address) to the Data Register

2. We wait the receive buffer to be full by polling the RXNE bit in the Status 
Register.

4. We read received data to clean the receiver buffer (GARBARGE DATA)

5. We wait the transmit buffer to be empty by polling the TXE bit in the Status 
Register.

6. we write our data (DUMMY BYTE) to the Data Register

7. We wait the receive buffer to be full by polling the RXNE bit in the Status 
Register.

8. We read received data and it is the value of the register we asked for

### Get WHO_AM_I value
Jump on L3GD20 gyroscope datasheet.
You should find a the content table you should find an Register description
section and a WHO_AM_I register. It's not important to get the value from this
register and we don't need it but it's a good way to check if our read function
 works.
It should return 0xD4.

### Write to L3GD20 register
Jump on L3GD20 gyroscope datasheet.
You should find a the content table you should find an SPI_write section.

### Configure L3DG20
In order to turn on the sensor we should write the byte 0x0F to the register CTRL_REG1.

### Get real-time data from gyro



SPI 	AF, 50MHz 	PP 	Pull-Up 	Speed is important

Reference Manual page 962 6 30.5.7 Configuration of SPI

master-slave : master manage communication

Full duplex bus: receive and emit at the same time

page 231
Each I/O pin has a multiplexer with up to sixteen alternate function inputs (AF0 to AF15) that
can be configured through the GPIOx_AFRL (for pin 0 to 7) and GPIOx_AFRH (for pin 8 to
15) registers:
• After reset the multiplexer selection is alternate function 0 (AF0). The I/Os are
configured in alternate function mode through GPIOx_MODER register.
• The specific alternate function assignments for each pin are detailed in the device
datasheet.

=> lead to datasheet page 45


- MISO: Master In / Slave Out data. In the general case, this pin is used to transmit data
in slave mode and receive data in master mode.
• MOSI: Master Out / Slave In data. In the general case, this pin is used to transmit data
in master mode and receive data in slave mode.
• SCK: Serial Clock output pin for SPI masters and input pin for SPI slaves.
• NSS: Slave select pin. Depending on the SPI and NSS settings, this pin can be used to
either:
– select an individual slave device for communication
– synchronize the data frame or
– detect a conflict between multiple masters


The SPI shift register can be set up to shift out MSB-first or LSB-first, depending on the
value of the LSBFIRST bit. The data frame size is chosen by using the DS bits. It can be set
from 4-bit up to 16-bit length and the setting applies for both transmission and reception.
Whatever the selected data frame size, read access to the FIFO must be aligned with the
FRXTH level. When the SPIx_DR register is accessed, data frames are always right-aligned
into either a byte (if the data fits into a byte) or a half-word (see Figure 356). During
communication, only bits within the data frame are clocked and transferred.




YOU HAVE TO READ RX EACH TIME TO GET NEW DATA FROM IT

The Chip Select (CS) signal goes low at the start of the transmission and goes high at the end.
When the CS is high the master (MCU) should provide the Serial Port Clock signal (SPC).
The readData() function is a lot like the writeData() function, but there is one important difference. It’s not enough to send the address byte, the MCU should also provide the clock signal in order to read the data from the sensor. To do this we should send a dummy byte (0x00) to the senso

There's one last line you should be aware of, called SS for Slave Select. This tells the slave that it should wake up and receive / send data and is also used when multiple slaves are present to select the one you'd like to talk to.

### Coding function
