/*
 * LCD_Image_Display.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#ifndef INC_LCD_IMAGE_DISPLAY_H_
#define INC_LCD_IMAGE_DISPLAY_H_

#include <stm32f411xe.h>
#include "LCD_write.h"

void ST7789_DrawFullScreen(const uint16_t *img);
void ST7789_SetFullScreen(void);
extern const uint16_t myImage[45*50*50];
#endif /* INC_LCD_IMAGE_DISPLAY_H_ */
