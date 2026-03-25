/*
 * SPI_Init.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */


#include "SPI_Init.h"

/**
 * @brief     This function is used for initializing GPIO pins for SPI 1
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, and Dean's book  Listing 8.5 Code to initialize SPI1 peripheral
 *
 * Author: Likhita Jonnakuti
 *
 */
void SPI1_GPIO_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	//PA5 - SCK (AF5)
	GPIOA->MODER &= ~(3U << (5*2));//clear moder
	GPIOA->MODER |= (2U << (5*2)); //AF mode
	GPIOA->AFR[0] &= ~(0xF << (5*4));// clear AF
	GPIOA->AFR[0] |= (5U << (5*4)); // SPI SCK -> AF5


	//PA7 - MOSI (AF5)
    GPIOA->MODER &= ~(3U << (7*2));// clear moder
	GPIOA->MODER |= (2U << (7*2)); //AF mode
	GPIOA->AFR[0] &= ~(0xF << (7*4));// clear AF
	GPIOA->AFR[0] |= (5U << (7*4));// SPI SCK -> AF5
}

/**
 * @brief     This function is used for initializing SPI 1
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, and Dean's book  Listing 8.5 Code to initialize SPI1 peripheral
 *
 * Author: Likhita Jonnakuti
 *
 */
void SPI1_Init(void)
{
	//SPI1 CLK enable
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 = 0;
	SPI1->CR2 = 0;

	//CR1 Settings
	SPI1->CR1 |= SPI_CR1_MSTR; //Master mode
	SPI1->CR1 |= SPI_CR1_SSM;  // Software NSS
	SPI1->CR1 |= SPI_CR1_SSI; // Internal NSS high

	//Mode 0
	SPI1->CR1 |= SPI_CR1_CPOL;  // CPOL=0
	SPI1->CR1 |= SPI_CR1_CPHA;  // CPHA=0

	//Baud Rate = fclk/8
	SPI1->CR1 |= SPI_CR1_BR_0;

	SPI1->CR1 |= SPI_CR1_SPE;
}
