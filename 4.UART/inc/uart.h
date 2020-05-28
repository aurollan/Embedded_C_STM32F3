#ifndef __UART_H__
#define __UART_H__

#include "stm32f303xc.h"

/* USART CR1 register clear Mask ((~(uint32_t)0xFFFFE6F3)) */
#define CR1_CLEAR_MASK            ((uint32_t)(USART_CR1_M | USART_CR1_PCE | \
                                              USART_CR1_PS | USART_CR1_TE | \
                                              USART_CR1_RE))
typedef enum
{
  GPIO_PuPd_NOPULL = 0x00,
  GPIO_PuPd_UP     = 0x01,
  GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;

typedef enum
{
  GPIO_Speed_Level_1  = 0x01, /*!< Fast Speed     */
  GPIO_Speed_Level_2  = 0x02, /*!< Meduim Speed   */
  GPIO_Speed_Level_3  = 0x03  /*!< High Speed     */
}GPIOSpeed_TypeDef;

typedef enum
{
  GPIO_OType_PP = 0x00,
  GPIO_OType_OD = 0x01
}GPIOOType_TypeDef;

typedef enum
{
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN   = 0x03  /*!< GPIO Analog In/Out Mode      */
}GPIOMode_TypeDef;

void RCC_USART1_enable();
void GPIOA_connect_PIN9_PIN10();
void GPIOA_config();
void USART_enable();
void USART_output(char *s);
void echo_back();

#endif
