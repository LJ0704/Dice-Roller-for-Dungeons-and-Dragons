/*
 * LCD_Fill_shapes.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "LCD_Fill_shapes.h"

/**
 * @brief     This function is used to fill screen with 16 bit color
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
void ST7789_Fill(uint16_t color)
{
	writecommand(ST7789_MADCTL);
	writedata(0xA0);
  //Likhita : Modified since we are using landscape mode
    writecommand(ST7789_RASET);
    writedata(0x00);
    writedata(0);        // x start
    writedata(0x00);
    writedata(0xEF);      // x end (0–239)

   // Likhita : Modified since we are using landscape mode
    writecommand(ST7789_CASET);
    writedata(0x00);
    writedata(0);        // y start
    writedata(0x01);
    writedata(0x3F);     // y end = 319 = 0x013F
    //Start writing into RAM
    writecommand(0x2C); // RAMWR
    DC_DATA();
    CS_LOW();

    uint8_t hi = color >> 8;
    uint8_t lo = color;

    for (uint32_t i = 0; i < 240 * 320; i++)
    {
        spi1_write(hi);  
        spi1_write(lo);
    }

    while (SPI1->SR & SPI_SR_BSY);   // Wait until SPI is not busy
    CS_HIGH();
}

/**
 * @brief     This function is used to set address window by setting hte Column address and row address.
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
void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    writecommand(ST7789_CASET);
    writedata(x0 >> 8); writedata(x0 & 0xFF);
    writedata(x1 >> 8); writedata(x1 & 0xFF);

    writecommand(ST7789_RASET);
    writedata(y0 >> 8); writedata(y0 & 0xFF);
    writedata(y1 >> 8); writedata(y1 & 0xFF);

    writecommand(0x2C);
}

/**
 * @brief     This function is used for filling rectangles .
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
 */
void ST7789_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if ((x >= 320) || (y >= 240)) return;
    if ((x + w - 1) >= 320) w = 320 - x;
    if ((y + h - 1) >= 240) h = 240 - y;

    ST7789_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    DC_DATA();
    CS_LOW();

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    uint32_t total = w * h;

    while (total--) {
        spi1_write(hi);
        spi1_write(lo);
    }

    CS_HIGH();
}

/**
 * @brief     This function is used for drawing rectangles.
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
 */
void ST7789_DrawRect(uint16_t x, uint16_t y,
                     uint16_t w, uint16_t h,
                     uint16_t color)
{
    for (uint16_t i = 0; i < w; i++)
    {
        ST7789_DrawPixel(x + i, y, color);          // Top
        ST7789_DrawPixel(x + i, y + h - 1, color);  // Bottom
    }
    for (uint16_t i = 0; i < h; i++)
    {
        ST7789_DrawPixel(x, y + i, color);          // Left
        ST7789_DrawPixel(x + w - 1, y + i, color);  // Right
    }
}
