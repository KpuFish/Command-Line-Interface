/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* Uart Structure Init Function */
static void InitUartStructure(void);
static void DeInitUartStructure(void);

UART_HandleTypeDef huart3;
void MX_USART3_UART_Init(void)
{
    huart3.Instance           = USART3;
    huart3.Init.BaudRate      = 115200;
    huart3.Init.WordLength    = UART_WORDLENGTH_8B;
    huart3.Init.StopBits      = UART_STOPBITS_1;
    huart3.Init.Parity        = UART_PARITY_NONE;
    huart3.Init.Mode          = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl     = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling  = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
      Error_Handler();
    }

    /*  Setting for Uart Buffer  */
    InitUartStructure();
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    if (uartHandle->Instance==USART3)
    {
      /* USART3 clock enable */
      __HAL_RCC_USART3_CLK_ENABLE();

      __HAL_RCC_GPIOC_CLK_ENABLE();
      /**USART3 GPIO Configuration    
      PC10     ------> USART3_TX
      PC11     ------> USART3_RX 
      */
      GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      /* USART3 interrupt Init */
      HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{	
	if (uartHandle->Instance==USART3)
    {
      /* Peripheral clock disable */
      __HAL_RCC_USART3_CLK_DISABLE();

      /**USART3 GPIO Configuration    
      PC10     ------> USART3_TX
      PC11     ------> USART3_RX 
      */
      HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

      /* USART3 interrupt Deinit */
      HAL_NVIC_DisableIRQ(USART3_IRQn);
    }

    DeInitUartStructure();
}


rUART *s_uart;

void InitUartStructure(void)
{
    s_uart = (rUART *)malloc(sizeof(rUART));

    if (s_uart == NULL) {
      printf("UART Init Failed \n");
    }
}

void DeInitUartStructure(void)
{
    if (s_uart != NULL) {
      free(s_uart);
    }
}

rUART *GetUartStructure(void)
{
    return s_uart;
}

uint8_t IsRxDone(void)
{
	  return s_uart->rx_done;
}

void ResetRxDone(void)
{
	  s_uart->rx_done = RESET;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == huart3.Instance) {
        if (((s_uart->isr_rx_data == _LF_) || (s_uart->isr_rx_data == _CR_)) &&
            (s_uart->rx_done == RESET)) {
             s_uart->rx_index = RESET;
             s_uart->rx_done  = SET;
        } else {
           if (s_uart->rx_index < MAX_BUFFER) {
               s_uart->rx_data[s_uart->rx_index++] = s_uart->isr_rx_data;
           } else {
               s_uart->rx_data[s_uart->rx_index] = NULL;
               s_uart->rx_index = RESET;
           }
        }
        HAL_UART_Receive_IT(&huart3, (uint8_t *)&s_uart->isr_rx_data, 1);  // INT Re Set-Up
    }
}

int fputc(int ch, FILE* f)
{
    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, UART_TIMEOUT);
    return ch;
}
