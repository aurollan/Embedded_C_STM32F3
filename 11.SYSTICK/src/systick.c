#include "leds.h"
#include "timer.h"
#include "systick.h"

void SysTick_Handler(void)
{
	switch_on_leds();
	/* switeched for 500 ms */
	delay(500);
}

void	init_systick(void)
{
	/* interrupt every 2s at 8 Mhz */
	SysTick_Config(15999999);
}
