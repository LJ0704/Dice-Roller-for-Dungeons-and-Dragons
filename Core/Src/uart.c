/**
  ******************************************************************************
  * @file           : uart.c
  * @brief          : UART and Command Interface Initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) December 1st, 2025 Kenneth Alcineus.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"
#include "uart.h"

#define MAX_CAPACITY 2048
#define BAUD_9600 0x09C4

uint8_t tx_buffer[MAX_CAPACITY];
uint8_t rx_buffer[MAX_CAPACITY];

int tx_writepos = 0;
int tx_readpos = 0;
int tx_isfull = 0;
int tx_isempty = 1;

int rx_writepos = 0;
int rx_readpos = 0;
int rx_isfull = 0;
int rx_isempty = 1;

//overrides the weak __io_putchar function to get stdlib output functions working
int __io_putchar(int ch)
{
	if (!tx_isfull)
	{
		tx_buffer[tx_writepos++] = (uint8_t)ch;

		if (tx_isempty)
		{
			tx_isempty = 0;
		}
		if (tx_writepos == MAX_CAPACITY)
		{
			tx_writepos = 0;
		}
		if (tx_writepos == tx_readpos)
		{
			tx_isfull = 1;
		}
		if (!(USART2->CR1 & USART_CR1_TXEIE))
		{
			USART2->CR1 |= USART_CR1_TXEIE;
		}
		return 1;
	}
	return -1;
}

//overrides the weak __io_putchar function to get stdlib input functions working
int __io_getchar(void)
{
	if (!rx_isempty)
	{
		uint8_t ch = rx_buffer[rx_readpos++];
		rx_buffer[rx_readpos - 1] = 0;

		if (rx_isfull)
		{
			rx_isfull = 0;
		}
		if (rx_readpos == MAX_CAPACITY)
		{
			rx_readpos = 0;
		}
		if (rx_writepos == rx_readpos)
		{
			rx_isempty = 1;
		}
		if (!(USART2->CR1 & USART_CR1_RXNEIE))
		{
			USART2->CR1 |= USART_CR1_RXNEIE;
		}
		return (int)ch;
	}
	return -1;
}

//USART ISR to write rx_buffer with data register contents or write data register with tx_buffer contents
void USART2_IRQHandler(void)
{
	uint8_t byte;
	if ((USART2->SR & USART_SR_RXNE) && (USART2->CR1 & USART_CR1_RXNEIE))
	{
		if (!rx_isfull)
		{
			byte = (uint8_t)USART2->DR;
			rx_buffer[rx_writepos++] = byte;

			if (rx_isempty)
			{
				rx_isempty = 0;
			}
			if (rx_writepos == MAX_CAPACITY)
			{
				rx_writepos = 0;
			}
			if (rx_writepos == rx_readpos)
			{
				rx_isfull = 1;
			}
		}
		else
		{
			//rx buffer full so disable rx interrupts
			USART2->CR1 &= ~(USART_CR1_RXNEIE);
		}
	}
	if ((USART2->SR & USART_SR_TXE) && (USART2->CR1 & USART_CR1_TXEIE))
	{
		int tx_empty_flag = 1;
		for (int i = 0; i < MAX_CAPACITY; i++)
		{
			if (tx_buffer[i] != 0)
			{
				tx_empty_flag = 0;
			}
		}
		tx_isempty = tx_empty_flag;
		if (!tx_isempty)
		{
			byte = tx_buffer[tx_readpos++];
			tx_buffer[tx_readpos - 1] = 0;
			USART2->DR = (uint32_t)(byte & 0xFF);

			if (tx_isfull)
			{
				tx_isfull = 0;
			}
			if (tx_readpos == MAX_CAPACITY)
			{
				tx_readpos = 0;
			}
			if (tx_writepos == rx_readpos)
			{
				tx_isempty = 1;
			}
		}
		else
		{
			//tx buffer empty so disable tx interrupts
			USART2->CR1 &= ~(USART_CR1_TXEIE);
		}
	}
}

//The following USART Initialization function is attributed to Nalin Saxena
void UART_Init()
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // send clock for usart
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //send clock to register A

	// we need to set PA2 and PA3 as alternate moder
	GPIOA->MODER |= 0b10 << GPIO_MODER_MODER2_Pos;
	GPIOA->MODER |= 0b10 << GPIO_MODER_MODER3_Pos;

	//now Pa2 to USART_TX PA3 to Rx  making 7
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2; //tx
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2; //rx

	//enable USART
	USART2->CR1 |= USART_CR1_UE;

	//enable receive and transfer interrupts
	USART2->CR1 |= USART_CR1_TXEIE;
	USART2->CR1 |= USART_CR1_RXNEIE;

	NVIC_EnableIRQ(USART2_IRQn);
	USART2->BRR = BAUD_9600;

	//enable transmitter and receiver
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;
}
