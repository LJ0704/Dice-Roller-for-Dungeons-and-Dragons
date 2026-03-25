/*
 * i2c.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */


#include "i2c.h"

/**
 * @brief     This function is used for initializing I2C
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference : 1) STM32 Reference Manual
 *             2) https://github.com/hussamaldean
 *             3) ST7789 Datasheet
 *
 *
 * Original Author: original author: Husamuldeen <https://github.com/hussamaldean>
 *
 */
void i2c_init(void){

	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN; //enable gpiob clock
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN; //enable i2c1 clock
	GPIOB->MODER|=0xA0000; //set pb8and9 to alternative function
	GPIOB->AFR[1]|=0x44;
	GPIOB->OTYPER|=GPIO_OTYPER_OT8|GPIO_OTYPER_OT9; //set pb8 and pb9 as open drain
	I2C1->CR1=I2C_CR1_SWRST;
	I2C1->CR1&=~I2C_CR1_SWRST;
	I2C1->CR2|=16;
	I2C1->CCR=80;
	I2C1->TRISE=17; //output max rise
	I2C1->CR1|=I2C_CR1_PE;

}
