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
	SYSCFG->EXTICR[0] &= ~((uint32_t)0x7);
	EXTI->IMR |= 1;
	//EXTI->FTSR |= 1;
	EXTI->RTSR |= 1;
 	NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler()
{
	_write(0, "Hello World\n", 12);
	EXTI->PR = ((uint32_t)(0x1));
}
