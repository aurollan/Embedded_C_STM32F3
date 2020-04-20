#include "int.h"

void setup_user_button_interrupt()
{
	/* necessary ? */
	RCC->APB2ENR |= 1;

 //  ISER[8];  /*  (R/W)  Interrupt Set Enable Register           */
 //  ICER[8];  /*  (R/W)  Interrupt Clear Enable Register         */
 //  ISPR[8];  /*  (R/W)  Interrupt Set Pending Register          */
 //  ICPR[8];  /*  (R/W)  Interrupt Clear Pending Register        */
 //  IABR[8];  /*  (R/W)  Interrupt Active bit Register           */
 //  IP[240];  /*  (R/W)  Interrupt Priority Register (8Bit wide) */
 //  STIR;     /*  ( /W)  Software Trigger Interrupt Register     */
 //
	SYSCFG->EXTICR[0] &= ~((uint32_t)0xF);
	EXTI->IMR |= 1;
	EXTI->RTSR |= 1;
}

void EXTI0_IRQHandler()
{
	switch_off_leds();
	delay(1000);
}
