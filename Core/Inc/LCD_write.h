/*
 * LCD_command.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#ifndef INC_LCD_WRITE_H_
#define INC_LCD_WRITE_H_

#include <stm32f411xe.h>
#include "LCD_ST7789_Init.h"

void writecommand(uint8_t cmd);
void writedata(uint8_t data);
void spi1_write(uint8_t d);
void DC_DATA(void);
void DC_CMD(void);
void CS_HIGH(void);
void CS_LOW(void);
#endif /* INC_LCD_WRITE_H_ */
