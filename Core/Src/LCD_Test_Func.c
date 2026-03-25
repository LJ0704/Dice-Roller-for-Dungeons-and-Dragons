/*
 * LCD_Test_Func.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "LCD_Test_Func.h"

/**
 * @brief     This function is used test the LCD by filling with different colors in sequence.
 *
 * @param[in]  NA
 *
 * @return     NA
 *Author: Likhita Jonnakuti
 *
 */
void LCD_test(void)
{
  ST7789_Fill(0x001F);

  	ST7789_Fill(0xFFFF);
  	  		delay_ms(300);
  	  		ST7789_Fill(0x0000);
  	  		delay_ms(300);
  	  		ST7789_Fill(0x7FFF);
  	  		delay_ms(300);
  	  		ST7789_Fill(0xFFE0);
  	  		delay_ms(300);
  	  		ST7789_Fill(0X5458);
  	  		delay_ms(300);
  	  		ST7789_Fill(0X841F);
  	  		delay_ms(300);
  	  		ST7789_Fill(0XBC40);
  	  		delay_ms(300);
  	  		ST7789_Fill(0X07FF);
  	  		delay_ms(300);
  	  		ST7789_Fill(0xF81F);
  	  		delay_ms(300);

  	  	ST7789_Fill(0x0000);  // Black screen
  	  delay_ms(300);
}
