#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)8000000) /*!< Default value of the External oscillator in Hz.
                                                This value can be provided and adapted by the user application. */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)8000000) /*!< Default value of the Internal oscillator in Hz.
                                                This value can be provided and adapted by the user application. */
#endif /* HSI_VALUE */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field.
                                  This value must be a multiple of 0x200. */



#include "stm32f3xx.h"



uint32_t SystemCoreClock = 8000000;




const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};



const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

static void ft_setup_max_clock_frequency()


/**
  * @brief  Setup the microcontroller system
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
/* FPU settings --------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
#endif
  ft_setup_max_clock_frequency();
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}
/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @note   - The system frequency computed by this function is not the real
  *           frequency in the chip. It is calculated based on the predefined
  *           constant and the selected clock source:
  *
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**)
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *
  *         (*) HSI_VALUE is a constant defined in stm32f3xx_hal.h file (default value
  *             8 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.
  *
  *         (**) HSE_VALUE is a constant defined in stm32f3xx_hal.h file (default value
  *              8 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, predivfactor = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case RCC_CFGR_SWS_HSI:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case RCC_CFGR_SWS_HSE:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case RCC_CFGR_SWS_PLL:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & RCC_CFGR_PLLMUL;
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;
      pllmull = ( pllmull >> 18) + 2;

#if defined (STM32F302xE) || defined (STM32F303xE) || defined (STM32F398xx)
        predivfactor = (RCC->CFGR2 & RCC_CFGR2_PREDIV) + 1;
      if (pllsource == RCC_CFGR_PLLSRC_HSE_PREDIV)
      {
        /* HSE oscillator clock selected as PREDIV1 clock entry */
        SystemCoreClock = (HSE_VALUE / predivfactor) * pllmull;
      }
      else
      {
        /* HSI oscillator clock selected as PREDIV1 clock entry */
        SystemCoreClock = (HSI_VALUE / predivfactor) * pllmull;
      }
#else      
      if (pllsource == RCC_CFGR_PLLSRC_HSI_DIV2)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
        predivfactor = (RCC->CFGR2 & RCC_CFGR2_PREDIV) + 1;
        /* HSE oscillator clock selected as PREDIV1 clock entry */
        SystemCoreClock = (HSE_VALUE / predivfactor) * pllmull;
      }
#endif /* STM32F302xE || STM32F303xE || STM32F398xx */
      break;
    default: /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK clock frequency ----------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

/* Sysclock = 72 Mhz
 * HCLK		= 72 Mhz
 * APB1		= 36 Mhz
 * APB2		= 72 Mhz
 * AHB		= 72 Mhz
 */

static void ft_setup_max_clock_frequency()
{
	/* HSEON		*/
	/* Enable HSE	*/
	RCC->CR |= (1 << 16);

    /* Enable Prefetch Buffer and set Flash Latency				*/
	/* page 78 set flash latency to 2 for HCLK between 16-72mhz */
    FLASH->ACR = (0x1UL << (4U)) | (uint32_t)(0x2UL << (0U));

	/* Wait till HSE is ready	*/
	/* while HSE not ready		*/
	while ((RCC->CR & (0x1 << 17)) == 0) {};

	/* HPRE HLCK prescqler sysclk not divided	*/
	/* HCLK = SYSCLK / 1						*/
	RCC->CFGR &= ~(0xF << 4);

	/* PRE2 hight speed APB2 prescaler no prescaler	*/
	/* PCLK2 = HCLK / 1								*/
	RCC->CFGR &= ~(0x7 << 11);
	
	/* PRE1 low speed APB1  divided by 2 prescaler	*/
	/* PCLK1 = HCLK / 2								*/
	RCC->CFGR |= (0x4 << 8);
	
    /* PLL configuration	*/
	/* PLLMULL * 9 (0111)	*/ 
	RCC->CFGR &= ~(0xF << 18);
	RCC->CFGR |= (0x7 << 18);
	/* PLLXTPRE HSE input to PLL not divided (8MHZ) bit to 0*/
	RCC->CFGR &= ~(0x1 << 17);
	/* PLLSRC HSE/PREVID1 selected as PLL input clock */
	RCC->CFGR |= (0x1 << 16);

	/* PLLON		*/
    /* Enable PLL	*/
	RCC->CR |= (0x1 << 24);
	/* while PLLRDY locked		*/
    /* Wait till PLL is ready	*/
	while ((RCC->CR & (0x1 << 25)) == 0) {};

	/* System clock swith to PLL clock SW	*/
    /* Select PLL as system clock source	*/
	RCC->CFGR &= ~(3 << 0);
	RCC->CFGR |= (2 << 0);

	/* CHECK SWS									*/
    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (3 << 2)) != (2 << 2)) {};
}
