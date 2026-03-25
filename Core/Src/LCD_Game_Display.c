/*
 * LCD_Game_Display.c
 *
 *  Created on: Dec 2, 2025
 *      Author: Likhita
 */
#include "LCD_Game_Display.h"

extern volatile int lcd_down_selection;
extern volatile int return_to_program_start_flag;
extern volatile int lcd_roll_prompt_selection_ok_pressed_flag;
extern volatile int lcd_down_selection;
extern volatile int lcd_down_selection_changed;
extern volatile int flag_ok;
extern volatile int flag_left;
extern volatile int flag_right;
extern volatile int lcd_roll_prompt_selection_changed;
extern char names_character[10][20];
extern int visible_items;
extern volatile int lcd_combat_end_flag;
extern volatile int lcd_roll_prompt_selection;
#define CANCEL_Y 200

#define ROLL_X   10
#define CANCEL_X 115
#define MENU_Y   200
#define HIT_X       30
#define CRITICAL_X 100
#define MISS_X     236
#define LCD_WIDTH  320
#define LCD_HEIGHT 240

#define FONT_BASE_WIDTH 5   //WIDTH
#define FONT_BASE_HEIGHT 8  // height


volatile int cursor_index = 0;
volatile int selected_index = -1;     // -1 = no selection

int top_index = 0;

char actions_character[13][20] = {
		"CANCEL", "ATTACK", "DASH", "DISENGAGE", "DODGE", "HELP",
    "HIDE", "INFLUENCE", "MAGIC", "READY", "SEARCH",
	"STUDY","UTILIZE"};

char ability_check_lcd[19][20] = {"CANCEL","Acrobatics", "Animal Handling", "Arcana", "Athletics",
		"Deception", "History", "Insight", "Intimidation", "Investigation", "Medicine", "Nature",
		"Perception", "Performance", "Persuasion", "Religion", "Sleight of Hand", "Stealth", " Survival"
};

char weapon_lcd[38][20] = {"Club", "Dagger", "Greatclub", "Handaxe",
		"Javelin", "Light Hammer", "Mace", "Quarterstaff", "Sickle", "Spear", "Dart",
		"Light Crossbow", "Shortbow", "Sling", "Battleaxe", "Flail", "Glaive", " Greataxe",
		"Greatsword", "Halberd", "Lance", "Longsword", "Maul", "Morningstar", " Pike",
		"Rapier", "Scimitar", "Shortsword", "Trident", "Warhammer", "War Pick", " Whip",
		"Blowgun", "Hand Crossbow", "Heavy Crossbow", "Longbow", "Musket", "Pistol"
};


/**
 * @brief     This function is used to center align a string on the LCD display
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void draw_centered_string(int y, char *str, uint16_t color, uint8_t size)
{
    int char_width = FONT_BASE_WIDTH *size;
    int str_px_width = strlen(str)*char_width;
    int x = (LCD_WIDTH-str_px_width) /2;
    if (x < 0) x = 0;
    ST7789_DrawString(x, y, str, color, size);
}


/**
 * @brief     This function is used to draw pixel at (x,y) with a certain color.
 *
 * @param[in]  NA
 *
 * @return     NA
 *
 * Reference : 1) Working with STM32F4 and SPI-TFT: ST7789 IPS 240×240 LCD by Husamuldeen
 *             https://blog.embeddedexpert.io/?p=1215
 *             2) Github Repository : Majid Derhambakhsh
 *             https://github.com/Majid-Derhambakhsh/ST7789/blob/main/Src/st7789.c
 *
 * Original Author: Majid Derhambakhsh (Github Repository) and Husamuldeen (Embedded Expert IO blog)
 *
 * Modified by: Likhita Jonnakuti
 *
 */
