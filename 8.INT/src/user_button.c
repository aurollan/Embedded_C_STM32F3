#include "user_button.h"

void	setup_user_button()
{
	/* Enable GPIOA clock */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	/* Ensure that PA0 MODER is 00 Input Mode */
	GPIOA->MODER &= ~((uint32_t)0x3);
	/* Make sure OUTPUT TYPE is reset state */ 
	GPIOA->OTYPER &= ~((uint32_t)0x1);
	/* Make sure OUTPUT SPEED is reset state */ 
	GPIOA->OSPEEDR |= ((uint32_t)0x3);
	/* Make sure No Pull Up No Pull Down */ 
	GPIOA->PUPDR &= ~((uint32_t)0x3);
	/* IDR is read only and we configured GPIO PA0 to input so we don't touch ODR */
	/* No alternate function used here */
}
