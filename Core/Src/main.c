/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) December 1st, 2025 Kenneth Alcineus, Likhita Jonnakuti.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <command.h>
#include "game_definitions.h"
#include "game_logic.h"
#include "main.h"
#include <stdio.h>
#include "uart.h"
#include "EEPROM.h"
#include "i2c.h"
#include <stm32f411xe.h>
#include <string.h>
#include "button_interrupts.h"
#include "SPI_Init.h"
#include "LCD_ST7789_Init.h"
#include "sys_timer_delay.h"
#include "LCD_write.h"
#include "LCD_Text_Display.h"
#include "Onboard_LED_Init.h"
#include "LCD_Pixel_Draw.h"
#include "LCD_Fill_shapes.h"
#include "LCD_Test_Func.h"
#include "LCD_Game_Display.h"
#include "LCD_Image_Display.h"

int static press_button_0 = 0;
int static press_button_1 = 0;

int static press_button_2 = 0;
volatile int event_ok = 0;
volatile int event_up = 0;
volatile int event_down = 0;


volatile int flag_ok = 0;
volatile int flag_left = 0;
volatile int flag_right = 0;
//states
#define PROGRAM_START 0
#define DICE_ROLL 1
#define COMBAT 2
#define ABILITY_CHECK 3
#define SERIAL 4

int deterministic_state = PROGRAM_START;

char names_character[10][20] = {
    "Likhita", "Vamsi", "Uday", "Appu", "Visali",
    "Sailesh", "Manasa", "Sailu", "PremSai", "Kasi"
};

//int top_index = 0;      // The first visible item in the scrolling window
int visible_items = 5;  // How many items to display (adjustable)

//volatile int cursor_index = 0;        // 0–9 = names, 10 = Confirm, 11 = Cancel
//volatile int selected_index = -1;     // -1 = no selection

//parameters (TBD)

//other define statements

//interrupt-driven global variables
volatile int button_flag = 0;
volatile int lcd_roll_prompt_selection= 0;
volatile int lcd_roll_prompt_selection_ok_pressed_flag = 0;
volatile int lcd_down_selection = 0;
volatile int lcd_down_selection_changed = 0;
volatile int lcd_roll_prompt_selection_changed = 0;
volatile int return_to_program_start_flag =0;
volatile int lcd_combat_end_flag = 0;
volatile int led_flag = 1;

uint8_t data_read;

uint8_t data_write=0x58;

uint16_t address=0x0123;

uint16_t address_1=0x0127;

void EXTI0_IRQHandler(void)
{
	for(int i =0 ; i<1000000; i++);
   if (EXTI->PR & (1 << 0))
   {
       EXTI->PR |= (1 << 0);     // Clear pending bit
   }

  button_flag = 1;
  led_flag = 1;
  lcd_roll_prompt_selection_ok_pressed_flag = 1;

  lcd_combat_end_flag = 1;
  return_to_program_start_flag = 1;

  flag_ok = 1;
  if (press_button_0 == 0)
  {
  GPIOD->BSRR=GPIOD->BSRR |(GPIO_BSRR_BS13_Msk);
  press_button_0 = 1;
  }
  else if(press_button_0 == 1)
  {
  GPIOD->BSRR=GPIOD->BSRR |(GPIO_BSRR_BR13_Msk);
  press_button_0 = 0;
  }
}