/**
 * @brief     This function is used to display combat mode on LCD.
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void combat_mode_lcd(void)
{
		ST7789_Fill(0xFFE0);
		ST7789_DrawString(88,88 ,"COMBAT", RED, 4);
	    ST7789_DrawString(112,120 ,"MODE", RED, 4);
		delay_ms(8);
}
/**
 * @brief     This function is used to display DICE ROLLER mode on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void dice_roller_mode_lcd(void)
{
		ST7789_Fill(0xFFE0);
		ST7789_DrawString(18, 88, "DICE ROLLER", RED, 4);
		ST7789_DrawString(112,120 ,"MODE", RED, 4);
		delay_ms(8);
}
/**
 * @brief     This function is used to cdisplay SERIAL MODE on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void serial_mode_lcd(void)
{
		ST7789_Fill(0xFFE0);
		ST7789_DrawString(88, 88, "SERIAL", RED, 4);
		ST7789_DrawString(112,120 ,"MODE", RED, 4);
		delay_ms(8);
}
/**
 * @brief     This function is used to display ability check mode on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void ability_check_mode_lcd(void)
{
		ST7789_Fill(0xFFE0);
		ST7789_DrawString(18, 88, "ABILITY CHECK", RED, 4);
		ST7789_DrawString(112,120 ,"MODE", RED, 4);
		delay_ms(8);
}
/**
 * @brief     This function is used to siaply program start mode  on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void program_start_mode_lcd(void)
{
		ST7789_Fill(0xFFE0);
		ST7789_DrawString(104, 100, "START", RED, 4);

		delay_ms(8);

}
/**
 * @brief     This function is used to display START on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void character_name_lcd()
{
		ST7789_Fill(0xFFE0);
		ST7789_DrawString(104, 100, "START", RED, 4);
        delay_ms(8);
}

/**
/**
 * @brief     This function is used to display the character menu on LCD, only 6 characters are visible on the screen 
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void character_menu(character_t *characters, int size)
{
    ST7789_FillRect(0, 60, 240, 200, YELLOW);

    ST7789_DrawString(10, 20, "TURN ORDER", RED, 3);

    int y = 60;

    int enemyctr = 1;

    for (int i = top_index; i < top_index + visible_items; i++)
    {
        uint16_t color;


        if (i == selected_index)
            color = AQUA;
        else if (selected_index == -1 && i == cursor_index)
            color = RED;
        else
            color = 0xBC40;

        if (characters[i].name[0] == 1) //if enemy
		{
			char tempstr[8];
			tempstr[0] = 'E';
			tempstr[1] = 'N';
			tempstr[2] = 'E';
			tempstr[3] = 'M';
			tempstr[4] = 'Y';
			tempstr[5] = ('0' + (enemyctr / 10));
			tempstr[6] = ('0' + (enemyctr % 10));
			tempstr[7] = '\0';
			ST7789_DrawString(10, y, tempstr, color, 2);
			y += 20;
			enemyctr++;
		}
		if (characters[i].name[0] >= 32)
		{
			ST7789_DrawString(10, y, (char*)characters[i].name, color, 2);
			y += 20;
		}
    }

    // Draw confirm/cancel to select option from 
    ST7789_DrawString(10, 200, "Confirm", cursor_index == size ? WHITE : 0xBC40, 2);
    ST7789_DrawString(10, 220, "Cancel",  cursor_index == size + 1 ? WHITE : 0xBC40, 2);
}

/**
 * @brief     This function is used to display the actions menu on LCD, only 6 characters are visible on the screen 
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void action_menu(void)
{
    ST7789_FillRect(0, 60, 240, 200, YELLOW);

    int y = 60;

    for (int i = top_index; i < top_index + visible_items; i++)
    {
        uint16_t color;
       if ( i == cursor_index)
            color = RED;
        else
            color = 0xBC40;

        ST7789_DrawString(10, y, actions_character[i], color, 2);
        y += 20;
    }

}
/**
 * @brief     This function is used to display the weapons menu on LCD, only 6 characters are visible on the screen 
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void weapon_menu(uint8_t *weapons)
{
    ST7789_FillRect(0, 60, 240, 200, YELLOW);

	char dice_damage_str[5];
	int temp_damage;
    int y = 20;

    for (int i = top_index; i < top_index + visible_items; i++)
    {
        uint16_t color;
       if ( i == cursor_index)
            color = RED;
        else
            color = 0xBC40;

       ST7789_DrawString(10, y, weapon_lcd[weapons[i]], color, 2);

      temp_damage = weapon_damage_lookup[weapons[i]] / 16;
      dice_damage_str[0] = '0' + (temp_damage / 10);
      dice_damage_str[1] = '0' + (temp_damage % 10);
      dice_damage_str[2] = 'd';
      temp_damage = weapon_damage_lookup[weapons[i]] % 16;
      dice_damage_str[3] = '0' + (temp_damage / 10);
      dice_damage_str[4] = '0' + (temp_damage % 10);

      ST7789_DrawString(30, y, dice_damage_str, color, 2);
        y += 40;
    }
}
/**
 * @brief     This function is used to highlight the selected weapon on LCD 
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void lcd_weapon_select_prompt_draw(uint8_t *weapons){
	int temp_damage;
	char dice_damage_str[5];
	int y = 20;
	uint16_t color;

	for (int i = 0; i < 8; i++) //8 represents max number of equipped weapons
		{
			if (weapons[i] != 0)
			{
				y += 40;
				if(lcd_roll_prompt_selection == i)
				{
					color = RED;
				}
				else
				{
					color = PURPLE;
				}
				ST7789_DrawString(10, y, weapon_lcd[weapons[i]], color, 2);

				temp_damage = weapon_damage_lookup[weapons[i]] / 16;
				dice_damage_str[0] = '0' + (temp_damage / 10);
				dice_damage_str[1] = '0' + (temp_damage % 10);
				dice_damage_str[2] = 'd';
				temp_damage = weapon_damage_lookup[weapons[i]] % 16;
				dice_damage_str[3] = '0' + (temp_damage / 10);
				dice_damage_str[4] = '0' + (temp_damage % 10);

				ST7789_DrawString(180, y, dice_damage_str, color, 2);
			}
		}
}

/**
 * @brief     This function is used to display ability check menu on LCD 
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void ability_check_menu(void)
{
    ST7789_FillRect(0, 60, 240, 200, YELLOW);

    int y = 60;
    for (int i = top_index; i < top_index + visible_items; i++)
       {
           uint16_t color;
          if ( i == cursor_index)
               color = RED;
           else
               color = 0xBC40;

           ST7789_DrawString(10, y, ability_check_lcd[i], color, 2);
           y += 20;
       }

}

/**
 * @brief     This function is used to highlight the character from the character menu list on LCD 
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */

void character_check_menu(character_t *characters)
{
    ST7789_FillRect(0, 60, 240, 200, YELLOW);
//
//    ST7789_DrawString(10, 20, "ABILITY CHECK MODE", RED, 3);

	int y = 60;
	uint16_t color;

	for (int i = 0; i < 6; i++)// Maximum Total number of character in this game is 6
		{
			if (characters[i].name[0] != 0)
			{
				y += 20;
				if(lcd_roll_prompt_selection == i)
				{
					color = RED;
				}
				else
				{
					color = PURPLE;
				}
				ST7789_DrawString(10, y, (char*)characters[i].name, color, 2);

			}
		}

}

