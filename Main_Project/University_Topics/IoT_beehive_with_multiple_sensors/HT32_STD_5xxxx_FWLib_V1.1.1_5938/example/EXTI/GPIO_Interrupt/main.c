/*********************************************************************************************************//**
 * @file    EXTI/GPIO_Interrupt/main.c
 * @version $Rev:: 5683         $
 * @date    $Date:: 2021-12-30 #$
 * @brief   Main program.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup EXTI_Examples EXTI
  * @{
  */

/** @addtogroup GPIO_Interrupt
  * @{
  */


/* Private function prototypes -----------------------------------------------------------------------------*/
void EXTI_Configuration(void);

void Key_Process(void);

/* Global variables ----------------------------------------------------------------------------------------*/
vu32 guKeyState[1];

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  HT32F_DVB_LEDInit(HT_LED1);

  EXTI_Configuration();

  while (1)
  {
    Key_Process();
  }
}

/*********************************************************************************************************//**
  * @brief  Key Process
  * @retval None
  ***********************************************************************************************************/
void Key_Process(void)
{
  if (guKeyState[0] == TRUE)
  {
    guKeyState[0] = FALSE;
    HT32F_DVB_LEDToggle(HT_LED1);
  }
}

/*********************************************************************************************************//**
  * @brief  EXTI configuration.
  * @retval None
  ***********************************************************************************************************/
void EXTI_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.EXTI = 1;
    CKCUClock.Bit.PB   = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Configure AFIO mode of input pins                                                                      */
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_12, AFIO_FUN_GPIO);

  /* Enable GPIO Input Function                                                                             */
  GPIO_InputConfig(HT_GPIOB, GPIO_PIN_12, ENABLE);

  /* Configure GPIO pull resistor of input pins                                                             */
  GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_12, GPIO_PR_DISABLE);

  /* Select Port as EXTI Trigger Source                                                                     */
  AFIO_EXTISourceConfig(AFIO_EXTI_CH_12, AFIO_ESS_PB);

  { /* Configure EXTI Channel n as rising edge trigger                                                      */

    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_12;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_POSITIVE_EDGE;
    EXTI_Init(&EXTI_InitStruct);
  }

  /* Enable EXTI & NVIC line Interrupt                                                                      */
  EXTI_IntConfig(EXTI_CHANNEL_12, ENABLE);
  NVIC_EnableIRQ(EXTI12_IRQn);
}

#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: printf("Parameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
