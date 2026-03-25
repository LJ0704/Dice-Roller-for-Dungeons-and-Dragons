/*
 * EEPROM.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "EEPROM.h"

#include "i2c.h"

#include "stm32f4xx.h"

/**
 * @brief     This function is used for reading a byte I2C
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
 * Modified the read sequence as per 24LC16B EEPROM : Likhita Jonnakuti
 *
 */
void EEPROM_ReadByte(uint16_t address, uint8_t *data)
{
	while (I2C1->SR2 & I2C_SR2_BUSY);           //wait until bus not busy
    uint8_t block = (address >> 8) & 0x07;
    uint8_t byte  = address & 0xFF;

    uint8_t devAddrWrite = 0xA0 | (block << 1);
    uint8_t devAddrRead  = devAddrWrite | 1;

	I2C1->CR1 |= I2C_CR1_START;                 //generate start

	while (!(I2C1->SR1 & I2C_SR1_SB)){;}	    //wait until start is generated

    I2C1->DR = devAddrWrite;
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){;}        //wait until address flag is set

	(void) I2C1->SR2; 						      //Clear SR2

	while (!(I2C1->SR1 & I2C_SR1_TXE)){;}           //Wait until Data register empty
	 I2C1->DR = byte;

	I2C1->CR1 |= I2C_CR1_START;                 //generate start

	while (!(I2C1->SR1 & I2C_SR1_SB)){;}	    //wait until start is generated

    I2C1->DR = devAddrRead;

	while(!(I2C1->SR1&I2C_SR1_ADDR)){;}			//Wait for address flag to set

	I2C1->CR1&=~I2C_CR1_ACK;					//Disable acknowledgment

	(void)I2C1->SR2;							// Clear SR2

	I2C1->CR1|=I2C_CR1_STOP;					// Generate stop condition

	while(!(I2C1->SR1&I2C_SR1_RXNE)){;}			//Wait for receiver buffer to be filled

	*data=I2C1->DR;								//Store the I2C bus.

}

/**
 * @brief     This function is used for write a byte I2C
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
 * Modified the read sequence as per 24LC16B EEPROM : Likhita Jonnakuti
 *
 */
void EEPROM_WriteByte(uint16_t address, uint8_t data)
{
	while (I2C1->SR2 & I2C_SR2_BUSY);           //wait until bus not busy

    uint8_t block = (address >> 8) & 0x07;     // A10–A8
    uint8_t byte  = address & 0xFF;            // A7–A0

    uint8_t devAddr = 0xA0 | (block << 1);

	I2C1->CR1 |= I2C_CR1_START;                 //generate start

	while (!(I2C1->SR1 & I2C_SR1_SB)){;}	    //wait until start is generated

	I2C1->DR = devAddr;

	while (!(I2C1->SR1 & I2C_SR1_ADDR)){;}        //wait until address flag is set

	(void) I2C1->SR2; 						      //Clear SR2

	while (!(I2C1->SR1 & I2C_SR1_TXE)){;}          //Wait until Data register empty

	 I2C1->DR = byte;

	while(!(I2C1->SR1&I2C_SR1_TXE)){;}			//Wait until Data register empty

	I2C1->DR = data; 							//Write the data to the EEPROM

	while (!(I2C1->SR1 & I2C_SR1_BTF));      	//wait until transfer finished

	I2C1->CR1 |=I2C_CR1_STOP;					//Generate Stop

}

