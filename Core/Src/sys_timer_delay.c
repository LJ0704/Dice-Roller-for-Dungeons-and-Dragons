/*
 * sys_timer_delay.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "sys_timer_delay.h"


/**
 * @brief     This function is used for micro second delay using Systick timer
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, Dean's book and PES Assignment - 4
 *
 * Author: Likhita Jonnakuti
 *
 */
void delayuS(int ms)
{

	SysTick->LOAD=100-1;
	SysTick->VAL=0;
	SysTick->CTRL=0x5;
		for (int i=0;i<ms;i++)
		{
			while(!(SysTick->CTRL &0x10000)){}
		}
	SysTick->CTRL=0;



	}

/**
 * @brief     This function is used for milli second delay using Systick timer
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, Dean's book and PES Assignment - 4
 *
 * Author: Likhita Jonnakuti
 *
 */
void delay_ms(int ms)
{

	SysTick->LOAD=100000-1;
	SysTick->VAL=0;
	SysTick->CTRL=0x5;
		for (int i=0;i<ms;i++)
		{
			while(!(SysTick->CTRL &0x10000)){}
		}
	SysTick->CTRL=0;

}
