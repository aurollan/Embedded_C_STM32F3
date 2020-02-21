# Understanding I2C
# Ressource needed
https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf


## I2C Initialization flowchart (RTFM 825/1141 Figure 295)
=> Clear PE bit in I2C_CR1
=> Configure ANOFF and DNF (Analog noise filter and Digital Noise Filter)
=> Configure PRESC/SDADEL/SCLDEL/SCLH/SCLL in I2C_TIMIGR
=> Configure NOSTRETCH in I2C_CR1
=> Sel PE bit in I2C_CR1
=> end


## I2C Data transfert (RTFM 826/1141)
### Reception
if RXNE == 0
	=> Data are copied to I2C_RXDR 
else
	=> Previous received data has not yet been read

### Transmission
if TXE == 0
	=> data are copied to shift register
else
	=> no data is written yet to I2C_TXDR


### I2C Hardware transfert management
Way to close communication:
=> NACK, STOP, ReSTART in master mode
=> ACK control in slave receiver mode
=> PEC generation/checking when SMBus feature is supported

Number of bytes to be transfered: I2C_CR2 NBYTES[7:0] bit field
How to end after n bytes are transfered:
=> AUTOEND == 1 bit field in I2C_CR2 
	automatically send STOP when NBYTES are transfered
=> NO AUTOEND == 0 bit field in I2C_CR2
	I2C_ISR_TCR is set when Transfert Complete R? TCR is cleared by software when NBYTES is written to a non-zero value

## I2C master mode
=> The master clock must be configured by setting SCLH and SCLL in I2C_TIMINGR register
NEED CALCUL FOR CLOCK

### Master communication initialization
MUST program I2C_CR2:
=> adressing mode (7bits or 10 bits)
=> Slave adress to be send: SADD[9:0]
=> Transfert direction: read == 1, write == 0
=> Number of bytes to be transfered: NBYTES[7:0]

=> set START bit in I2C_CR2 all above can't be changed after this

=> Wait bus is free BUSY == 0 

=> start bit is reset after adress has been send

### Master transmitter
the TXIS flag is set after each byte transmission
TXIS generate an event if TXIE is set in I2C_CR1.
TXIS is cleared when I2C_TXDR register is written

the number of event TXIS correspond to NBYTES value

The TXIS flag is not set when a NACK is received.
• When RELOAD=0 and NBYTES data have been transferred:
	– In automatic end mode (AUTOEND=1), a STOP is automatically sent.
	– In software end mode (AUTOEND=0), the TC flag is set and the SCL line is
	stretched low in order to perform software actions:

	A RESTART condition can be requested by setting the START bit in the I2C_CR2
	register with the proper slave address configuration, and number of bytes to be
	transferred. Setting the START bit clears the TC flag and the START condition is
	sent on the bus.

	A STOP condition can be requested by setting the STOP bit in the I2C_CR2
	register. Setting the STOP bit clears the TC flag and the STOP condition is sent on
	the bus

If a NACK is received: the TXIS flag is not set, and a STOP condition is automatically
sent after the NACK reception. the NACKF flag is set in the I2C_ISR register, and an
interrupt is generated if the NACKIE bit is set.


Master receiver
In the case of a read transfer, the RXNE flag is set after each byte reception, after the 8th
SCL pulse. An RXNE event generates an interrupt if the RXIE bit is set in the I2C_CR1
register. The flag is cleared when I2C_RXDR is read.
If the total number of data bytes to be received is greater than 255, reload mode must be
selected by setting the RELOAD bit in the I2C_CR2 register. In this case, when
NBYTES[7:0] data have been transferred, the TCR flag is set and the SCL line is stretched
low until NBYTES[7:0] is written to a non-zero value.
• When RELOAD=0 and NBYTES[7:0] data have been transferred:
– In automatic end mode (AUTOEND=1), a NACK and a STOP are automatically
sent after the last received byte.
– In software end mode (AUTOEND=0), a NACK is automatically sent after the last
received byte, the TC flag is set and the SCL line is stretched low in order to allow
software actions:
A RESTART condition can be requested by setting the START bit in the I2C_CR2
register with the proper slave address configuration, and number of bytes to be
transferred. Setting the START bit clears the TC flag and the START condition,
followed by slave address, are sent on the bus.
A STOP condition can be requested by setting the STOP bit in the I2C_CR2
register. Setting the STOP bit clears the TC flag and the STOP condition is sent on
the bus.


