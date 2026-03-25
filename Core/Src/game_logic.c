/**
  ******************************************************************************
  * @file           : game_logic.c
  * @brief          : Functions determining the logic within the Combat, Ability Check, and Dice Roll program states
  ******************************************************************************
  * @attention
  *
  * Copyright (c) December 1st, 2025 Kenneth Alcineus.
  * All rights reserved.
  *
  *
  ******************************************************************************
  */
#include "game_logic.h"

#define MAX_CHARACTERS 6
#define MAX_COMBATANTS 16

character_t characters[MAX_CHARACTERS];
character_t turn_order[MAX_COMBATANTS];

int combat_end_flag = 0;
int character_count = 0;
int characters_loaded = 0;

//function to load characters stored in EEPROM
void load_characters(void)
{
	if (characters_loaded == 0)
	{
		for (int i = 0; i < MAX_CHARACTERS; i++)
		{
			characters[i] = get_character(i + 1);
			if (characters[i].name[0] != 0)
			{
				character_count++;
			}
		}
		characters_loaded = 1;
	}
}

//function activate and deactivate the solenoid
void toggle_solenoid(void)
{
      GPIOD->BSRR |= GPIO_BSRR_BS12;

      delay_ms(100);

	  GPIOD->BSRR |= GPIO_BSRR_BR12;

	  delay_ms(1000);
}

//the entry-point function for combat mode - first prompt the players and DM to roll initiative, prompt the DM to enter number of enemies, then prompt for the list of characters and enemies to be sorted
int set_turn_order(void)
{
	load_characters();

	if (character_count == 0)
	{
		return 0;
	}

	int has_advantage[MAX_CHARACTERS];
	int initiative_bonus[MAX_CHARACTERS];
	int reroll_flag = 0;

	for (int i = 0; i < character_count; i++)
	{
		//if character is an assassin rogue of at least level 3
		if (((characters[i].primary_class_subclass == 0x2b) && (characters[i].primary_level >= 3))
			|| ((characters[i].secondary_class_subclass == 0x2b) && (characters[i].secondary_level >= 3))
			|| ((characters[i].tertiary_class_subclass == 0x2b) && (characters[i].tertiary_level >= 3)))
		{
			has_advantage[i] = 1;
			reroll_flag = 1;
		}
		
		//if character is a champion fighter of at least level 3
		if (((characters[i].primary_class_subclass == 0x17) && (characters[i].primary_level >= 3))
			|| ((characters[i].secondary_class_subclass == 0x17) && (characters[i].secondary_level >= 3))
			|| ((characters[i].tertiary_class_subclass == 0x17) && (characters[i].tertiary_level >= 3)))
		{
			has_advantage[i] = 1;
			reroll_flag = 1;
		}
		
		//if character is a barbarian of at least level 7
		if (((characters[i].primary_class_subclass >= 0x01 && characters[i].primary_class_subclass <= 0x05) && (characters[i].primary_level >= 7))
			|| ((characters[i].secondary_class_subclass >= 0x01 && characters[i].secondary_class_subclass <= 0x05) && (characters[i].secondary_level >= 7))
			|| ((characters[i].tertiary_class_subclass >= 0x01 && characters[i].tertiary_class_subclass <= 0x05) && (characters[i].tertiary_level >= 7)))
		{
			has_advantage[i] = 1;
			reroll_flag = 1;
		}
		
		//if character is an gloom stalker ranger of at least level 3
		if (((characters[i].primary_class_subclass == 0x27) && (characters[i].primary_level >= 3))
			|| ((characters[i].secondary_class_subclass == 0x27) && (characters[i].secondary_level >= 3))
			|| ((characters[i].tertiary_class_subclass == 0x27) && (characters[i].tertiary_level >= 3)))
		{
			initiative_bonus[i] = characters[i].wisdom_ability_modifier;
		}
	}

	//prompt players and DM to enter dice in dice roller into LCD, present roll or cancel options - lcd.c
        if (lcd_roll_prompt(1) == 0) //initiative status
		{
        	return 0;
		}
        toggle_solenoid();

	//if not cancelled:
	if (reroll_flag)
	{
	    if (lcd_reroll_prompt() == 0)
		{
        	return 0;
		}
        toggle_solenoid();
		//prompt to reroll for advantage/disadvantage, present roll or cancel options - lcd.c

	}

        //prompt DM to enter enemy count, present count ranging from 1-10 or cancel options - lcd.c
	int enemy_count = lcd_enemies_prompt(10);

	if (enemy_count == 0)
	{
		return 0;
	}

	int temp_enemy_count = enemy_count;

	for (int i = 0; i < MAX_COMBATANTS; i++)
	{
		if (i < MAX_CHARACTERS && characters[i].name[0] >= 32)
		{
			turn_order[i] = characters[i];
		}
		else if (temp_enemy_count > 0)
		{
			character_t c;
			c.name[0] = 1;
			turn_order[i] = c;
			temp_enemy_count--;
		}
		else
		{
			break;
		}
	}

    lcd_turn_order_prompt(turn_order, characters);
	//pass in turn order to prompt players and DM to manually sort player and enemy turn order, present character names with initiative bonus and advantage/disadvantage flag alongside each one, and enemy names, so the user can sort them. present confirm and cancel options as well - lcd.c

	return 1;
}

