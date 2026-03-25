/*
 * LCD_Text_Display.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "LCD_Text_Display.h"



/**
 * @brief     This function is used for selecting LCD device by pulling Chip Select LOW.
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference : 1) STM32 Reference Manual
 *             2) Working with STM32F4 and SPI-TFT: ST7789 IPS 240×240 LCD by Husamuldeen
 *             https://blog.embeddedexpert.io/?p=1215
 *             3) Github Repository : Majid Derhambakhsh
 *             https://github.com/Majid-Derhambakhsh/ST7789/blob/main/Src/st7789.c
 *             4) ST7789 Datasheet
 *
 *
 * Original Author: Majid Derhambakhsh (Github Repository) and Husamuldeen (Embedded Expert IO blog)
 *
 * Modified the character draw based on input parameter size
 *
 */
void ST7789_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t size)
{
    if (c < 32 || c > 127) return;

    const uint8_t *bitmap = font5x7[c - 32];

    for (int col = 0; col < 5; col++)
    {
        uint8_t bits = bitmap[col];

        for (int row = 0; row < 7; row++)
        {
            if (bits & (1 << row))
            {
                // Draw a block size instead of 1 pixel
                for (int dx = 0; dx < size; dx++)
                {
                    for (int dy = 0; dy < size; dy++)
                    {
                        ST7789_DrawPixel(x + col * size + dx,
                                         y + row * size + dy,
                                         color);
                    }
                }
            }
        }
    }
}

/**
 * @brief     This function is used for selecting LCD device by pulling Chip Select LOW.
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference : 1) STM32 Reference Manual
 *             2) Working with STM32F4 and SPI-TFT: ST7789 IPS 240×240 LCD by Husamuldeen
 *             https://blog.embeddedexpert.io/?p=1215
 *             3) Github Repository : Majid Derhambakhsh
 *             https://github.com/Majid-Derhambakhsh/ST7789/blob/main/Src/st7789.c
 *             4) ST7789 Datasheet
 *
 *
 * Original Author: Majid Derhambakhsh (Github Repository) and Husamuldeen (Embedded Expert IO blog)
 *
 * Modified: Likhita Jonnakuti added space after pixel draw
 *
 */
void ST7789_DrawString(uint16_t x, uint16_t y, const char *s, uint16_t color, uint8_t size)
{
    while (*s)
    {
        ST7789_DrawChar(x, y, *s, color, size);
        x += (5 * size) + size;
        s++;
    }
}