/**
 * @brief     This function is used to scroll the character information when the charcter selecter index is less or geater than the visible items
 * on the LCD then the top index is incrementd or decremented as per the logic.
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void scroll_character_info(int direction,character_t *characters)
{
    const int item_height = 20;
    const int total_shift = item_height;
    const int step = 28;

    for (int shift = 0; shift <= total_shift; shift += step)
    {
        int draw_shift = shift * direction;

        // Clear only the scroll area (safe)
        ST7789_FillRect(0, 60, 240, 200, YELLOW);

        int y = 60 - draw_shift;

        for (int row = 0; row < visible_items + 1; row++)
        {
            int i = top_index + row;

            if (i < 0 || i > 9) continue;

            uint16_t color;
            if (i == selected_index) color = AQUA;
            else if (selected_index == -1 && i == cursor_index) color = RED;
            else color = 0xBC40;

        	char shortened_name[20];
        	//do the thing where as many characters are displayed on the screen as possible and implement scrolling if needed
        	for (int i = 0; i < 6; i++)
        	{
        		for (int j = 0; j < 20; j++)
        		{
        			shortened_name[j] = characters[i].name[j];
        		}
        	}

            if (y >= 60 && y <= 260)
                ST7789_DrawString(10, y, shortened_name, color, 2);

            y += item_height;
        }

      //  delay_ms(8);
    }
}

/**
 * @brief     This function is used to scroll the actions information when the action selecter index is less or geater than the visible items
 * on the LCD then the top index is incrementd or decremented as per the logic.
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void scroll_actions_info(int direction)
{
    const int item_height = 20;
    const int total_shift = item_height;
    const int step = 28;

    for (int shift = 0; shift <= total_shift; shift += step)
    {
        int draw_shift = shift * direction;

        // Clear only the scroll area
        ST7789_FillRect(0, 60, 240, 200, YELLOW);

        int y = 60 - draw_shift;

        for (int row = 0; row < visible_items + 1; row++)
        {
            int i = top_index + row;

            if (i < 0 || i > 9) continue;

            uint16_t color;
            if (i == selected_index) color = AQUA;
            else if (selected_index == -1 && i == cursor_index) color = RED;
            else color = 0xBC40;

            if (y >= 60 && y <= 260)
                ST7789_DrawString(10, y, actions_character[i], color, 2);

            y += item_height;
        }

      //  delay_ms(8);
    }
}

/**
 * @brief     This function is used to scroll the ability information when the abilit selecter index is less or geater than the visible items
 * on the LCD then the top index is incrementd or decremented as per the logic.
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void scroll_ability_info(int direction)
{
    const int item_height = 20;
    const int total_shift = item_height;
    const int step = 28;

    for (int shift = 0; shift <= total_shift; shift += step)
    {
        int draw_shift = shift * direction;

        // Clear only the scroll area (safe)
        ST7789_FillRect(0, 60, 240, 200, YELLOW);

        int y = 60 - draw_shift;

        for (int row = 0; row < visible_items + 1; row++)
        {
            int i = top_index + row;

            if (i < 0 || i > 9) continue;

            uint16_t color;
            if (selected_index == -1 && i == cursor_index) color = RED;
            else color = 0xBC40;

            if (y >= 60 && y <= 260)
                ST7789_DrawString(10, y, ability_check_lcd[i], color, 2);

            y += item_height;
        }

    }
}

/**
 * @brief     //prompt player to enter D20 into dice roller LCD while displaying relevant skill modifier, present roll or cancel options.
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Kenneth Alcineus
 *
 */

int lcd_roll_ability_prompt(int ability, int modifier)
{
    lcd_roll_prompt_selection = 0;

	// Clear flags before starting
	flag_left = 0;
	flag_right = 0;
	flag_ok = 0;
	char modifier_str[2];
	ST7789_Fill(0xFFE0);
	ST7789_DrawString(10, 10, "ROLL CHECK FOR", RED, 3);
	ST7789_DrawString(10, 40, ability_check_lcd[ability], RED, 3);
	ST7789_DrawString(10, 80, "AND ADD ", RED, 3);
	sprintf(modifier_str, "%d", modifier);
	ST7789_DrawString(10, 120, modifier_str, RED, 3);

	while(1)
	{
		if (lcd_roll_prompt_selection < 0)
		{
			lcd_roll_prompt_selection = 1;
		}
		if (lcd_roll_prompt_selection > 1)
		{
			lcd_roll_prompt_selection = 0;
		}
		if (flag_left || flag_right)
		{
			flag_right = 0;
			flag_left = 0;
			lcd_roll_prompt_draw(lcd_roll_prompt_selection);
		}
		if (flag_ok)
		{
			flag_ok = 0;
			if(lcd_roll_prompt_selection == 0)
				return 1;//ROLL DICE
			else
				return 0;//CANCEL
		}
	}
}

