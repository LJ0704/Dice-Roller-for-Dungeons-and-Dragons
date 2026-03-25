/*
 * LCD_Pixel_Draw.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "LCD_Pixel_Draw.h"

/**
 * @brief     This function is used to draw pixel at (x,y) with a certain color.
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference : 1) Working with STM32F4 and SPI-TFT: ST7789 IPS 240×240 LCD by Husamuldeen
 *             https://blog.embeddedexpert.io/?p=1215
 *             2) Github Repository : Majid Derhambakhsh
 *             https://github.com/Majid-Derhambakhsh/ST7789/blob/main/Src/st7789.c
 *
 * Original Author: Majid Derhambakhsh (Github Repository) and Husamuldeen (Embedded Expert IO blog)
 *
 * Modified by: Likhita Jonnakuti
 *
 */
void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    writecommand(ST7789_CASET);
    writedata(x >> 8); writedata(x & 0xFF);
    writedata(x >> 8); writedata(x & 0xFF);

    writecommand(ST7789_RASET);
    writedata(y >> 8); writedata(y & 0xFF);
    writedata(y >> 8); writedata(y & 0xFF);

    writecommand(0x2C);

    DC_DATA();
    CS_LOW();
    spi1_write(color >> 8);
    spi1_write(color & 0xFF);
    CS_HIGH();
}
