# Understanding Interrupts
If you read your device documentation you should see a lost of topic 
about interrupts. It's a very useful tool widely used in embedded software 
development. It allow us to do something very interesting:

	suspend the program flow to do a task then resume it.

Here we are going to implement a simple interrupt using LEDs project and 
User_button project.
The program with switch on all leds and switch them off only when pressing the User_button.
Of course this is easy to do without interrupt but it's a clear and easy example to understand how it works.
# How Interrupts works ?
# How to program it ?
We just need setup our user_button interrupt to do what we want.
## Exploring device documentation
### Datasheet
We find a section about interrupts:

	page 20
	3.12 Interrupts and events

According to this section our device have a Nested Vectored Interrupt Controller (NVIC) able to handle

- 66 channels
- 16 priority levels

And that's all for the Datasheet.

### User manual
Here we find informations about User Push-button:

	Connected to the GPIOA0

We already know this from the USER_BUTTON poject.
There is no information about interrupt.

### Reference manual
#### Finding detailled information
There are 2 notions to understand here. NVIC we saw above and how to handle interruption for our GPIOA0.

Contents section drives us to 

	page 285
	14 Interrupts and events
	
	page 228
	11 General-purpose I/Os (GPIO)


First let's read the Interrup and events section.

#### Interrupts and events
According to the Reference Manual (Note that this information differs from device DataSheet):

- 74 channels + 16 for Cortex M4 with FPU units like our device
- 16 priority levels

Looking at 

	Page 285
	Table 82. STM32F303xB/C/D/E, STM32F358xC and STM32F398xE vector table

There is no trace of our PA0 and all interrupts seems explicitly related to 
communication bus exept for EXTI line, so we keep reading.

We finally end 

	page 295
	Figure 51. External interrupt/event GPIO mapping

Here we find our PA0 attached to EXTI0 and referring to 

	page 285
	Table 82. STM32F303xB/C/D/E, STM32F358xC and STM32F398xE vector table

| 6 | 13 | settable | EXTI0 | EXTI Line0 interrupt | 0x0000 0058 |

Here is our interrupt function called when it occurs.
We even find 

	page 294
	14.2.5 Functional description
		   Hardware interrupt selection

explaining how to configure or Hardware interrupt.
And there is a cripted final step :

	Configure the enable and mask bits that control the NVIC IRQ channel mapped to the EXTI so that an interrupt coming from one of the EXTI line can be correctly
	acknowledged.

Did we miss something ?
Yes, we do. We don't know how to manage the NVIC we talk about.

	page 285
	14 Interrupts and events
	14.1 Nested vectored interrupt controller (NVIC)
	14.1.1 NVIC main features

There is few information for such an important topic...
It's because they redirect us to the `PM0214 programming manual for
Cortex ® -M4 products`.

### Cortex M4 Manual
This is a docimentation dedicated to our Cortex M4F microcontroller.
#### Interrupt and exception
If we look for Interrupt we end 

	page 26
	2.1.4 Exceptions and interrupts

We ar elooking for interrupt and it redirect us to

	page 208
	4.3 Nested vectored interrupt controller (NVIC)

We find the register summary and finally

	page 209
	4.3.1 Accessing the Cortex-M4 NVIC registers using CMSIS
	Table 46. CMSIS access NVIC functions

A library to manage the NVIC. Function names are explicitm they are located in core_m4.h like the typedef.

#### General-purpose I/Os (GPIO)
We have already read a lot about GPIO on the previous projects, so we focus only on new informations we need.
Looking for interrupt and GPIO not exception so we end up

	page 233
	11.3.8 External interrupt/wakeup lines

We see that GPIO must be configured in Input mode. That's what we have done for our user_button so let's check the 2 links provided

	page 292
	14.2 Extended interrupts and events controller (EXTI)

	page 293
	14.2.3 Wakeup event management

Going back to what we read at first and no more.
It seems that we only need to configure this part to get our interrupt to work.
Maybe we should have start here...

## How to code
### Finding the right configuration
Here we want to set an Hardware interrupt.
As soon as our user_button is pushed the code is interrupted.

We follow the steps:
	To configure a line as interrupt source, use the following procedure:
	• Configure the corresponding mask bit in the EXTI_IMR register.
	• Configure the Trigger Selection bits of the Interrupt line (EXTI_RTSR and EXTI_FTSR)
	• Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
	EXTI so that an interrupt coming from one of the EXTI line can be correctly
	acknowledged.

And :
	
	page 295
	Figure 51. External interrupt/event GPIO mapping

According to this figure we need to configure this register too.
A quick search leads us to 

	page 249
	12.1.3 SYSCFG external interrupt configuration register 1 (SYSCFG_EXTICR1)

We need to configure this register too.
From this information we get that:
Line A, B, C, D, E, F, G, H are internal line and our PA0 is mapped to line 0.

### Coding function
#### Configure SYSCFG_EXTICR1
We only want to use interrupts comming from GPIO PA0.
We have to setup SYSCFG_EXTICR1 EXTI0 for PA0.

#### Configure line as input source
Configure the corresponding mask bit in the EXTI_IMR register.
	We have to write 1 to line 0 in the Interrupt Mask Register in order to enable it.

Configure the Trigger Selection bits of the Interrupt line (EXTI_RTSR and EXTI_FTSR)
	We wait for an input which signify a rising edge to our gpioa. So we only have to write 1 to the RTSR.

Configure the enable and mask bits that control the NVIC IRQ channel mapped to the EXTI so that an interrupt coming from one of the EXTI line can be correctly acknowledged.

	NVIC core_cm4.h function ?
	enable EXTI and set priority