/**
 * @brief     This function is used to prompt player to remove character or enemy from combat
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_remove_from_combat(character_t *turn_order, int is_enemy)
{
	lcd_roll_prompt_selection = 0;

	// Clear flags before starting
	flag_left = 0;
	flag_right = 0;
	flag_ok = 0;
	int y = 10;
	int enemyctr = 1;


	if (is_enemy)
	{
		ST7789_DrawString(10, y, "Select Enemy:", RED, 4);
		for (int i = 0; i < 16; i++) //16 represents max number of characters and enemies (6 + 10 respectively)
		{
			if (turn_order[i].name[0] == 1) //if enemy
			{
				char tempstr[8];
				y += 10;
				tempstr[0] = 'E';
				tempstr[1] = 'N';
				tempstr[2] = 'E';
				tempstr[3] = 'M';
				tempstr[4] = 'Y';
				tempstr[5] = ('0' + (enemyctr / 10));
				tempstr[6] = ('0' + (enemyctr % 10));
				tempstr[7] = '\0';
				ST7789_DrawString(10, y, tempstr, PURPLE, 2);
				enemyctr++;
			}
		}
	}
	else
	{
		ST7789_DrawString(10, y, "Select Character:", RED, 3);
		for (int i = 0; i < 16; i++) //16 represents max number of characters and enemies (6 + 10 respectively)
		{
			if (turn_order[i].name[0] >= 32) //if character
			{
				y += 10;
				ST7789_DrawString(10, y, (char*)turn_order[i].name, PURPLE, 2);
			}
		}
	}
	while(1)
	{
		if (lcd_roll_prompt_selection < 16)
		{
			if (is_enemy)
			{
				while (turn_order[lcd_roll_prompt_selection].name[0] != 1)
				{
					if (flag_left)
					{
						flag_right = 0;
						lcd_roll_prompt_selection++;
					}
					if (flag_right)
					{
						flag_left = 0;
						lcd_roll_prompt_selection--;
					}
					if (lcd_roll_prompt_selection > 15)
					{
						lcd_roll_prompt_selection = 0;
					}
					if (lcd_roll_prompt_selection < 0)
					{
						lcd_roll_prompt_selection = 15;
					}
				}
			}
			else
			{
				while (turn_order[lcd_roll_prompt_selection].name[0] < 32)
				{
					if (flag_left)
					{
						flag_right = 0;
						lcd_roll_prompt_selection++;
					}
					if (flag_right)
					{
						flag_left = 0;
						lcd_roll_prompt_selection--;
					}
					if (lcd_roll_prompt_selection > 15)
					{
						lcd_roll_prompt_selection = 0;
					}
					if (lcd_roll_prompt_selection < 0)
					{
						lcd_roll_prompt_selection = 15;
					}
				}
			}

		}
		if (lcd_roll_prompt_selection < 0)
		{
			lcd_roll_prompt_selection = 16;
		}
		if (lcd_roll_prompt_selection > 16) //16 is the maximum number of characters + enemies and prompt selection is 0-indexed, so 16 would represent cancel
		{
			lcd_roll_prompt_selection = 0;
		}
		if (flag_left || flag_right)
		{
			flag_right = 0;
			flag_left = 0;
			//call the draw function that will display each character shortened_name
			//lcd_remove_from_combat_draw(lcd_roll_prompt_selection);
		}
		if (flag_ok)
		{
			flag_ok = 0;
			if (lcd_roll_prompt_selection == 16)
			{
				return 0; //CANCEL
			}
			else
			{
				turn_order[lcd_roll_prompt_selection].name[0] = 0;
				return (lcd_roll_prompt_selection + 1); //return character number
			}
		}
	}
}

////prompt player to select weapon, present the equipped weapons or cancel option - lcd.c
int lcd_weapon_select_prompt(uint8_t *weapons)
{
	lcd_roll_prompt_selection = 0;

	// Clear flags before starting
	flag_left = 0;
	flag_right = 0;
	flag_ok = 0;
	int y = 10;
	ST7789_Fill(0xFFE0);
	ST7789_DrawString(10, y, "Select Weapon:", RED, 2);
	lcd_weapon_select_prompt_draw(weapons);
	while(1)
	{
		if (lcd_roll_prompt_selection < 8)
		{
			while (weapons[lcd_roll_prompt_selection] == 0)
			{
				if (flag_left)
				{
					flag_right = 0;
					lcd_roll_prompt_selection++;
				}
				if (flag_right)
				{
					flag_left = 0;
					lcd_roll_prompt_selection--;
				}
				if (lcd_roll_prompt_selection > 7)
				{
					lcd_roll_prompt_selection = 0;
				}
				if (lcd_roll_prompt_selection < 0)
				{
					lcd_roll_prompt_selection = 7;
				}
			}
		}
		if (lcd_roll_prompt_selection < 0)
		{
			lcd_roll_prompt_selection = 8;
		}
		if (lcd_roll_prompt_selection > 8) //8 is the maximum number of weapons and prompt selection is 0-indexed, so 8 would represent cancel
		{
			lcd_roll_prompt_selection = 0;
		}
		if (flag_left || flag_right)
		{
			flag_right = 0;
			flag_left = 0;
			//call the draw function that will display each character shortened_name
			//lcd_weapon_select_prompt_draw(lcd_roll_prompt_selection);
			lcd_weapon_select_prompt_draw(weapons);
		}
		if (flag_ok)
		{
			flag_ok = 0;
			if (lcd_roll_prompt_selection == 8)
			{
				return 0; //CANCEL
			}
			else
			{
				return (lcd_roll_prompt_selection + 1); //return weapon number
			}
		}
	}
}


/**
 * @brief     This function is used to draw roll-dice  prompt on LCD
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void lcd_roll_prompt_draw(int selection)
{
	// erase menu area only
		    ST7789_FillRect(0, MENU_Y, 320, 40, 0xFFE0);
			if (selection == 0)
			{
				ST7789_DrawString(ROLL_X,MENU_Y,"ROLL-DICE", PURPLE, 3);
				ST7789_DrawString(CANCEL_Y,MENU_Y,"CANCEL", RED, 3);
			}
			if(selection == 1)
			{
				ST7789_DrawString(CANCEL_Y,MENU_Y,"CANCEL", PURPLE, 3);
				ST7789_DrawString(ROLL_X,MENU_Y ,"ROLL-DICE", RED, 3);
			}

			delay_ms(100);
}
/**
 * @brief     This function is used to draw no roll prompt
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void lcd_no_roll_prompt_draw(int selection)
{
	ST7789_FillRect(0, MENU_Y, 320, 40, 0xFFE0);
			        if(selection == 0)
					{
				    ST7789_DrawString(ROLL_X,MENU_Y,"CONTINUE", PURPLE, 2);
					}
					else
					{
					 ST7789_DrawString(ROLL_X,MENU_Y ,"CONTINUE", RED, 2);
					}
					if(selection == 1)
					{
					ST7789_DrawString(CANCEL_Y,MENU_Y,"CANCEL", PURPLE, 2);
					}
					else
					{
					ST7789_DrawString(CANCEL_Y,MENU_Y,"CANCEL", RED, 2);
					}
					delay_ms(8);
}
/**
 * @brief     This function is used to draw down check prompt on LCD
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void lcd_down_check_draw(int selection)
{
	  ST7789_FillRect(0, MENU_Y, 320, 40, 0xFFE0);
		        if(selection == 0)
				{
			    ST7789_DrawString(ROLL_X,MENU_Y,"YES", PURPLE, 4);
				}
				else
				{
				 ST7789_DrawString(ROLL_X,MENU_Y ,"YES", RED, 4);
				}
				if(selection == 1)
				{
				ST7789_DrawString(CANCEL_X,MENU_Y,"NO", PURPLE, 4);
				}
				else
				{
				ST7789_DrawString(CANCEL_X,MENU_Y,"NO", RED, 4);
				}
				delay_ms(8);
}
/**
 * @brief     This function is used to draw confirm hit prompt on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
void lcd_confirm_hit_prompt_draw(int selection)
{
	  ST7789_FillRect(0, MENU_Y, 320, 60, 0xFFE0);
		        if(selection == 0)
				{
			    ST7789_DrawString(ROLL_X,MENU_Y,"CRITICAL", PURPLE, 2);
				}
				else
				{
				 ST7789_DrawString(ROLL_X,MENU_Y ,"CRITICAL", RED, 2);
				}
				if(selection == 1)
				{
				ST7789_DrawString(CANCEL_X,MENU_Y,"HIT", PURPLE, 2);
				}
				else
				{
				ST7789_DrawString(CANCEL_X,MENU_Y,"HIT", RED, 2);
				}
				if(selection == 2)
				{
				ST7789_DrawString(CANCEL_Y,MENU_Y,"MISS", PURPLE, 2);
				}
				else
				{
				ST7789_DrawString(CANCEL_Y,MENU_Y,"MISS", RED, 2);
				}
				delay_ms(8);
}

/**
 * @brief     This function is used to prompt player to enter dice roll ,or cancel the action
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_roll_prompt(int x)
{
	lcd_roll_prompt_selection = 0;
	flag_left = 0;
	flag_right = 0;
	flag_ok = 0;
	if(x == 0)
	{
		ST7789_Fill(0xFFE0);
		draw_centered_string(40, "ENTER DICE", RED, 4);
		draw_centered_string(80 ,"TO ROLL", RED, 4);
		delay_ms(8);
	}
	else if(x == 1)
	{
		ST7789_Fill(0xFFE0);
		draw_centered_string( 40, "ENTER DICE ", RED, 4);
		draw_centered_string(80 ,"FOR INITIATIVE", RED, 4);
		delay_ms(8);
	}
	else if(x == 2)
	{
		ST7789_Fill(0xFFE0);
		draw_centered_string( 40, "ENTER DICE ", RED, 4);
		draw_centered_string(80 ,"TO ATTACK", RED, 4);
		delay_ms(8);
	}
	else if(x == 3)
	{
		ST7789_Fill(0xFFE0);
		draw_centered_string( 40, "ENTER DICE ", RED, 4);
		draw_centered_string(80 ,"FOR MAGIC SPELL", RED, 4);
		delay_ms(8);
	}
	//Display initial prompt
	lcd_roll_prompt_draw(lcd_roll_prompt_selection);
	//selection
	while(1)
				{
	    if (lcd_roll_prompt_selection < 0)
	    {
	                lcd_roll_prompt_selection = 0;   // 0 = ROLL, 1 = CANCEL
	    }
	    if (lcd_roll_prompt_selection > 1)
	    {
	                lcd_roll_prompt_selection = 1;
	    }

				if(flag_left || flag_right)
				{
					flag_left = 0;
					flag_right = 0;
				lcd_roll_prompt_draw(lcd_roll_prompt_selection);


				}
				if(flag_ok)
				{
					flag_ok = 0;
					if(lcd_roll_prompt_selection == 0)
						return 1;//ROLL DICE
					else
						return 0;//CANCEL
				}
				}
}


/**
 * @brief     This function is used to check if the ability check moe is exited on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_ability_check_end(void)
{
	flag_ok = 0;
	ST7789_Fill(0xFFE0);
	draw_centered_string(80, "EXITING ", RED, 4);
	draw_centered_string(120 ,"ABILITY CHECK MODE", RED, 4);
	draw_centered_string(170 ,"RETURN", RED, 4);
	delay_ms(8);

	while(1)
	{
		if(flag_ok == 1)
		{
			draw_centered_string(170 ,"RETURN", AQUA, 4);
			delay_ms(100);
			ability_check_mode_lcd(); // RETURN?
			flag_ok = 0;
			return 1;
		}
	}
}
/**
 * @brief     This function is used exit combat mode
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_combat_end(void)
{
	flag_ok = 0;
	ST7789_Fill(0xFFE0);
	ST7789_DrawString(88, 88, "EXITING...", RED, 4);
	ST7789_DrawString(112,120 ,"COMBAT MODE", RED, 4);
	ST7789_DrawString(112,170 ,"RETURN", RED, 4);
	delay_ms(8);

	while(1)
	{
		if(flag_ok == 1)
		{
			ST7789_DrawString(112,170 ,"RETURN", AQUA, 4);
			delay_ms(100);
			ability_check_mode_lcd();// RETURN?
			flag_ok = 0;
			return 1;
		}
	}
}
/**
 * @brief     This function is used to check if the attack is a downed attack on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_down_check(int x)
{
	lcd_roll_prompt_selection = 0;
		flag_left = 0;
		flag_right = 0;
		flag_ok = 0;
	ST7789_Fill(0xFFE0);
	if(x == 0)
	{
		ST7789_DrawString(20, 40, "PLAYER", RED, 4);
	}else if(x == 1)
	{
		ST7789_DrawString(20, 40, "ENEMY", RED, 4);
	}
	ST7789_DrawString(88, 88, "DOWN?", RED, 4);
	//Display initial prompt
	lcd_down_check_draw(lcd_roll_prompt_selection);
		//selection
		while(1)
					{
			 if (lcd_roll_prompt_selection < 0)
					    {
					                lcd_roll_prompt_selection = 0;
					    }
					    if (lcd_roll_prompt_selection > 1)
					    {
					                lcd_roll_prompt_selection = 1;
					    }
					if(flag_left || flag_right)
					{
						flag_right = 0;
						flag_left = 0;
						lcd_down_check_draw(lcd_roll_prompt_selection);
					}
					if(flag_ok)
					{
						flag_ok = 0;
						if(lcd_roll_prompt_selection == 0)
							return 1;
						else
							return 0;
					}
					}
}
/**
 * @brief     This function is used to prompt the player/DM to continue without rolling dice 
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_no_roll_prompt(void)
{
	lcd_roll_prompt_selection = 0;
	flag_left = 0;
	flag_right = 0;
	flag_ok = 0;
	ST7789_Fill(0xFFE0);
	draw_centered_string(40, "This Action Doesn't", RED, 2);
	draw_centered_string(80 ,"REQUIRE TO ROLL DICE", RED, 1);
	//Display initial prompt
	lcd_no_roll_prompt_draw(lcd_roll_prompt_selection);
	while(1)
	{
	    if (lcd_roll_prompt_selection < 0)
		    {
		                lcd_roll_prompt_selection = 0;   // 0 = ROLL, 1 = CANCEL
		    }
		    if (lcd_roll_prompt_selection > 1)
		    {
		                lcd_roll_prompt_selection = 1;
		    }
					if(flag_left || flag_right)
					{
						flag_right = 0;
						flag_left = 0;
						lcd_no_roll_prompt_draw(lcd_roll_prompt_selection);
					}
					if(flag_ok)
					{
						flag_ok = 0;
						if(lcd_roll_prompt_selection == 0)
							return 1;//ROLL DICE
						else
							return 0;//CANCEL
					}
					}



}
/**
 * @brief     This function is used to reroll the dice for advantage or disadvantage on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_reroll_prompt(void)
{
	lcd_roll_prompt_selection = 0;
		flag_left = 0;
		flag_right = 0;
		flag_ok = 0;
	ST7789_Fill(0xFFE0);
	draw_centered_string(40, "REROLL", RED, 4);
	draw_centered_string(100, "FOR ADVANTAGE/DISADVANTAGE", RED, 2);
	//Display initial prompt
	lcd_roll_prompt_draw(lcd_roll_prompt_selection);
	//selection
	while(1)
				{
		if (lcd_roll_prompt_selection < 0)
				    {
				                lcd_roll_prompt_selection = 0;   // 0 = ROLL, 1 = CANCEL
				    }
				    if (lcd_roll_prompt_selection > 1)
				    {
				                lcd_roll_prompt_selection = 1;
				    }
				if(flag_left || flag_right)
				{
					flag_right = 0;
					flag_left = 0;
				lcd_roll_prompt_draw(lcd_roll_prompt_selection);
				}
				if(flag_ok)
				{
					flag_ok = 0;
					if(lcd_roll_prompt_selection == 0)
						return 1;//ROLL DICE
					else
						return 0;//CANCEL
				}
				}
}
/**
 * @brief     This function is used prompt the player/DM to confirm hit, miss or critical hit on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_confirm_hit_prompt(int tohitbonus, int isplayer)
{
	int selection = 1; // default = HIT

	    // Clear flags before starting
	flag_left =0;
	flag_right = 0;
	flag_ok = 0;
	char buf[16];
	ST7789_Fill(0xFFE0);
	if(isplayer == 1)
	{
		ST7789_DrawString(20, 40, "TO-HIT BONUS", RED, 4);
		sprintf(buf, "%d", tohitbonus);
		ST7789_DrawString(20, 80, buf, RED, 4);
	}else
	{
		ST7789_DrawString(20, 40, "DM", RED, 4);
	}
	lcd_confirm_hit_prompt_draw(selection);
	while(1)
	{
        if (flag_left)
        {
            flag_left = 0; // clear

            if (selection > 0)
                selection--;

            lcd_confirm_hit_prompt_draw(selection);
        }

        if (flag_right)
        {
            flag_right = 0;

            if (selection < 2)
                selection++;

            lcd_confirm_hit_prompt_draw(selection);
        }

        // OK from ISR → finish
        if (flag_ok)
        {
            flag_ok = 0;
            return selection;   // 0=MISS, 1=HIT, 2=CRIT
        }
	}


}
/**
 * @brief     This function is used to select enemy reaction.
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
//Check this function
int lcd_attack_reaction_prompt(void)
{
	lcd_roll_prompt_selection = 0;
	flag_left =0;
		flag_right = 0;
		flag_ok = 0;
	ST7789_Fill(0xFFE0);
	ST7789_DrawString(20, 40, "ENEMY REACTION", RED, 4);
	//Display initial prompt
	lcd_roll_prompt_draw(lcd_roll_prompt_selection);
	//selection
	while(1)
	{
		if (lcd_roll_prompt_selection < 0)
				    {
				                lcd_roll_prompt_selection = 0;   // 0 = ROLL, 1 = CANCEL
				    }
				    if (lcd_roll_prompt_selection > 1)
				    {
				                lcd_roll_prompt_selection = 1;
				    }

							if(flag_left || flag_right)
							{
								flag_left = 0;
								flag_right = 0;
								lcd_down_check_draw(lcd_roll_prompt_selection);
							}
							if(flag_ok)
							{
								flag_ok = 0;
								if(lcd_roll_prompt_selection == 0)
									return 1;//ROLL DICE
								else
									return 0;//CANCEL
							}
    }
}
/**
 * @brief     This function is used to show the user to add bonus to the damage rolled  
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */

