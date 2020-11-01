# TOOL CHAIN
Working in embedded development require a specific toolchain.\
There is a lot of tools we can use and we work with the following:

- gcc-arm (Compiler for ARM arch)
- openocd (flashing and debugging)
- minicom (UART)
- itmdump (ITM)

## arm-none-eabi
 The toolchain used to build our executable. 

### Why arm-none-eabi-gcc ?
- arm: target architecture
- none: no vendor
- No OS is specified (bare metal programming)
- eabi: complies embedded-application binary interface
- gcc: call the compiler in this case

##### Why eabi is specified ?
Because we target an Embedded abi we want to use this specific optimized binary
interface for our project.\
Reference: https://en.wikipedia.org/wiki/Application_binary_interface

##### How is the command line build ?
The name is build with a target triplet convention not only used in embedded 
development.\
Reference: https://wiki.osdev.org/Target_Triplet

## arm-none-eabi-gdb (MacOsX) / gdb-multiarch (Linux)
Simply the gdb ARM debugger. We use it to load executable on the device through
openocd.

### Options

#### -q
-quiet
-q  "Quiet".  Do not print the introductory and copyright messages.
These messages are also suppressed in batch mode.

#### -x file.gdb
-x file
Execute GDB commands from file.

##### Why use a file ?
To avoid writing manually those commands each time we launch a debug session.

##### Why use those gdb command ?
- target remote: 3333\
3333 is the ports we use to communicate with openocd when it is connected to
 our device.  We debug and load executable using openocd bridge, so we have to 
 connect to it.

- load\
Command to load/flash our executable in the device flash memory.

- break main\
set a breakpoint at the main function entry.

###### Following command are used only for ITM configuration
- monitor tpiu config internal itm.txt uart off 72000000\
	- monitor:
	- tpiu:
	- config:
	- internal:
	- uart:
	- off:
	- itm.txt: file where data will be written
	- 720000000: device clock
	- TODO: Finish

- monitor itm port 0
	- Use port 0 as we use this one to send our data in our ITM configuration

- continue\
Just start the program which should stop at main given our previous breakpoint.

## openocd
Software used to debug and flash.

## minicom
Software used to get UART data from device.

## itmdump
Software used to get ITM data from device.
