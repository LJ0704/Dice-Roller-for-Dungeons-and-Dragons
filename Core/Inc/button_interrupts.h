/*
 * button_interrupts.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#ifndef INC_BUTTON_INTERRUPTS_H_
#define INC_BUTTON_INTERRUPTS_H_

#include <stm32f411xe.h>

void exti0_initialization(void);
void exti1_initialization(void);
void exti2_initialization(void);
void gpio_button_initialization(void);
void button_interrupt_initialization(void);

#endif /* INC_BUTTON_INTERRUPTS_H_ */
