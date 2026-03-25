/*
 * LCD_Game_Display.h
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */

#ifndef INC_LCD_GAME_DISPLAY_H_
#define INC_LCD_GAME_DISPLAY_H_

#include <stm32f411xe.h>
#include "LCD_Fill_shapes.h"
#include "LCD_Text_Display.h"
#include "sys_timer_delay.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "game_definitions.h"
void combat_mode_lcd(void);
void dice_roller_mode_lcd(void);
void serial_mode_lcd(void);
void ability_check_mode_lcd(void);
void character_menu(character_t *characters, int size);
void ability_check_menu(void);
void program_start_mode_lcd(void);
void scroll_character_info(int direction, character_t *characters);
int lcd_roll_ability_prompt(int ability, int modifier);
int lcd_list_characters_prompt(character_t *characters, int ability);
void character_check_menu(character_t *characters);
int lcd_list_abilities_prompt();
int lcd_remove_from_combat(character_t *turn_order, int is_enemy);
int lcd_weapon_select_prompt(uint8_t *weapons);
int lcd_attack_reaction_prompt(void);
int lcd_confirm_hit_prompt(int weapon, int isplayer);
int lcd_reroll_prompt(void);
int lcd_no_roll_prompt(void);
int lcd_down_check(int x);
int lcd_combat_end(void);
int lcd_ability_check_end(void);
int lcd_roll_prompt(int x);
void lcd_confirm_hit_prompt_draw(int selection);
void lcd_down_check_draw(int selection);
void lcd_no_roll_prompt_draw(int selection);
void lcd_roll_prompt_draw(int selection);
int lcd_roll_damage_prompt(int damage, int bonus);
int lcd_enemies_prompt(int current_num_enemies);
int lcd_actions_prompt(character_t current, int turn);
int lcd_list_abilities_prompt(void);
void scroll_ability_info(int direction);
void ability_check_menu(void);
int lcd_turn_order_prompt(character_t *turn_order, character_t *characters);
void lcd_weapon_select_prompt_draw(uint8_t *weapons);



#endif /* INC_LCD_GAME_DISPLAY_H_ */
