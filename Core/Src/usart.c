///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file    usart.c
//  * @brief   This file provides code for the configuration
//  *          of the USART instances.
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (c) 2023 STMicroelectronics.
//  * All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  * If no LICENSE file comes with this software, it is provided AS-IS.
//  *
//  ******************************************************************************
//  */
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/
//#include "usart.h"
//
///* USER CODE BEGIN 0 */
//
///* USER CODE END 0 */
//
///* LPUART1 init function */
//
//void MX_LPUART1_UART_Init(void)
//{
//
//  /* USER CODE BEGIN LPUART1_Init 0 */
//
//  /* USER CODE END LPUART1_Init 0 */
//
//  LL_LPUART_InitTypeDef LPUART_InitStruct = {0};
//
//  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//  LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1);
//
//  /* Peripheral clock enable */
//  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);
//
//  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
//  /**LPUART1 GPIO Configuration
//  PA2   ------> LPUART1_TX
//  */
//  GPIO_InitStruct.Pin = Debug_lpuart_Pin;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
//  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
//  GPIO_InitStruct.Alternate = LL_GPIO_AF_12;
//  LL_GPIO_Init(Debug_lpuart_GPIO_Port, &GPIO_InitStruct);
//
//  /* USER CODE BEGIN LPUART1_Init 1 */
//
//  /* USER CODE END LPUART1_Init 1 */
//  LPUART_InitStruct.PrescalerValue = LL_LPUART_PRESCALER_DIV1;
//  LPUART_InitStruct.BaudRate = 500000;
//  LPUART_InitStruct.DataWidth = LL_LPUART_DATAWIDTH_8B;
//  LPUART_InitStruct.StopBits = LL_LPUART_STOPBITS_1;
//  LPUART_InitStruct.Parity = LL_LPUART_PARITY_NONE;
//  LPUART_InitStruct.TransferDirection = LL_LPUART_DIRECTION_TX;
//  LL_LPUART_Init(LPUART1, &LPUART_InitStruct);
//  LL_LPUART_SetTXFIFOThreshold(LPUART1, LL_LPUART_FIFOTHRESHOLD_1_8);
//  LL_LPUART_SetRXFIFOThreshold(LPUART1, LL_LPUART_FIFOTHRESHOLD_1_8);
//  LL_LPUART_DisableFIFO(LPUART1);
//  LL_LPUART_EnableHalfDuplex(LPUART1);
//
//  /* USER CODE BEGIN WKUPType LPUART1 */
//
//  /* USER CODE END WKUPType LPUART1 */
//
//  LL_LPUART_Enable(LPUART1);
//
//  /* Polling LPUART1 initialisation */
//  while(!(LL_LPUART_IsActiveFlag_TEACK(LPUART1)))
//  {
//  }
//  /* USER CODE BEGIN LPUART1_Init 2 */
//
//  /* USER CODE END LPUART1_Init 2 */
//
//}
///* USART2 init function */
//
//void MX_USART2_UART_Init(void)
//{
//
//  /* USER CODE BEGIN USART2_Init 0 */
//
//  /* USER CODE END USART2_Init 0 */
//
//  LL_USART_InitTypeDef USART_InitStruct = {0};
//
//  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
//
//  /* Peripheral clock enable */
//  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
//
//  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
//  /**USART2 GPIO Configuration
//  PB3   ------> USART2_TX
//  */
//  GPIO_InitStruct.Pin = OneWire_USART2_Pin;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
//  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
//  LL_GPIO_Init(OneWire_USART2_GPIO_Port, &GPIO_InitStruct);
//
//  /* USER CODE BEGIN USART2_Init 1 */
//
//  /* USER CODE END USART2_Init 1 */
//  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
//  USART_InitStruct.BaudRate = 115200;
//  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
//  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
//  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
//  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
//  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
//  LL_USART_Init(USART2, &USART_InitStruct);
//  LL_USART_SetTXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
//  LL_USART_SetRXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
//  LL_USART_DisableFIFO(USART2);
//  LL_USART_ConfigHalfDuplexMode(USART2);
//
//  /* USER CODE BEGIN WKUPType USART2 */
//
//  /* USER CODE END WKUPType USART2 */
//
//  LL_USART_Enable(USART2);
//
//  /* Polling USART2 initialisation */
//  while((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))))
//  {
//  }
//  /* USER CODE BEGIN USART2_Init 2 */
//
//  /* USER CODE END USART2_Init 2 */
//
//}
///* USART3 init function */
//
//void MX_USART3_UART_Init(void)
//{
//
//  /* USER CODE BEGIN USART3_Init 0 */
//
//  /* USER CODE END USART3_Init 0 */
//
//  LL_USART_InitTypeDef USART_InitStruct = {0};
//
//  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//  LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
//
//  /* Peripheral clock enable */
//  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
//
//  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
//  /**USART3 GPIO Configuration
//  PC10   ------> USART3_TX
//  PC11   ------> USART3_RX
//  */
//  GPIO_InitStruct.Pin = KS_USART3_TX_Pin;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
//  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
//  LL_GPIO_Init(KS_USART3_TX_GPIO_Port, &GPIO_InitStruct);
//
//  GPIO_InitStruct.Pin = KS_USART3_RX_Pin;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
//  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
//  LL_GPIO_Init(KS_USART3_RX_GPIO_Port, &GPIO_InitStruct);
//
//  /* USER CODE BEGIN USART3_Init 1 */
//
//  /* USER CODE END USART3_Init 1 */
//  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
//  USART_InitStruct.BaudRate = 115200;
//  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
//  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
//  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
//  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
//  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
//  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
//  LL_USART_Init(USART3, &USART_InitStruct);
//  LL_USART_SetTXFIFOThreshold(USART3, LL_USART_FIFOTHRESHOLD_1_8);
//  LL_USART_SetRXFIFOThreshold(USART3, LL_USART_FIFOTHRESHOLD_1_8);
//  LL_USART_DisableFIFO(USART3);
//  LL_USART_ConfigAsyncMode(USART3);
//
//  /* USER CODE BEGIN WKUPType USART3 */
//
//  /* USER CODE END WKUPType USART3 */
//
//  LL_USART_Enable(USART3);
//
//  /* Polling USART3 initialisation */
//  while((!(LL_USART_IsActiveFlag_TEACK(USART3))) || (!(LL_USART_IsActiveFlag_REACK(USART3))))
//  {
//  }
//  /* USER CODE BEGIN USART3_Init 2 */
//
//  /* USER CODE END USART3_Init 2 */
//
//}
//
///* USER CODE BEGIN 1 */
//int __io_putchar(int ch) {
//    uint8_t c[1];
//    c[0] = ch & 0xFF;
////    while (hlpuart1.gState == HAL_UART_STATE_BUSY_TX) {}
////    HAL_UART_Transmit(&hlpuart1, c, 1, HAL_MAX_DELAY);
//    while (!LL_USART_IsActiveFlag_TXE(LPUART1)) {
//        /* Wait active flag TXE */
//    }
//    LL_USART_TransmitData8(LPUART1, *(uint8_t*)c);
//}
//
//int _write(int file, char *ptr, int len) {
//    int DataIdx;
//    for (DataIdx = 0; DataIdx < len; DataIdx++) {
//        __io_putchar(*ptr++);
//    }
//    return len;
//}
///* USER CODE END 1 */
