/*
 * LCD_Pixel_Draw.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#ifndef INC_LCD_PIXEL_DRAW_H_
#define INC_LCD_PIXEL_DRAW_H_
#include <stm32f411xe.h>
#include "LCD_ST7789_Init.h"
#include "LCD_write.h"

void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color);


#endif /* INC_LCD_PIXEL_DRAW_H_ */
