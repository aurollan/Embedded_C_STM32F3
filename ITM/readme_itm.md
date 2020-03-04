p1114

By default, these pin is NOT assigned. It can be assigned by setting the TRACE_IOEN and
TRACE_MODE bits in the Debug MCU configuration register (DBGMCU_CR). This
configuration has to be done by the debugger host.a

To assign the TRACE pin, the debugger host must program the bits TRACE_IOEN and
TRACE_MODE[1:0] of the Debug MCU configuration Register (DBGMCU_CR). By default
the TRACE pins are not assigned.
This register is mapped on the external PPB and is reset by the PORESET (and not by the
SYSTEM reset). It can be written by the debugger under SYSTEM reset.


p1104
The packets emitted by the ITM are output to the TPIU (Trace Port Interface Unit). The
formatter of the TPIU adds some extra packets (refer to TPIU) and then output the complete
packets sequence to the debugger host.
The bit TRCEN of the Debug Exception and Monitor Control Register must be enabled
before programming or using the ITM.

Write 0xC5ACCE55 to unlock Write Access to the other ITM
registers
Example of configuration
To output a simple value to the TPIU:
• Configure the TPIU and assign TRACE I/Os by configuring the DBGMCU_CR (refer to
Section 33.17.2: TRACE pin assignment and Section 33.16.3: Debug MCU
configuration register)
• Write 0xC5ACCE55 to the ITM Lock Access Register to unlock the write access to the
ITM registers
• Write 0x00010005 to the ITM Trace Control Register to enable the ITM with Sync
enabled and an ATB ID different from 0x00
• Write 0x1 to the ITM Trace Enable Register to enable the Stimulus Port 0
• Write 0x1 to the ITM Trace Privilege Register to unmask stimulus ports 7:0
• Write the value to output in the Stimulus Port Register 0: this can be done by software
(using a printf function)

config p1106 and p1119

You must enable TRCENA of the Debug Exception and Monitor Control Register (DEMCR) before you program or use the ITM

 ITM registers are fully accessible in privileged mode. In user mode, all registers can be read,
but only the Stimulus Registers and Trace Enable Registers can be written, and only when the
corresponding Trace Privilege Register bit is set. Invalid user mode writes to the ITM registers
are discarded.
If the debugger host wants to switch to the SW-DP, it must provide a dedicated JTAG
sequence on TMS/TCK (respectively mapped to SWDIO and SWCLK) which disables the
JTAG-DP and enables the SW-DP. This way it is possible to activate the SWDP using only
the SWCLK and SWDIO pins.
This sequence is:
1. Send more than 50 TCK cycles with TMS (SWDIO) =1
2. Send the 16-bit sequence on TMS (SWDIO) = 0111100111100111 (MSB transmitted
first)
3. Send more than 50 TCK cycles with TMS (SWDIO) =1

