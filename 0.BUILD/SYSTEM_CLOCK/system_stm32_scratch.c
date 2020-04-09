static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

/******************************************************************************/
/*            Reset															  */
/******************************************************************************/

  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR &= 0xF87FC00C;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

  /* Reset PREDIV1[3:0] bits */
  RCC->CFGR2 &= (uint32_t)0xFFFFFFF0;

  /* Reset USARTSW[1:0], I2CSW and TIMs bits */
  RCC->CFGR3 &= (uint32_t)0xFF00FCCC;
  
  /* Disable all interrupts */
  RCC->CIR = 0x00000000;
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/

  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------*/
  /* Enable HSE */
  RCC->CR |= ((uint32_t)(0x1UL << (16U)));
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & (0x1UL << (17U));
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != ((uint16_t)0x5000)));

  if ((RCC->CR & (0x1UL << (17U))) != 0U)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer and set Flash Latency */
    FLASH->ACR = (0x1UL << (4U)) | (uint32_t)(0x2UL << (0U));
 
     /* HCLK = SYSCLK / 1 */
     RCC->CFGR |= (uint32_t)(0x00000000U);
       
     /* PCLK2 = HCLK / 1 */
     RCC->CFGR |= (uint32_t)(0x00000000U);
     
     /* PCLK1 = HCLK / 2 */
     RCC->CFGR |= (uint32_t)(0x00000400U);

    /* PLL configuration */
    RCC->CFGR &= (uint32_t)((uint32_t)~((0x1UL << (16U)) | (0x1UL << (17U)) | ((uint32_t)0x003C0000)));
    RCC->CFGR |= (uint32_t)(((uint32_t)0x00010000) | ((uint32_t)0x00000000) | ((uint32_t)0x001C0000));

    /* Enable PLL */
    RCC->CR |= (0x1UL << (24U));

    /* Wait till PLL is ready */
    while((RCC->CR & (0x1UL << (25U))) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~((0x3UL << (0U))));
    RCC->CFGR |= (uint32_t)(0x00000002U);

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)(0x3UL << (2U))) != (uint32_t)(0x00000008U))
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}

void start_clock()
{
	/* HSEON */
	RCC->CR |= (1 << 16);

    FLASH->ACR = (0x1UL << (4U)) | (uint32_t)(0x2UL << (0U));

	/* while HSE not ready */
	while ((RCC->CR & (1 << 17)) == 0) {};

	/* Microcontroller Clock Output 100 sysclk selected */
	// RCC->CFGR |= (4 << 24);

	/* HPRE HLCK prescqler sysclk not divided */
	RCC->CFGR &= ~(0xF << 4);

	/* PRE2 hight speed APB2 prescaler no prescaler*/
	RCC->CFGR &= ~(7 << 11);
	
	/* PRE1 low speed APB1  divided by 2 prescaler */
	RCC->CFGR |= (4 << 8);
	
	/* PLLMULL * 9 (0111) */ 
	RCC->CFGR |= (7 << 18);

	/* PLLXTPRE HSE input to PLL not divided (8MHZ) bit to 0*/
	RCC->CFGR &= ~(1 << 17);

	/* PLLSRC HSE/PREVID1 selected as PLL input clock */
	RCC->CFGR |= (1 << 16);

	/* 			PLLON		*/
	RCC->CR |= (1 << 24);
	/* while PLLRDY locked */
	while ((RCC->CR & (1 << 25)) == 1) {};

	/* System clock swith to PLL clock SW */
	RCC->CFGR &= ~(3 << 0);
	RCC->CFGR |= (2 << 0);

	/* CHECK SWS */
    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)(2 << 2)) == 0) {};
}
