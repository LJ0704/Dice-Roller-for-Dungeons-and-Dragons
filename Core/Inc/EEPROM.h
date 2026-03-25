/*
 * EEPROM.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "stdint.h"

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
void EEPROM_ReadByte(uint16_t address, uint8_t *data);

/**
 * @brief     This function is used for writing a byte I2C
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
void EEPROM_WriteByte(uint16_t address, uint8_t data);



#endif /* EEPROM_H_ */
