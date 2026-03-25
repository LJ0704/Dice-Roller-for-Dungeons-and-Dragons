/*
 * button_interrupts.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#include "button_interrupts.h"

/**
 * @brief     This function is used for initializing External Interrupt 0 which can get triggered using button.
 *            It is used to act as OK/Select button.
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, in class lab tutorial and Dean's book
 *
 * Author: Likhita Jonnakuti
 */
void exti0_initialization(void) {
	//Enable SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    //Mapping the PC0 -> EXTI0
    SYSCFG->EXTICR[0] &= ~(0xF << 0);  // Clear EXTI0 bits
    SYSCFG->EXTICR[0] |=  (0x2 << 0);  // Mapping PA0 EXTI0

    //Configure external interrupt
    EXTI->IMR  |= (1 << 0);   // Unmask line 0
    EXTI->RTSR |= (1 << 0);   // Rising edge trigger
    EXTI->FTSR &= ~(1 << 0);  // Disable falling edge

    //Enable EXTI0 interrupt in NVIC
    NVIC_EnableIRQ(EXTI0_IRQn);
}



/**
 * @brief     This function is used for initializing External Interrupt 1 which can get triggered using button.
 *            It is used to navigate down in the menu
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, in class lab tutorial and Dean's book
 *
 * Author: Likhita Jonnakuti
 *
 */
void exti1_initialization(void) {

    //Enable SYSCFG clock already done as a part of exti0_initialization

    //Mapping the PE1 -> EXTI1
    SYSCFG->EXTICR[0] &= ~(0xF << 4);  // Clear EXTI1 bits
    SYSCFG->EXTICR[0] |=  (0x2 << 4);  // Mapping PE1 EXTI1

    //Configure external interrupt
    EXTI->IMR  |= (1 << 1);   // Unmask line 1
    EXTI->RTSR |= (1 << 1);   // Rising edge trigger
    EXTI->FTSR &= ~(1 << 1);  // Disable falling edge

    //Enable EXTI1 interrupt in NVIC
    NVIC_EnableIRQ(EXTI1_IRQn);
}

/**
 * @brief     This function is used for initializing External Interrupt 2 which can get triggered using button.
 *            It is used to navigate up in the menu
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, in class lab tutorial and Dean's book
 *
 * Author: Likhita Jonnakuti
 *
 */
void exti2_initialization(void) {

	//Enable SYSCFG clock already done as a part of exti0_initialization

    //Mapping the PE2 -> EXTI2
    SYSCFG->EXTICR[0] &= ~(0xF << 8);  // Clear EXTI2 bits
    SYSCFG->EXTICR[0] |=  (0x2 << 8);  // Mapping EXTI2

    //Configure external interrupt
    EXTI->IMR  |= (1 << 2);   // Unmask line 2
    EXTI->RTSR |= (1 << 2);   // Rising edge trigger
    EXTI->FTSR &= ~(1 << 2);  // Disable falling edge

    //Enable EXTI2 interrupt in NVIC
    NVIC_EnableIRQ(EXTI2_IRQn);
}


/**
 * @brief     This function is used for initializing GPIO C bus and configure PC0, PC1, PC2 as input pins to raise interrupts whenever pressed.
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, in class lab tutorial and Dean's book
 *
 * Author: Likhita Jonnakuti
 *
 */
void gpio_button_initialization(void)
{
	  //Enable GPIOC clock
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
      //Set PE0, PE1 and PE2 as input (button interrupts)
	  GPIOC->MODER &= ~GPIO_MODER_MODER0;
	  GPIOC->PUPDR |= GPIO_PUPDR_PUPD0_0;
	  GPIOC->MODER &= ~GPIO_MODER_MODER1;
	  GPIOC->PUPDR |= GPIO_PUPDR_PUPD1_0;
	  GPIOC->MODER &= ~GPIO_MODER_MODER2;
	  GPIOC->PUPDR |= GPIO_PUPDR_PUPD2_0;
}

/**
 * @brief     This function is used for initializing External interrupts.
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference :STM32 Reference Manual, in class lab tutorial and Dean's book
 *
 * Author: Likhita Jonnakuti
 *
 */

void button_interrupt_initialization(void)
{
	exti0_initialization();
	exti1_initialization();
	exti2_initialization();
}



