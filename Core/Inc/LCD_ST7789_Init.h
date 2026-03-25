/*
 * LCD_ST7789_Init.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#ifndef INC_LCD_ST7789_INIT_H_
#define INC_LCD_ST7789_INIT_H_

#include <stm32f411xe.h>
#include "LCD_write.h"


//Pin bit masks
#define CS_PIN   (1U << 4)   // PA4
#define DC_PIN   (1U << 1)   // PB1
#define RST_PIN  (1U << 0)   // PB0
#define BL_PIN   (1U << 3)   // PB3



//LCD Commands
#define ST7789_SLPOUT 0x11
#define ST7789_NORON 0x13
#define ST7789_MADCTL 0x36

#define ST7789_RAMCTRL 0xB0
#define ST7789_COLMOD 0x3A
#define ST7789_PORCTRL 0xB2
#define ST7789_GCTRL 0xB7
#define ST7789_VCOMS 0xBB
#define ST7789_LCMCTRL 0xC0
#define ST7789_VDVVRHEN 0xC2
#define ST7789_VRHS 0xC3
#define ST7789_VDVSET 0xC4
#define ST7789_FRCTR2 0xC6
#define ST7789_PWCTRL1 0xD0
#define ST7789_PVGAMCTRL 0xE0
#define ST7789_NVGAMCTRL 0xE1
#define ST7789_INVON 0x21
#define ST7789_CASET 0x2A
#define ST7789_RASET 0x2B
#define ST7789_DISPON 0x29
#define ST7789_SWRESET 0x01

#define TFT_MAD_RGB 0x00
#define TFT_MAD_BGR 0x08

void gpio_init_ST7789(void);
void ST7789_Init(void);
void ST7789_Reset(void);

#endif /* INC_LCD_ST7789_INIT_H_ */