int lcd_roll_damage_prompt(int damage, int bonus)
{
	flag_ok = 0;
	char x_buf[16];
	char y_buf[16];
	char buf[16];

	sprintf(buf, "%d", bonus);
	int x = damage >> 4;
	sprintf(x_buf, "%d", x);
	int y = damage & 0xF;
	sprintf(y_buf, "%d", y);
	ST7789_Fill(0xFFE0);
    ST7789_DrawString(20, 40, "ROLL DICE", RED, 3);
    ST7789_DrawString(20, 80, "AND ADD ", RED, 3);
    ST7789_DrawString(140, 80, buf, RED, 3);

    ST7789_DrawString(20, 120, "ROLL DICE", RED, 3);
while(1)
{
    if(flag_ok)
    {
    	flag_ok = 0;
    	 ST7789_DrawString(20, 120, "ROLL DICE", AQUA, 3);
    	 return 1;
    }
}
}
/**
 * @brief     This function is used to promp the user to enter number of enemeis on LCD
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_enemies_prompt(int current_num_enemies)
{
	int x = 0;
	char buf[16];
	flag_ok = 0;
	ST7789_Fill(0xFFE0);
	ST7789_DrawString(20, 40, "HOW MANY ENEMIES?", RED, 3);
	ST7789_DrawString(20, 200, "OK", RED, 4);
while(1)
{
	if(flag_left)
	{
		flag_left = 0;
        x--;
        if(x <= 0)
        {
        	x = 0;
        	 ST7789_FillRect(0, 80, 320, 40, 0xFFE0);
        	ST7789_DrawString(20, 80, "CANCEL", RED, 4);
        }else
        {
        	sprintf(buf, "%d", x);
        	ST7789_FillRect(0, 80, 320, 40, 0xFFE0);
        	ST7789_DrawString(20, 80, buf, RED, 4);
        }
	}
	if(flag_right)
	{
		flag_right = 0;
		x++;
		if(x >= current_num_enemies)
		{
			x = current_num_enemies;
			sprintf(buf, "%d", x);
			ST7789_FillRect(0, 80, 320, 40, 0xFFE0);
			ST7789_DrawString(20, 80, buf, RED, 4);
		}else
		{
			sprintf(buf, "%d", x);
			ST7789_FillRect(0, 80, 320, 40, 0xFFE0);
			ST7789_DrawString(20, 80, buf, RED, 4);
		}
	}


	if(flag_ok)
	{
		flag_ok = 0;
		ST7789_Fill(0xFFE0);
		return x;
	}
}

}

/**
 * @brief     This function is used to scroll the actions information when the action selecter index is less or geater than the visible items
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_actions_prompt(character_t current, int turn)
{
	// Clear flags before starting
	flag_left = 0;
	flag_right = 0;
	flag_ok = 0;
	cursor_index =0;
	top_index =0;
	ST7789_Fill(0xFFE0);
	ST7789_DrawString(20, 10, (char*)current.name, RED, 2);
	char buf[16];
	sprintf(buf, "%d", turn);
	ST7789_DrawString(20, 30, buf, RED, 4);
while(1)
{

	if(flag_left)
	{
		flag_left = 0;
		 cursor_index++;
		 if(cursor_index >13)
		{
			cursor_index = 13;
		}
		 if (cursor_index < 13)
		 {
		 	 if (cursor_index >= top_index + visible_items)
		 	 {
		 	  	top_index++;
		 	  	scroll_actions_info(+1);  // Slide down
		 	 } else {
		 		action_menu();
		 	 }
		 }else {

		 		 action_menu();

		 }
	}
	if(flag_right)
	{
		flag_right = 0;
		if(cursor_index >0)
		{
			cursor_index--;
		}
		else{
		  cursor_index = 0;
		}
		 if (cursor_index < 13) {
		  	if (cursor_index < top_index)
		  	{
		  		top_index--;
		  		scroll_actions_info(-1);   // Slide up
		  	} else {
		  		action_menu();
		  	}
		  	} else {

		  	action_menu();
		}

	}
	if(flag_ok)
	{
		flag_ok =0;
		return cursor_index;
	}

}
}

////prompt player to select a character from the list to perform the ability check, present character names alongside ability modifier or cancel options - lcd.c
int lcd_list_characters_prompt(character_t *characters, int ability)
{
    lcd_roll_prompt_selection = 0;

	// Clear flags before starting
	flag_left = 0;
	flag_right = 0;
	flag_ok = 0;
	char shortened_name[20];
	int y = 80;
	ST7789_Fill(0xFFE0);
	ST7789_DrawString(10, 20, "Select Character:", RED, 3);
	//do the thing where as many characters are displayed on the screen as possible and implement scrolling if needed
	for (int i = 0; i < 6; i++)
	{
		if(characters[i].name[0] == 0)
		{
			continue;
		}
		for (int j = 0; j < 20; j++)
		{
			shortened_name[j] = characters[i].name[j];
		}
		//scroll_character_info ... is this the right place to call it?
		//show their shortened names and ability modifier alongside that
		ST7789_DrawString( 10,y, shortened_name, RED, 2);
		y += 40;
	}

	while(1)
	{
		if (lcd_roll_prompt_selection < 6)
		{
			while (characters[lcd_roll_prompt_selection].name[0] == 0)
			{
				if (flag_left)
				{
					flag_right = 0;
					lcd_roll_prompt_selection--;
				}
				if (flag_right)
				{
					flag_left = 0;
					lcd_roll_prompt_selection++;
				}
				if (lcd_roll_prompt_selection > 5)
				{
					lcd_roll_prompt_selection = 0;
				}
				if (lcd_roll_prompt_selection < 0)
				{
					lcd_roll_prompt_selection = 5;
				}
			}
		}
		if (lcd_roll_prompt_selection < 0)
		{
			lcd_roll_prompt_selection = 0;
		}
		if (lcd_roll_prompt_selection > 6) //6 is the maximum number of characters and prompt selection is 0-indexed, so 6 would represent cancel
		{
			lcd_roll_prompt_selection = 6;
		}
		if (flag_left || flag_right)
		{
			flag_right = 0;
			flag_left = 0;
			//call the draw function that will display each character shortened_name and ability modifier
			character_check_menu(characters);
		}
		if (flag_ok)
		{
			flag_ok = 0;
			if (lcd_roll_prompt_selection == 6)
				return 0; //CANCEL
			else
				return (lcd_roll_prompt_selection + 1); //return character number
		}
	}
}
/**
 * @brief     This function is used to scroll the ability infomration when the ability count selecter index is less or geater than the visible items
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_list_abilities_prompt(void)
{
	// Clear flags before starting
	flag_left = 0;
	flag_right = 0;
	flag_ok = 0;
	cursor_index =0;
	top_index =0;
	ST7789_Fill(0xFFE0);
	ST7789_DrawString(10, 20, "SELECT ABILITY:", RED, 3);

	while(1)
	{

		if(flag_left)
		{
			flag_left = 0;
			 cursor_index++;
			 if(cursor_index >19)
			{
				cursor_index = 19;
			}
			 if (cursor_index < 19)
			 {
			 	 if (cursor_index >= top_index + visible_items)
			 	 {
			 	  	top_index++;
			 	  	scroll_ability_info(+1);  // Slide down
			 	 } else {
			 		ability_check_menu();
			 	 }
			 }else {

				 ability_check_menu();

			 }
		}
		if(flag_right)
		{
			flag_right = 0;
			if(cursor_index >0)
			{
				cursor_index--;
			}
			else{
			  cursor_index = 0;
			}
			 if (cursor_index < 19) {
			  	if (cursor_index < top_index)
			  	{
			  		top_index--;
			  		scroll_ability_info(-1);   // Slide up
			  	} else {
			  		ability_check_menu();
			  	}
			  	} else {

			  		ability_check_menu();
			}

		}
		if(flag_ok)
		{
			flag_ok =0;
			return cursor_index;
		}

	}
}

/**
 * @brief     This function is used to scroll the character information when the character count selecter index is less or geater than the visible items
 * on the LCD then the top index is incrementd or decremented as per the logic.
 *
 * @param[in]  NA
 *
 * @return     NA
 * Author: Likhita Jonnakuti
 *
 */
