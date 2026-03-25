/**
  ******************************************************************************
  * @file           : uart.h
  * @brief          : UART and Command Interface Initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Kenneth Alcineus.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef INC_UART_H_
#define INC_UART_H_

int __io_putchar(int ch);
int __io_getchar(void);
void USART2_IRQHandler(void);
void UART_Init();

#endif /* INC_UART_H_ */
