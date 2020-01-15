/* GPIOE MAPPING */
#define GPIOE (*(unsigned int *)0x48001000) 

/* Address offset: 0x00 */
#define GPIOE_MODER (*(unsigned int *)0x48001000) 
/* Address offset: 0x14 */
#define GPIOE_ODR (*(unsigned int *)0x48001014) 
/* Address offset: 0x18 */
#define GPIOE_BSRR (*(unsigned int *)0x48001018) 

/* RCC MAPPING */
#define RCC (*(unsigned int *)0x40021000)
/* Address offset: 0x14 */
#define RCC_AHBENR (*(unsigned int *)0x40021014)

int main ()
{
	/* Bit 21 IOPEEN: I/O port E clock enable */
	/* 0: I/O port E clock disabled */
	/* 1: I/O port E clock enabled. */
	RCC_AHBENR = 1 << 21;

	/* Turn on 2 leds with BSSR */
	GPIOE_BSRR = 1 << 9;
	GPIOE_BSRR = 1 << 11;

	/* Set up leds as output */
	GPIOE_MODER |= 0x55550000; // a trouver

	/* Turn on 2 leds with ODR */
	GPIOE_ODR |= 0x55550000; // a trouver

	while (1)
	{

	};
	return (0);
}

