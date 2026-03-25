/*
 * LCD_Fill_shapes.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#ifndef INC_LCD_FILL_SHAPES_H_
#define INC_LCD_FILL_SHAPES_H_

#include <stm32f411xe.h>
#include "LCD_write.h"
#include "LCD_Pixel_Draw.h"

#define WHITE 0xFFFF
#define AQUA 0x7FFF
#define RED 0xF800
#define BROWN 0XBC40
#define PURPLE 0x780F
#define YELLOW 0xFFE0

void ST7789_Fill(uint16_t color);
void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ST7789_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7789_DrawRect(uint16_t x, uint16_t y,uint16_t w, uint16_t h,uint16_t color);


#endif /* INC_LCD_FILL_SHAPES_H_ */
