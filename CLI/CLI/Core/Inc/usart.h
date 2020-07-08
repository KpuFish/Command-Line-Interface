/**
  ******************************************************************************
  * File Name          : USART.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#define UART_TIMEOUT        5000

#define _LF_                0x0A
#define _CR_                0x0D

#define RESET               0
#define SET                 1
#define MAX_BUFFER          32


typedef struct __UART__
{
  //volatile uint8_t tx_data[MAX_BUFFER];
  volatile uint8_t rx_data[MAX_BUFFER];
  volatile uint8_t isr_rx_data;
  volatile uint8_t rx_index;
  volatile uint8_t rx_done;
} rUART;


extern UART_HandleTypeDef huart3;

rUART *GetUartStructure(void);

void MX_USART3_UART_Init(void);

void ResetRxDone(void);

uint8_t IsRxDone(void);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */
