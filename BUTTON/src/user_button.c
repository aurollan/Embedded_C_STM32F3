#include "user_button.h"

void	setup_user_button()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	/* Ensure that PA0 MODER is 00 */
	GPIOA->MODER &= ~((uint32_t)0x3);
	GPIOA->OTYPER &= ~((uint32_t)0x1);
	GPIOA->OSPEEDR |= ((uint32_t)0x3);
	GPIOA->PUPDR &= ~((uint32_t)0x3);
	/* IDR is read only and we configured GPIO PA0 to input so we don't touch ODR */
	/* No alternate function used here */
}
