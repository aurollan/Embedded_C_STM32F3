/*
 * DISCLAIMER: This code is not working this is just an illustration
 * I WILL make it works later but for know its just for
 * understanding and demystify the startup code
 */

void Default_Handler();

void	Reset_Handler(void) {
	/* We use __set_SP(uint32_t stack) function defined in `cmsis_gcc.h` to set stack pointer */
	__set_SP(_estack);
	char *sidata = (char*)_sidata;
	char *sdata  = (char*)_sdata;
	char *edata  = (char*)_edata;
	char *sbss   = (char*)_sbss;
	char *ebss   = (char*)_ebss;


	/* putting only 0 in bss section */
	while (sbss < ebss)
	{
		/* put 0 value into memory */
		*sbbs = 0;
		/* go to next address */
		sbbs++;
	}
	while (sdata < edata)
	{
		/* copy sidata value into sdata memory */
		*sdata = *sidata;
		/* go to next address in RAM */
		sdata++;
		/* go to next address in FLASH */
		sidata++;
	}
	/* call system init function */
	SystemInit();
	/* call main and start program */
	main();
	/* something has gone really wrong */
	Default_Handler();
}

void Default_Handler()
{
	/* Our infinite loop */
	while (1);
}

void NMI_Handler()
{
	Default_Handler();
}

void HardFault_Handler()
{
	Default_Handler();
}

void MemManage_Handler()
{
	Default_Handler();
}

void BusFault_Handler()
{
	Default_Handler();
}

void UsageFault_Handler()
{
	Default_Handler();
}

void SVC_Handler()
{
	Default_Handler();
}

void DebugMon_Handler()
{
	Default_Handler();
}

void PendSV_Handler()
{
	Default_Handler();
}

void SysTick_Handler()
{
	Default_Handler();
}

void WWDG_IRQHandler()
{
	Default_Handler();
}

void PVD_IRQHandler()
{
	Default_Handler();
}

void TAMPER_STAMP_IRQHandler()
{
	Default_Handler();
}

void RTC_WKUP_IRQHandler()
{
	Default_Handler();
}

void FLASH_IRQHandler()
{
	Default_Handler();
}

void RCC_IRQHandler()
{
	Default_Handler();
}

void EXTI0_IRQHandler()
{
	Default_Handler();
}

void EXTI1_IRQHandler()
{
	Default_Handler();
}

void EXTI2_TS_IRQHandler()
{
	Default_Handler();
}

void EXTI3_IRQHandler()
{
	Default_Handler();
}

void EXTI4_IRQHandler()
{
	Default_Handler();
}

void DMA1_Channel1_IRQHandler()
{
	Default_Handler();
}

void DMA1_Channel2_IRQHandler()
{
	Default_Handler();
}

void DMA1_Channel3_IRQHandler()
{
	Default_Handler();
}

void DMA1_Channel4_IRQHandler()
{
	Default_Handler();
}

void DMA1_Channel5_IRQHandler()
{
	Default_Handler();
}

void DMA1_Channel6_IRQHandler()
{
	Default_Handler();
}

void DMA1_Channel7_IRQHandler()
{
	Default_Handler();
}

void ADC1_2_IRQHandler()
{
	Default_Handler();
}

void USB_HP_CAN1_TX_IRQHandler()
{
	Default_Handler();
}

void USB_LP_CAN1_RX0_IRQHandler()
{
	Default_Handler();
}

void CAN1_RX1_IRQHandler()
{
	Default_Handler();
}

void CAN1_SCE_IRQHandler()
{
	Default_Handler();
}

void EXTI9_5_IRQHandler()
{
	Default_Handler();
}

void TIM1_BRK_TIM15_IRQHandler()
{
	Default_Handler();
}

void TIM1_UP_TIM16_IRQHandler()
{
	Default_Handler();
}

void TIM1_TRG_COM_TIM17_IRQHandler()
{
	Default_Handler();
}

void TIM1_CC_IRQHandler()
{
	Default_Handler();
}

void TIM2_IRQHandler()
{
	Default_Handler();
}

void TIM3_IRQHandler()
{
	Default_Handler();
}

void TIM4_IRQHandler()
{
	Default_Handler();
}

void I2C1_EV_IRQHandler()
{
	Default_Handler();
}

void I2C1_ER_IRQHandler()
{
	Default_Handler();
}

void I2C2_EV_IRQHandler()
{
	Default_Handler();
}

void I2C2_ER_IRQHandler()
{
	Default_Handler();
}