//after turn order is set, the dice roller will progress through each turn in combat for combat mode
int combat_turn(void)
{
        int turn = 1;
        int order = 0;
        int is_player = 0;
        int extra_action = 0;
        
        int bonus_actions[12];
        
        int weapon_damage[8];
        int weapon_to_hit[8];
        int weapon_damage_bonus[8];
        
        while (!combat_end_flag)
        {
                //setup phase
				if (turn_order[order].name[0] == 0) //if first byte in character is not 0
				{
						continue;
				}
                if (turn_order[order].name[0] >= 32) //if first byte in character is not 0
                {
                        is_player = 1;
                }
                
                for (int i = 0; i < 12; i++)
                {
                        bonus_actions[i] = 0;
                }
                
                for (int i = 0; i < 8; i++)
                {
                        weapon_damage[i] = 0;
                        weapon_to_hit[i] = 0;
                        weapon_damage_bonus[i] = 0;
                }
                
                if (is_player)
                {
                        //if character is a rogue of at least level 2
                    if (((turn_order[order].primary_class_subclass >= 0x29 && turn_order[order].primary_class_subclass <= 0x2D) && (turn_order[order].primary_level >= 2))
			        || ((turn_order[order].secondary_class_subclass >= 0x29 && turn_order[order].secondary_class_subclass <= 0x2D) && (turn_order[order].secondary_level >= 2))
			        || ((turn_order[order].tertiary_class_subclass >= 0x29 && turn_order[order].tertiary_class_subclass <= 0x2D) && (turn_order[order].tertiary_level >= 2)))
		        {
			        bonus_actions[0x02-1] = 1;
			        bonus_actions[0x03-1] = 1;
			        bonus_actions[0x06-1] = 1;
		        }
		        
                        //action phase - the only actions that require dice rolls are attack, hide, influence, magic, search, and study
                        
                        int action = lcd_actions_prompt(turn_order[order], turn);
                        //prompt player to enter action or unlisted bonus action, present all 12 actions, unlisted bonus action, and end combat options - lcd.c
                        
                        if (action == 0)
                        {
                                return 0;
                        }
                        
                        if (action <= 12)
                        {
                                if (bonus_actions[action - 1] == 1 && extra_action != -1)
                                {
                                        extra_action = 1;
                                }
                        }
                        
                        //if attack action is selected
                        if (action == 0x01)
                        {
                                for (int i = 0; i < 8; i++)
                                {
                                        if (turn_order[order].equipped_weapons[i] != 0)
                                        {
                                                //get weapon damage from equipment - item.c
                                                
                                                int prof_bonus = 0;
                                                int ability_bonus = 0;
                                                
                                                if (turn_order[order].weapon_proficiencies[turn_order[order].equipped_weapons[i]] == 1)
                                                {
                                                        prof_bonus = turn_order[order].proficiency_bonus;
                                                }
                                                
                                                if (turn_order[order].strength_ability_modifier >= turn_order[order].dexterity_ability_modifier)
                                                {
                                                        ability_bonus = turn_order[order].strength_ability_modifier;
                                                }
                                                else
                                                {
                                                        ability_bonus = turn_order[order].dexterity_ability_modifier;
                                                }
                                                
                                                weapon_to_hit[i] = prof_bonus + ability_bonus;
                                                weapon_damage_bonus[i] = ability_bonus;
                                        }
                                }
                                int selected_weapon = lcd_weapon_select_prompt(turn_order[order].equipped_weapons);
                                //prompt player to select weapon, present the equipped weapons or cancel option - lcd.c
                                
                                int confirm_hit = 0;

                                //if cancelled, break out of the action if statement
                                if (selected_weapon != 0)
                                {
                                	//prompt player to enter attack die into LCD, present roll or cancel options - lcd.c
                                	if (lcd_roll_prompt(2) == 0)
                                	{
                                		continue;
                                	}
                                	toggle_solenoid();


                                	confirm_hit = lcd_confirm_hit_prompt(weapon_to_hit[selected_weapon], 1); //critical = 2, hit = 1, miss = 0
                                	//while showing to-hit bonus, prompt player to confirm hit, miss, or critical hit - lcd.c


        	                        if (confirm_hit == 2)
        	                        {
        	                                int new_dice = weapon_damage[selected_weapon];
        	                                new_dice /= 8;
        	                                weapon_damage[selected_weapon] %= 16;
        	                                weapon_damage[selected_weapon] += (new_dice * 16);
        	                        }

        	                        if (confirm_hit != 0)
        	                        {
        	                                lcd_roll_damage_prompt(weapon_damage[selected_weapon], weapon_damage_bonus[selected_weapon]);
        	                                //while showing weapon damage and damage bonus, prompt player to roll for damage, present roll option only - lcd.c
        									toggle_solenoid();
        	                        }

        	                        if (lcd_down_check(1) > 0)
        	                        {
        	                          int downed_enemy_num = lcd_remove_from_combat(turn_order, 1);
                    			 turn_order[downed_enemy_num - 1].name[0] = 0;
                    			 for (int i = 0; i < MAX_COMBATANTS; i++)
                    			 {
                    			    combat_end_flag = 1;
                    			    if (turn_order[i].name[0] == 1)
                    			    {
                    			      combat_end_flag = 0;
                    			    }
                    			 }
    	                                lcd_remove_from_combat(turn_order, 1); // 1 represents removing of enemy
    	                                //prompt DM to navigate to which enemy to remove, present enemy options - lcd.c
        	                        }
        	                        else
        	                        {
    	                                //prompt DM to confirm whether attack action triggered an enemy reaction, present yes or no options - lcd.c
        	                        	//lcd_attack_reaction_prompt();
    	                                //ideally we should check whether the attack triggers a reaction from the enemy, but wont bother due to time constraints, so continue
        	                        }
                                }

	                        order++;
	                        continue;
                        }
                        
                        else if (action == 0x06) //hide action
                        {
                                if (lcd_roll_ability_prompt(0x11, turn_order[order].stealth_skill_modifier) == 0)
                                {
									continue;
								}
								toggle_solenoid();
	                        //prompt player to enter stealth check die into LCD while displaying stealth skill modifier, present roll or cancel options - lcd.c
                        }
                        
                        else if (action == 0x07) //influence action
                        {
                                if (lcd_roll_ability_prompt(0x0E, turn_order[order].persuasion_skill_modifier) == 0)
                                {
									continue;
								}
								toggle_solenoid();
	                        //prompt player to enter persuasion check die into LCD while displaying persuasion skill modifier, present roll or cancel options - lcd.c
                        }
                        
                        else if (action == 0x08) //magic action
                        {
                                if (lcd_roll_prompt(3) == 0) //magic status
                                {
                                	continue;
                                }
                                toggle_solenoid();
	                        //prompt player and DM to enter magic dice and/or enemy saving throw die into LCD, present roll or cancel options - lcd.c
                        }
                        
                        else if (action == 0x0A) //search action
                        {
                                if (lcd_roll_ability_prompt(0x0C, turn_order[order].perception_skill_modifier) == 0) //perception parameter
                                {
									continue;
								}
								toggle_solenoid();
	                        //prompt player to enter persuasion check die into LCD while displaying perception skill modifier, present roll or cancel options - lcd.c
                        }
                        
                        else if (action == 0x0B) //study action
                        {
                                if (lcd_roll_ability_prompt(0x0C, turn_order[order].insight_skill_modifier) == 0) //insight parameter
                                {
                                	continue;
                                }
                                toggle_solenoid();
	                        //prompt player to enter persuasion check die into LCD while displaying insight skill modifier, present roll or cancel options - lcd.c
                        }
                        
                        else
                        {
                                lcd_no_roll_prompt();
                                //acknowledge to player that the selected action does not require a dice roll, present continue or cancel options - lcd.c
                        }
                        
                        if (extra_action == 1)
                        {
                                extra_action = -1;
                                continue;
                        }
                        
                        order++;
	                continue;
                }
                
                else
                {
                     if (lcd_roll_prompt(2) == 1) //attack status
                     {
                    	 //with 0 parameter, prompt DM to enter confirm hit, miss, or critical hit
                    	 if (lcd_confirm_hit_prompt(0, 0) != 0)
                    	 {
                    		 if (lcd_down_check(0))
                    		 {
                    		        int downed_character_num = lcd_remove_from_combat(turn_order, 0);
                    			 turn_order[downed_character_num - 1].name[0] = 0;
                    			 for (int i = 0; i < MAX_COMBATANTS; i++)
                    			 {
                    			    combat_end_flag = 1;
                    			    if (turn_order[i].name[0] >= 32)
                    			    {
                    			      combat_end_flag = 0;
                    			    }
                    			 }
                    		 }
                    	 }
                     }
	                order++;
	                continue;
                }
        }
        lcd_combat_end();
        //acknowledge to the user that combat has ended, upon button press return to program start state - lcd.c
        return 1;
}

