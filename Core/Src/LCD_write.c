/*
 * LCD_write.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "LCD_write.h"
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
 * Author: Likhita Jonnakuti
 *
 */
void CS_LOW(void)
{
	GPIOA->BSRR = (CS_PIN << 16);
}

/**
 * @brief     This function is used for not selecting LCD device by pulling Chip Select HIGH.
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
 * Author: Likhita Jonnakuti
 *
 */
void CS_HIGH(void)
{
	GPIOA->BSRR = CS_PIN;
}

/**
 * @brief     This function is used for Sending Command instruction to LCD.
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
 * Author: Likhita Jonnakuti
 *
 */
void DC_CMD(void)
{
	GPIOB->BSRR = (DC_PIN << 16);
}

/**
 * @brief     This function is used for Sending Data to LCD.
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
 * Author: Likhita Jonnakuti
 *
 */
void DC_DATA(void)
{
	GPIOB->BSRR = DC_PIN;
}


/**
 * @brief     This function is used for doing SPI write.
 *
 * @param[in]  d, data to be written over SPI
 *
 * @return     NA
 *
 * Reference : STM32 Reference Manual and Dean's Book Listing 8.4 Code to transmit and receive one byte with SPI using polling.
 *
 * Author: Likhita Jonnakuti
 *
 */
void spi1_write(uint8_t d)
{
    volatile uint8_t dummy;

    while (!(SPI1->SR & SPI_SR_TXE));  // TX buffer empty
    SPI1->DR = d;                      // Write data

    while (!(SPI1->SR & SPI_SR_RXNE)); // Wait until RX full
    dummy = SPI1->DR;                  // Read to clear RXNE
    while ((SPI1->SR & SPI_SR_BSY));
}


/**
 * @brief     This function is used for writing LCD command .
 *
 * @param[in] cmd, command to be given to the LCD
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
 * Author: Likhita Jonnakuti
 *
 */
void writecommand(uint8_t cmd)
{
    DC_CMD();     // command mode
    CS_LOW();
    spi1_write(cmd);
    CS_HIGH();
}

/**
 * @brief     This function is used for writing LCD data.
 *
 * @param[in]  data, the data to be written to the LCD
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
 * Author: Likhita Jonnakuti
 *
 */
void writedata(uint8_t data)
{
    DC_DATA();    // data mode
    CS_LOW();
    spi1_write(data);
    CS_HIGH();
}