void SPI1_IRQHandler()
{
	Default_Handler();
}

void SPI2_IRQHandler()
{
	Default_Handler();
}

void USART1_IRQHandler()
{
	Default_Handler();
}

void USART2_IRQHandler()
{
	Default_Handler();
}

void USART3_IRQHandler()
{
	Default_Handler();
}

void EXTI15_10_IRQHandler()
{
	Default_Handler();
}

void RTC_Alarm_IRQHandler()
{
	Default_Handler();
}

void USBWakeUp_IRQHandler()
{
	Default_Handler();
}

void TIM8_BRK_IRQHandler()
{
	Default_Handler();
}

void TIM8_UP_IRQHandler()
{
	Default_Handler();
}

void TIM8_TRG_COM_IRQHandler()
{
	Default_Handler();
}

void TIM8_CC_IRQHandler()
{
	Default_Handler();
}

void ADC3_IRQHandler()
{
	Default_Handler();
}

void SPI3_IRQHandler()
{
	Default_Handler();
}

void UART4_IRQHandler()
{
	Default_Handler();
}

void UART5_IRQHandler()
{
	Default_Handler();
}

void TIM6_DAC_IRQHandler()
{
	Default_Handler();
}

void TIM7_IRQHandler()
{
	Default_Handler();
}

void DMA2_Channel1_IRQHandler()
{
	Default_Handler();
}

void DMA2_Channel2_IRQHandler()
{
	Default_Handler();
}

void DMA2_Channel3_IRQHandler()
{
	Default_Handler();
}

void DMA2_Channel4_IRQHandler()
{
	Default_Handler();
}

void DMA2_Channel5_IRQHandler()
{
	Default_Handler();
}

void ADC4_IRQHandler()
{
	Default_Handler	();
}

void COMP1_2_3_IRQHandler()
{
	Default_Handler();
}

void COMP4_5_6_IRQHandler()
{
	Default_Handler();
}

void COMP7_IRQHandler()
{
	Default_Handler	();
}

void USB_HP_IRQHandler()
{
	Default_Handler();
}

void USB_LP_IRQHandler()
{
	Default_Handler();
}

void USBWakeUp_RMP_IRQHandler()
{
	Default_Handler();
}

void FPU_IRQHandler()
{
	Default_Handler();
}

/* Note that we should load it to the section .isr_vector */
g_pfnVector = [
	_estack,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	PVD_IRQHandler,
	TAMPER_STAMP_IRQHandler,
	RTC_WKUP_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_TS_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_IRQHandler,
	DMA1_Channel3_IRQHandler,
	DMA1_Channel4_IRQHandler,
	DMA1_Channel5_IRQHandler,
	DMA1_Channel6_IRQHandler,
	DMA1_Channel7_IRQHandler,
	ADC1_2_IRQHandler,
	USB_HP_CAN1_TX_IRQHandler,
	USB_LP_CAN1_RX0_IRQHandler,
	CAN1_RX1_IRQHandler,
	CAN1_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_TIM15_IRQHandler,
	TIM1_UP_TIM16_IRQHandler,
	TIM1_TRG_COM_TIM17_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM4_IRQHandler,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	I2C2_EV_IRQHandler,
	I2C2_ER_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	USART3_IRQHandler,
	EXTI15_10_IRQHandler,
	RTC_Alarm_IRQHandler,
	USBWakeUp_IRQHandler,
	TIM8_BRK_IRQHandler,
	TIM8_UP_IRQHandler,
	TIM8_TRG_COM_IRQHandler,
	TIM8_CC_IRQHandler,
	ADC3_IRQHandler,
	0,
	0,
	0,
	SPI3_IRQHandler,
	UART4_IRQHandler,
	UART5_IRQHandler,
	TIM6_DAC_IRQHandler,
	TIM7_IRQHandler,
	DMA2_Channel1_IRQHandler,
	DMA2_Channel2_IRQHandler,
	DMA2_Channel3_IRQHandler,
	DMA2_Channel4_IRQHandler,
	DMA2_Channel5_IRQHandler,
	ADC4_IRQHandler,
	0,
	0,
	COMP1_2_3_IRQHandler,
	COMP4_5_6_IRQHandler,
	COMP7_IRQHandler,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	USB_HP_IRQHandler,
	USB_LP_IRQHandler,
	USBWakeUp_RMP_IRQHandler,
	0,
	0,
	0,
	0,
	FPU_IRQHandler,
	];