//function for the ability check state - select an ability, then select a character, then roll the die
int ability_check(void)
{
	load_characters();

	if (character_count == 0)
	{
		return 0;
	}

        int ability = lcd_list_abilities_prompt();
        //prompt player to select an ability from a list, present abilities or cancel options - lcd.c
        
        if (ability == 0)
        {
        	return 0;
        }
        
        int character_num = lcd_list_characters_prompt(characters, ability);
        //prompt player to select a character from the list to perform the ability check, present character names alongside ability modifier or cancel options - lcd.c
                
                //if canceled, return 0
        if (character_num == 0)
        {
        	return 0;
        }

        uint8_t ability_modifier = 0;

        switch (ability)
        {
        case 1:
        	ability_modifier = characters[character_num - 1].acrobatics_skill_modifier;
        	break;
        case 2:
        	ability_modifier = characters[character_num - 1].animal_handling_skill_modifier;
        	break;
        case 3:
        	ability_modifier = characters[character_num - 1].arcana_skill_modifier;
        	break;
        case 4:
        	ability_modifier = characters[character_num - 1].athletics_skill_modifier;
        	break;
        case 5:
        	ability_modifier = characters[character_num - 1].deception_skill_modifier;
			break;
        case 6:
        	ability_modifier = characters[character_num - 1].history_skill_modifier;
        	break;
        case 7:
        	ability_modifier = characters[character_num - 1].insight_skill_modifier;
        	break;
        case 8:
        	ability_modifier = characters[character_num - 1].intimidation_skill_modifier;
        	break;
        case 9:
        	ability_modifier = characters[character_num - 1].investigation_skill_modifier;
        	break;
        case 10:
        	ability_modifier = characters[character_num - 1].medicine_skill_modifier;
        	break;
        case 11:
        	ability_modifier = characters[character_num - 1].nature_skill_modifier;
        	break;
        case 12:
        	ability_modifier = characters[character_num - 1].perception_skill_modifier;
        	break;
        case 13:
        	ability_modifier = characters[character_num - 1].performance_skill_modifier;
        	break;
        case 14:
        	ability_modifier = characters[character_num - 1].persuasion_skill_modifier;
        	break;
        case 15:
        	ability_modifier = characters[character_num - 1].religion_skill_modifier;
        	break;
        case 16:
        	ability_modifier = characters[character_num - 1].sleight_of_hand_skill_modifier;
        	break;
        case 17:
        	ability_modifier = characters[character_num - 1].stealth_skill_modifier;
        	break;
        case 18:
        	ability_modifier = characters[character_num - 1].survival_skill_modifier;
        	break;
        default:
        	break;
        }
        
        if (lcd_roll_ability_prompt(ability, (int)ability_modifier) == 0)
		{
        	return 0;
		}
        toggle_solenoid();

        //prompt player to enter ability check die into LCD while displaying ability skill modifier, present roll or cancel options - lcd.c

        lcd_ability_check_end();
        //acknowledge to the user that ability check has ended, upon button press return to program start state - lcd.c
        return 1;
}

//function for the dice roll state - prompt user to roll unconditionally
int roll_anything(void)
{
        while (1)
        {
        	//prompt user to roll whatever dice, present roll or cancel options - lcd.c
                if (lcd_roll_prompt(0) == 0)
                {
                	return 1;
                }
                toggle_solenoid();
        }

        return 0;
}