int lcd_turn_order_prompt(character_t *turn_order, character_t *characters)
{
	flag_left = 0;
		flag_right = 0;
		flag_ok = 0;
		cursor_index =0;
		top_index =0;
		int character_count = 0;
		int enemy_count = 0;

		for (int i = 0; i < 16; i++)
		{
			if (turn_order[i].name[0] >= 32)
			{
				character_count++;
			}
			if (turn_order[i].name[0] == 1)
			{
				enemy_count++;
			}
		}

		character_menu(turn_order, (character_count + enemy_count));
	 while(1)
	  	  {
	  		  if(flag_ok)
	  		  {
	  			flag_ok = 0;

	  			  if(cursor_index < (character_count + enemy_count))
	  			  {
	  				  //Selecting a character name
	  				  if(selected_index == -1)
	  				  {
	  					  selected_index = cursor_index;
	  				  }
	  				  else
	  				  {
	  					  selected_index = -1;
	  				  }
	  			  }
	  			  else {
	  				//Confirm or Cancel do nothing
	  				selected_index = -1;
	  				if(cursor_index == (character_count + enemy_count))
	  				{
	  					return 1;
	  				}
	  				else
	  				{
	  					return 0;
	  				}

	  			  }
	  			  character_menu(turn_order, (character_count + enemy_count));
	  		  }

	  		  if(flag_left)
	  		  {
	  			  flag_left = 0;
	  			  //Move Cursor
	  		
	  			  cursor_index++;
	  			  if(cursor_index > (character_count + enemy_count+ 1) )
	  			  {
	  				  cursor_index = (character_count + enemy_count+ 1);
	  			  }

	  			  if(selected_index != -1)
	  			  {
	  				  if(selected_index <  (character_count + enemy_count - 1))
	  				  {
						  character_t tempc;
						  memcpy(&tempc, &turn_order[selected_index], sizeof(character_t));
						  memcpy(&turn_order[selected_index], &turn_order[selected_index + 1], sizeof(character_t));
						  memcpy(&turn_order[selected_index + 1], &tempc, sizeof(character_t));
	  					  selected_index++;
	  				  }
	  			  }
	  		    // Scroll Window
	  		        if (cursor_index <  (character_count + enemy_count)) {
	  		            if (cursor_index >= top_index + visible_items) {
	  		                top_index++;
	  		              scroll_character_info(+1, characters);  // Slide down
	  		            } else {
	  		            	character_menu(turn_order, (character_count + enemy_count));
	  		            }
	  		        } else {
	  		        	if(selected_index != -1){}else
	  		        	  			  			        		{character_menu(turn_order, (character_count + enemy_count));}
	  		        }
	  		  }
	  		  if(flag_right)
	  		  {
	  			flag_right = 0;
	  		 //  int old_cursor = cursor_index;
	  			  			  //Move Cursor

	  			  			  if(cursor_index >0)
	  			  			  {
	  			  				  cursor_index--;
	  			  			  }
	  			  			  else{
	  			  				  cursor_index = 0;
	  			  			  }

	  			  			  if(selected_index != -1)
	  			  			  {
	  			  				  if(selected_index > 0)
	  			  				  {
	  			  					  character_t tempc;
	  			  					  memcpy(&tempc, &turn_order[selected_index], sizeof(character_t));
	  			  					  memcpy(&turn_order[selected_index], &turn_order[selected_index - 1], sizeof(character_t));
	  			  					  memcpy(&turn_order[selected_index - 1], &tempc, sizeof(character_t));
	  			  					  selected_index--;
	  			  				  }
	  			  			  }
	  			  			// Scroll Window
							if (cursor_index <  (character_count + enemy_count))
							{
								if (cursor_index < top_index)
								{
									top_index--;
									scroll_character_info(-1, characters);   // Slide up
								}
								else
								{
									character_menu(turn_order, (character_count + enemy_count));
								}
							}
							else
							{
								if(selected_index != -1){}
								else
								{
									character_menu(turn_order, (character_count + enemy_count));
								}
							}
	  		  }
	  	  }
}