//the following EXTI0_Init function is attributed to Aditya Ganesh
void EXTI0_Init(void)
{
  SYSCFG->EXTICR[0] &= ~(0xF << 0);  // Clear EXTI0 bits

  SYSCFG->EXTICR[0] |=  (0x0 << 0);  // PA0 → EXTI0

  EXTI->IMR  |= (1 << 0);   // Unmask line 0

  EXTI->RTSR |= (1 << 0);   // Rising edge trigger

  EXTI->FTSR &= ~(1 << 0);  // Disable falling edge

  NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI1_IRQHandler(void) {
	for(int i =0 ; i<1000000; i++);

    if (EXTI->PR & (1 << 1)) {
        EXTI->PR |= (1 << 1);     // Clear pending bit

        button_flag = 1;
    lcd_roll_prompt_selection++;

    lcd_roll_prompt_selection_changed = 1;
    flag_left = 1;
// PD15 -
    //display_once = 0;
        event_down = 1;
        lcd_down_selection = 1;
    if (press_button_1 == 0)
    {
    GPIOD->BSRR=GPIOD->BSRR |(GPIO_BSRR_BS15_Msk);
    press_button_1 = 1;
    }
    else if(press_button_1 == 1)
    {
    GPIOD->BSRR=GPIOD->BSRR |(GPIO_BSRR_BR15_Msk);
    press_button_1 = 0;
    }
    }
}

void EXTI2_IRQHandler(void) {
	for(int i =0 ; i<1000000; i++);

	flag_right = 1;
    if (EXTI->PR & (1 << 2)) {
        EXTI->PR |= (1 << 2);     // Clear pending bit
        button_flag = 1;
     event_up = 1;
    lcd_roll_prompt_selection--;

    lcd_roll_prompt_selection_changed = 1;

    //display_once_2 = 0;
    if (press_button_2 == 0)
    {
    GPIOD->BSRR=GPIOD->BSRR |(GPIO_BSRR_BS13_Msk);
    press_button_2 = 1;
    }
    else if(press_button_2 == 1)
    {
    GPIOD->BSRR=GPIOD->BSRR |(GPIO_BSRR_BR13_Msk);
    press_button_2 = 0;
    }
    }
}
void BUTTON_Init(void)
{
	  RCC->AHB1ENR |= (1 << 0);   // enable GPIOA

	  RCC->APB2ENR |= (1 << 14);
}

void led_state(int state)
{
	if (led_flag)
	{
		if (state == PROGRAM_START)
		{
			  GPIOD->ODR |= (GPIO_BSRR_BS13_Msk);
			  GPIOD->ODR &= ~(GPIO_BSRR_BS14_Msk);
			  GPIOD->ODR &= ~(GPIO_BSRR_BS15_Msk);
		}
		else if (state == DICE_ROLL)
		{
	  		  GPIOD->ODR &= ~(GPIO_BSRR_BS13_Msk);
	  		  GPIOD->ODR |= (GPIO_BSRR_BS14_Msk);
	  		  GPIOD->ODR &= ~(GPIO_BSRR_BS15_Msk);
		}
		else if (state == COMBAT)
		{
	  		  GPIOD->ODR &= ~(GPIO_BSRR_BS13_Msk);
	  		  GPIOD->ODR &= ~(GPIO_BSRR_BS14_Msk);
	  		  GPIOD->ODR |= (GPIO_BSRR_BS15_Msk);
		}
		else if (state == SERIAL)
		{
	  		  GPIOD->ODR &= ~(GPIO_BSRR_BS13_Msk);
	  		  GPIOD->ODR &= ~(GPIO_BSRR_BS14_Msk);
	  		  GPIOD->ODR &= ~(GPIO_BSRR_BS15_Msk);
		}
		led_flag = 0;
	}

}

void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  SystemClock_Config();
  //LED_Init();
  //EXTI0_Init();
 // BUTTON_Init();
  UART_Init();

  //LCD integration
  LED_gpio_initialization();
  gpio_button_initialization();
 // exti1_initialization();
  button_interrupt_initialization();
  gpio_init_ST7789();
  SPI1_GPIO_Init();
  SPI1_Init();
  ST7789_Init();
//  LCD_test();
  ST7789_Fill(0xFFFF);
  delay_ms(300);
  draw_centered_string(120, "D&D",PURPLE, 8);
  ST7789_DrawFullScreen(myImage);
  delay_ms(3000);
  //EEPROM integration
  i2c_init();






	char input_str[MAX_STR_LEN];


  while (1)
  {
	  button_flag = 0;
	  deterministic_state = lcd_roll_prompt_selection + 1000;
	  switch ((deterministic_state % 5))
	  {
	  	  case PROGRAM_START:
	  		program_start_mode_lcd();

	  		  if (flag_ok)
	  		  {
	  			flag_ok = 0;
	  			  break;
	  		  }
	  		  break;
	  	  case DICE_ROLL:
	  		dice_roller_mode_lcd();
	  		  if (flag_ok)
			  {
				  flag_ok = 0;
				  roll_anything();
				  break;
			  }
	  		  break;
	  	  case COMBAT:
	  		combat_mode_lcd();
	  		  if (flag_ok)
			  {
				  flag_ok = 0;
		  			if (set_turn_order() != 0)
		  			{
		  				combat_turn();
		  			}

				  break;
			  }
	  		  break;
	  	  case ABILITY_CHECK:
	  		ability_check_mode_lcd();
	  		  if (flag_ok)
	  		  {
	  			flag_ok = 0;
	  			ability_check();
	  			  break;
	  		  }
	  		  break;
	  	  case SERIAL:
	  		serial_mode_lcd();


	  		  if (flag_ok)
	  		  {

	  			for (int i = 0; i < MAX_STR_LEN; i++)
				{
					input_str[i] = 0;
				}

				printf("$$ ");
				accumulate_line(input_str);
				process_command(input_str);
				break;
	  		  }
			break;
	  	  default:
	  		  deterministic_state = PROGRAM_START;
	  		  break;
	  }
	  while (button_flag == 0){;}
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
