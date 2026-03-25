/*
 * Onboard_LED_Init.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */


#include "Onboard_LED_Init.h"
/**
 * @brief     This function is used for initializing On board LEDs.
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, in class lab tutorial and Dean's book
 *
 * Author: Likhita Jonnakuti
 *
 */
void LED_gpio_initialization(void)
{
	// Enable GPIOD clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	// PD12 = output
	GPIOD->MODER &= ~(3 << (12*2));  // clear mode bits
	GPIOD->MODER |=  (1 << (12*2));  // set to output mode
	GPIOD->AFR[1] &= ~(0xF << 16);

	// PD13 = output
  	GPIOD->MODER &= ~(3 << (13*2));  // clear mode bits
  	GPIOD->MODER |=  (1 << (13*2));  // set to output mode
  	GPIOD->AFR[1] &= ~(0xF << 20);

  	//PD14
  	GPIOD->MODER &= ~(3 << (14*2));  // clear mode bits
  	GPIOD->MODER |=  (1 << (14*2));  // set to output mode
  	GPIOD->AFR[1] &= ~(0xF << 24);

  	//PD15
  	GPIOD->MODER &= ~(3 << (15*2));  // clear mode bits
  	GPIOD->MODER |=  (1 << (15*2));  // set to output mode
  	GPIOD->AFR[1] &= ~(0xF << 28);

  	//Set PD12, PD13, PD14 , PD15
  	GPIOD->BSRR |= GPIO_BSRR_BS13;
  	GPIOD->BSRR |= GPIO_BSRR_BS14;
  	GPIOD->BSRR |= GPIO_BSRR_BR12;
  	GPIOD->BSRR |= GPIO_BSRR_BS15;
}
