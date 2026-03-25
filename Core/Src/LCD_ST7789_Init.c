/*
 * LCD_ST7789_Init.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "LCD_ST7789_Init.h"
#include "sys_timer_delay.h"

/**
 * @brief     This function is used for initializing GPIO pins used by ST7789 LCD Display.
 *            PA4 is used as Chip Select pin
 *            PB0 is used as Reset Pin
 *            PB1 is used as Data/Command pin
 *            PB3 is used for Back light
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
 * Author: Likhita Jonnakuti
 *
 */
void gpio_init_ST7789(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
	//PA4 = CS
	GPIOA->MODER &= ~(3U << (4*2)); //clear moder
	GPIOA->MODER |= (1U << (4*2));
	GPIOA->BSRR = CS_PIN;// CS HIGH i.e, Inactive

	//PB0 - RST
	GPIOB->MODER &= ~(3U << (0*2)); //clear moder
	GPIOB->MODER |= (1U << (0*2));
	GPIOB->BSRR = RST_PIN; // RST high

	//PB1 = DC
	GPIOB->MODER &= ~(3U << (1*2)); //clear moder
	GPIOB->MODER |= (1U << (1*2));
	GPIOB->BSRR = DC_PIN; //Data being set default

	//PB3 = BL
	GPIOB->MODER &= ~(3U << (3*2)); //clear moder
	GPIOB->MODER |= (1U << (3*2));
	GPIOB->BSRR = BL_PIN; //Back Light ON
}


/**
 * @brief     This function is used for initializing ST7789 LCD Display.
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
 * Author: Likhita Jonnakuti
 *
 */
void ST7789_Init(void)
{
	ST7789_Reset();
	writecommand(ST7789_SWRESET);

    writecommand(ST7789_MADCTL); // MADCTL
    writedata(0x00);    // RGB

    writecommand(ST7789_RAMCTRL); // RAMCTRL
    writedata(0x00);
    writedata(0xE0); // 5 to 6-bit conversion: r0 = r5, b0 = b5

    //Pixel Format - 16-bit RGB565
    writecommand(ST7789_COLMOD); // COLMOD
    writedata(0x55);    // 16-bit color
    delay_ms(10);

    //Porch Settings
    writecommand(ST7789_PORCTRL); // PORCTRL
    writedata(0x0C);
    writedata(0x0C);
    writedata(0x00);
    writedata(0x33);
    writedata(0x33);

    writecommand(ST7789_GCTRL); // GCTRL
    writedata(0x35);

    writecommand(ST7789_VCOMS); // VCOMS
    writedata(0x20);

    writecommand(ST7789_LCMCTRL); // LCMCTRL
    writedata(0x2C);

    writecommand(ST7789_VDVVRHEN); // VDVVRHEN
    writedata(0x01);
    // writedata(0xFF);

    writecommand(ST7789_VRHS); // VRHS
    writedata(0x0B);

    writecommand(ST7789_VDVSET); // VDVSET
    writedata(0x20);

    //Frame Rate Control
    writecommand(ST7789_FRCTR2); // FRCTR2
    writedata(0x0F);

    writecommand(ST7789_PWCTRL1); // PWCTRL1
    writedata(0xA4);
    writedata(0xA1);
    
    // Positive gamma
    writecommand(ST7789_PVGAMCTRL);
    writedata(0xd0);
    writedata(0x04);
    writedata(0x0D);
    writedata(0x11);
    writedata(0x13);
    writedata(0x2B);
    writedata(0x3F);
    writedata(0x54);
    writedata(0x4C);
    writedata(0x18);
    writedata(0x0D);
    writedata(0x0B);
    writedata(0x1F);
    writedata(0x23);

    // Negative gamma
    writecommand(ST7789_NVGAMCTRL);
    writedata(0xd0);
    writedata(0x04);
    writedata(0x0C);
    writedata(0x11);
    writedata(0x13);
    writedata(0x2C);
    writedata(0x3F);
    writedata(0x44);
    writedata(0x51);
    writedata(0x2F);
    writedata(0x1F);
    writedata(0x1F);
    writedata(0x20);
    writedata(0x23);

    writecommand(ST7789_INVON); // INVON
    writecommand(ST7789_SLPOUT); // SLPOUT
    delay_ms(150);
    writecommand(ST7789_NORON); // NORON
    delay_ms(20);
    writecommand(ST7789_CASET); // CASET
    writedata(0x00);
    writedata(0x00);
    writedata(0x00);
    writedata(0xEF);

    writecommand(ST7789_RASET); // RASET
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);

    writecommand(ST7789_DISPON); // DISPON
    delay_ms(50);

}


/**
 * @brief     This function is used for Reset ST7789 LCD Display.
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
 * Author: Likhita Jonnakuti
 *
 */
void ST7789_Reset(void)
{
	delay_ms(25);
    GPIOB->BSRR = (RST_PIN << 16); // RST low
    delay_ms(50);
    GPIOB->BSRR = RST_PIN;         // RST high
    delay_ms(50);
}
