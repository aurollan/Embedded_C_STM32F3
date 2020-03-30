# Understand SPI
# How SPI bus works ?
Here is a good explaination about how SPI works:
https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all

If you read carefully this article you should now be comfortable with the
principle. You can refer to this article if you are lost.
Now let's start the programming part!

## How to programm it ?
### Enable peripherals
First of all let's check the User Manual for Extension connectors.
You should find the following information for the L3GD20 gyroscope:

User Manual page 22/23
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
Enable GPIOA, GPIOE and SPI1 clock.
Enabling peripherals clocks should be easy for you if you started with some 
simpler projects like blinking leds. But if it's not please refer to
"readme_understand_memory_map.md".

#### Setup GPIOA and GPIOE pins
Enabling GPIOx Pins should be easy for you if you started with some 
simpler projects like blinking leds. But if it's not please refer to
"readme_understand_memory_map.md".

##### Setup GPIOE CS.
##### Setup GPIOA AF

### Read from L3GD20 register
Jump on L3GD20 gyroscope datasheet.
You should find a the content table you should find an SPI_read section.

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

