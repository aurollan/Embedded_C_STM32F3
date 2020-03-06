# TOOL CHAIN

Working in embedded development require a specific tool chain:

- gcc-arm-embedded (Compiling)
- openocd (loading and debugging)
- minicom (USART )
- itmdump

# gcc-arm-embedded

Tool chain used to build our executable and debug.
### arm-none-eabi-gcc
- arm: target architecture
- none: no vendor
- No OS is specified (bare metal programming)
- eabi: complies embedded-application binary interface
- gcc: call the compiler in this case

##### Why eabi is specified ?
https://en.wikipedia.org/wiki/Application_binary_interface

##### How is the command line build ?
https://stackoverflow.com/questions/5961701/arm-gcc-toolchain-as-arm-elf-or-arm-none-eabi-what-is-the-difference

https://community.arm.com/developer/tools-software/tools/f/arm-compilers-forum/44065/what-library-used-on-arm-gcc-none-eabi


#arm-none-eabi-gdb
Simply the gdb ARM debugger. We use it to load executable on the device through
openocd.

## Commands
### -q
       -quiet
       -q  "Quiet".  Do not print the introductory and copyright messages.
           These messages are also suppressed in batch mode.
### -x file.gdb
		       -x file
           Execute GDB commands from file.
##### Why use a file ?
To avoid writing manually those commands each time we launch a session.

##### Why use those command ?
- target remote: 3333
3333 is the ports used by openocd which is connected to our device.
We debug and load executable using openocd bridge, so we have to connect to it.

- load
Command to load our executable in the device flash memory.

- break main
set a breakpoint at the main function

###### Following command are used only for ITM configuration
- monitor tpiu config internal itm.txt uart off 72000000
	itm.txt: file where data will be written
	720000000: devie clock run
	TODO: Finish

- monitor itm port 0
Use port 0 as we use this one to send our data in our ITM configuration

- continue
Just start the program which should stop at main given our previous breakpoint.

# openocd

# minicom

# itmdump
