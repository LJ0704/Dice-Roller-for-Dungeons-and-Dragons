/*
 * i2c.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */



#ifndef __i2c_H
#define __i2c_H

#include "stm32f4xx.h"
#include <stdint.h>
#include "stdio.h"

/**
 * @brief     This function is used for initializing I2C
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference : 1) STM32 Reference Manual
 *             2) https://github.com/hussamaldean
 *             3) ST7789 Datasheet
 *
 *
 * Original Author: original author: Husamuldeen <https://github.com/hussamaldean>
 *
 */
void i2c_init(void);

#endif

