/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

TIM_HandleTypeDef htim1;

/* TIM1 init function */
#define __TIM_EN__          1
//#define __TIM_INT_EN__		1
//#define __INT_EN__			1
void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* 1us timer set-up */
  htim1.Instance = TIM1;
#if __TIM_EN__
  htim1.Init.Prescaler = 0;    // SysCLK is 84MHz (supplied clk), 84MHz / (84-1) = 1MHz, 1us tick timer
  htim1.Init.Period    = 65535-1; // This is up counter needed to larger than the number 1, 16bit up counter
#endif
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  #if defined __TIM_INT_EN__
  HAL_TIM_Base_Start_IT(&htim1);  // Tim Start with IT
  #else
  HAL_TIM_Base_Start(&htim1);     // Tim Start
  #endif
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM1)
  {
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    #if __TIM_EN__ && __INT_EN__
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    #endif
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();
  }
} 

#if defined __TIM_INT_EN__
volatile unsigned short int tick_1us = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1) {
        if (tick_1us >= 50) {
          tick_1us = 0;
          //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
		  //printf("1 TIMER CALL BACK %d  \n", htim1.Instance->CNT);
        } else {
          tick_1us++;
        }
    }
}
#endif
