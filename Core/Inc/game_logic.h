/*
 * game_logic.h
 *
 *  Created on: Dec 5, 2025
 *      Author: keal1743
 */

#ifndef INC_GAME_LOGIC_H_
#define INC_GAME_LOGIC_H_

#include "game_definitions.h"
#include "LCD_Game_Display.h"
#include "main.h"
#include "sys_timer_delay.h"
#include <stdio.h>
#include <stdlib.h>

void toggle_solenoid(void);
int set_turn_order(void);
int combat_turn(void);
int ability_check(void);
int roll_anything(void);

#endif /* INC_GAME_LOGIC_H_ */
