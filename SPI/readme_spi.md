# Understand SPI
https://makbit.com/web/firmware/tutorial-getting-started-with-stm32f3-discovery-board/
SPI 	AF, 50MHz 	PP 	Pull-Up 	Speed is important

In order to turn on the sensor we should write the byte 0x0F to the register CTRL_REG1. After that, let’s read the value of WHO_AM_I register. It should be equal 0xD4.


/* PE0 = INT1
 * PE1 = DRDY/INT2
 * PE3 = CS_I2C/SPI WTF IS THHIS ?
 */
 /* Enable GPIOA
 *
 * PA5 = SCL/SPC		=> clock
 * PA6 = SAO/SDO		=> MOSI
 * PA7 = SDA/SDI/SDO	=> MISO
 * */
Reference Manual page 962 6 30.5.7 Configuration of SPI

In any master receive only mode (RXONLY=1 or BIDIMODE=1 & BIDIOE=0), master starts
to communicate and the clock starts running immediately after SPI is enabled.

master slave : master manage communication

Full duplex bus: receive and emit at the same time
Semi duplux bus: emit or receive only

page 231
Each I/O pin has a multiplexer with up to sixteen alternate function inputs (AF0 to AF15) that
can be configured through the GPIOx_AFRL (for pin 0 to 7) and GPIOx_AFRH (for pin 8 to
15) registers:
• After reset the multiplexer selection is alternate function 0 (AF0). The I/Os are
configured in alternate function mode through GPIOx_MODER register.
• The specific alternate function assignments for each pin are detailed in the device
datasheet.

=> lead to datasheet page 45

/* User Manual page 22/23
/*
 * Enable GPIOA
 *
 * PA5 = SCL/SPC		=> clock
 * PA6 = SAO/SDO		=> MOSI
 * PA7 = SDA/SDI/SDO	=> MISO
 */

/*
 * Enable GPIOE
 *
 * PE0 = INT1
 * PE1 = DRDY/INT2
 * PE3 = CS_I2C/SPI
 */

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


The SPI can communicate in half-duplex mode by setting the BIDIMODE bit in the
SPIx_CR1 register.
The SPI can communicate in simplex mode by setting the SPI in transmit-only or in receive-
only using the RXONLY bit in the SPIx_CR2 register.
Receive-only mode (RXONLY=1):The
clock signal is generated continuously as long as the SPI is enabled. The only way to
stop the clock is to clear the RXONLY bit or the SPE bit and wait until the incoming
pattern from the MISO pin is finished and fills the data buffer structure, depending on its
configuration.

Prior to changing the CPOL/CPHA bits the SPI must be disabled by resetting the SPE bit.
The idle state of SCK must correspond to the polarity selected in the SPIx_CR1 register (by
pulling up SCK if CPOL=1 or pulling down SCK if CPOL=0).

The SPI shift register can be set up to shift out MSB-first or LSB-first, depending on the
value of the LSBFIRST bit. The data frame size is chosen by using the DS bits. It can be set
from 4-bit up to 16-bit length and the setting applies for both transmission and reception.
Whatever the selected data frame size, read access to the FIFO must be aligned with the
FRXTH level. When the SPIx_DR register is accessed, data frames are always right-aligned
into either a byte (if the data fits into a byte) or a half-word (see Figure 356). During
communication, only bits within the data frame are clocked and transferred.

When the master is in any receive only mode, the only way to stop the continuous clock is to
disable the peripheral by SPE=0.
o prevent
having unread data, ensure that RXFIFO is empty when disabling the SPI,
2. Wait until BSY=0 (the last data frame is processed).
3. Read data until FRLVL[1:0] = 00 (read all the received data).


If packing mode is used and an odd number of data frames with a format less than or equal
to 8 bits (fitting into one byte) has to be received, FRXTH must be set when FRLVL[1:0] =
01, in order to generate the RXNE event to read the last odd data frame and to keep good
FIFO pointer alignment.


A specific problem appears if an odd number of such “fit into one byte” data frames must be
handled. On the transmitter side, writing the last data frame of any odd sequence with an 8-
bit access to SPIx_DR is enough. The receiver has to change the Rx_FIFO threshold level
for the last data frame received in the odd sequence of frames in order to generate the
RXNE event.


YOU HAVE TO READ RX EACH TIME TO GET NEW DATA FROM IT

The Chip Select (CS) signal goes low at the start of the transmission and goes high at the end.
When the CS is high the master (MCU) should provide the Serial Port Clock signal (SPC).
The readData() function is a lot like the writeData() function, but there is one important difference. It’s not enough to send the address byte, the MCU should also provide the clock signal in order to read the data from the sensor. To do this we should send a dummy byte (0x00) to the senso

There's one last line you should be aware of, called SS for Slave Select. This tells the slave that it should wake up and receive / send data and is also used when multiple slaves are present to select the one you'd like to talk to.

https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all
