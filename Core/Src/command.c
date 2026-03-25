/**
  ******************************************************************************
  * @file           : command.c
  * @brief          : Command Processor Initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) December 1st, 2025 Kenneth Alcineus.
  * All rights reserved.
  *
  * Much of the command processor code is credited towards Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
  * The character command itself is unique to this project, but the actual processing of the commands will not be credited towards the final project.
  *
  ******************************************************************************
  */

#include <game_definitions.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "EEPROM.h"
#include "i2c.h"
#include "sys_timer_delay.h"
#include "uart.h"

#include "main.h"

#define MAX_STR_LEN 256

extern volatile int flag_ok;

//This typedef is attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
typedef void (*command_handler_t)(int, char argv[][MAX_STR_LEN]);

//This typedef is attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
typedef struct
{
	const char *name;
	command_handler_t handler;
} command_table_t;


//function to receive user input into a line
//This function is attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
void accumulate_line(char *s)
{
	char c = 0xFF;
	int len = 0;

	while (len != MAX_STR_LEN)
	{
		while (c == 0xFF)
		{
			c = (char)getc(stdin);
		}

		if (c == '\t' || c == '\v')
		{
			c = ' ';
		}

		if (c == '\b' || c == 0x7F)
		{
			if (len > 0)
			{
				s[--len] = '\0';
				printf("\b \b");
			}
			c = 0xFF;
			continue;
		}

		printf("%c", c);
		s[len++] = c;

		if (c == '\r')
		{
			break;
		}

		c = 0xFF;
	}

	printf("\n");

	if (len != MAX_STR_LEN)
	{
		s[len] = '\0';
	}
}

//function to check for invalid commands
//This function is attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
void unknown_command(int argc, char argv[][MAX_STR_LEN])
{
	printf("Unknown command <");
	printf("%s", argv[0]);

	for (int i = 1; i < argc; i++)
	{
		printf(" %s", argv[i]);
	}

	printf(">.\r\n");
}

//function to print the authors of the program
//This function is attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
void handle_author(int argc, char argv[][MAX_STR_LEN])
{
	if (argc > 1)
	{
		unknown_command(argc, argv);
		return;
	}

	printf("Kenneth Alcineus & Likhita Jonnakuti\r\n");
}

//function to test if the solenoid is in working condition
//This function represents the handle_led function attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
void handle_test(int argc, char argv[][MAX_STR_LEN])
{
	if (argc > 2)
	{
		unknown_command(argc, argv);
		return;
	}

	int blink_count = 0;

	//the following if condition is attributed to https://stackoverflow.com/a/39052987 (also referenced in Assignment 1)
	if (argv[1][0] == '0' && (argv[1][1] == 'x' || argv[1][1] == 'X'))
	{
		int hexstr_len = 2;
	    while (argv[1][hexstr_len] != '\0')
	    {
	    	blink_count <<= 4;
	        if ((argv[1][hexstr_len] >= '0' && argv[1][hexstr_len] <= '9'))
	        {
	        	blink_count += (argv[1][hexstr_len] - '0');
	        }
	        else if ((argv[1][hexstr_len] >= 'A' && argv[1][hexstr_len] <= 'F'))
	        {
	        	blink_count += (argv[1][hexstr_len] - 'A' + 10);
	        }
	        else if ((argv[1][hexstr_len] >= 'a' && argv[1][hexstr_len] <= 'f'))
	        {
	        	blink_count += (argv[1][hexstr_len] - 'a' + 10);
	        }
	        else
	        {
	            blink_count = 0;
	            break;
	        }
	        hexstr_len++;
	    }
	}

	else
	{
		blink_count = atoi(argv[1]);
	}

	if (blink_count <= 0)
	{
		unknown_command(argc, argv);
		return;
	}

	//simple test routine for the LED pin which also controls the solenoid
	for (int i = 0; i < blink_count; i++)
	{
		  GPIOD->BSRR |= GPIO_BSRR_BR12;
		  for(volatile int i = 0; i < 10000000; i++);
		  GPIOD->BSRR |= GPIO_BSRR_BS12;
		  for(int i = 0; i < 10000000; i++);
	}
}

//this command allows for writing bytes to an address on the EEPROM or performing a full data dump for the entire address space
void handle_hexedit(int argc, char argv[][MAX_STR_LEN])
{
	if (argc > 4)
	{
		unknown_command(argc, argv);
		return;
	}

	if (strcasecmp(argv[1], "DUMP") == 0)
	{
		for (int i = 0; i < 0x800; i += 16)
		{
			uint8_t byte_read = 0;
			printf("%03X : ", i);
			for (int j = 0; j < 16; j++)
			{
				EEPROM_ReadByte((i + j), &byte_read);
				delay_ms(10);
				printf("%02X ", (int)byte_read);
			}
			printf("\r\n");
		}
		return;
	}

	if ((strcasecmp(argv[1], "WRITE") == 0))
	{
		int hexstr_len;
		int address;

		//the following if condition is attributed to https://stackoverflow.com/a/39052987 (also referenced in Assignment 1)
		if (argv[2][0] == '0' && (argv[2][1] == 'x' || argv[2][1] == 'X'))
		{
			hexstr_len = 2;
		    while (argv[2][hexstr_len] != '\0')
		    {
		    	address <<= 4;
		        if ((argv[2][hexstr_len] >= '0' && argv[2][hexstr_len] <= '9'))
		        {
		        	address += (argv[2][hexstr_len] - '0');
		        }
		        else if ((argv[2][hexstr_len] >= 'A' && argv[2][hexstr_len] <= 'F'))
		        {
		        	address += (argv[2][hexstr_len] - 'A' + 10);
		        }
		        else if ((argv[2][hexstr_len] >= 'a' && argv[2][hexstr_len] <= 'f'))
		        {
		        	address += (argv[2][hexstr_len] - 'a' + 10);
		        }
		        else
		        {
		        	address = 0;
		            break;
		        }
		        hexstr_len++;
		    }
		}

		else
		{
			address = atoi(argv[2]);
		}

		if (address > 0x7FF || address < 0)
		{
			printf("Invalid address!");
			return;
		}

		int data;
		//the following if condition is attributed to https://stackoverflow.com/a/39052987 (also referenced in Assignment 1)
		if (argv[3][0] == '0' && (argv[3][1] == 'x' || argv[3][1] == 'X'))
		{
			hexstr_len = 2;
		    while (argv[3][hexstr_len] != '\0')
		    {
		    	address <<= 4;
		        if ((argv[3][hexstr_len] >= '0' && argv[3][hexstr_len] <= '9'))
		        {
		        	data += (argv[3][hexstr_len] - '0');
		        }
		        else if ((argv[3][hexstr_len] >= 'A' && argv[3][hexstr_len] <= 'F'))
		        {
		        	data += (argv[3][hexstr_len] - 'A' + 10);
		        }
		        else if ((argv[3][hexstr_len] >= 'a' && argv[3][hexstr_len] <= 'f'))
		        {
		        	data += (argv[3][hexstr_len] - 'a' + 10);
		        }
		        else
		        {
		        	data = 0;
		            break;
		        }
		        hexstr_len++;
		    }
		}

		else
		{
			data = atoi(argv[3]);
		}

		if (data > 0xFF || data < 0)
		{
			printf("Invalid data!");
			return;
		}

		EEPROM_WriteByte((uint16_t)address, (uint8_t)data);
		printf("Data written!\r\n");
		return;
	}

}

int get_int_input(int max_value, int min_value, char* input_str)
{
  //
  accumulate_line(input_str);
  int temp_int = 0;
  char input_string_digits[2];
  input_string_digits[0] = 0;
  input_string_digits[1] = 0;
  input_string_digits[0] = input_str[0];
  input_string_digits[1] = input_str[1];
  temp_int = atoi(input_string_digits);
  if (temp_int > max_value || temp_int < min_value)
  {
          printf("Invalid input, aborting character creation...\r\n");
          return -1000;
  }
  return temp_int;
}

//this command allows users to create, read, update, list, and delete characters
void handle_character(int argc, char argv[][MAX_STR_LEN])
{
	char input_string[MAX_STR_LEN];
	uint8_t character_data[0x100];
	int data_index = 0;
	int data_int = 0;

	if (argc == 1)
	{
		unknown_command(argc, argv);
		return;
	}

	if (argc > 3)
	{
		unknown_command(argc, argv);
		return;
	}

	if (strcasecmp(argv[1], "CREATE") == 0 || strcasecmp(argv[1], "UPDATE") == 0)
	{
		if (argc > 3 && strcasecmp(argv[1], "UPDATE") == 0)
		{
			unknown_command(argc, argv);
			return;
		}

		if (argc > 2)
		{
			unknown_command(argc, argv);
			return;
		}

		int character_num = 0;

		if (strcasecmp(argv[1], "UPDATE") == 0)
		{
			character_num = atoi(argv[2]);
			if (character_num > 6 || character_num < 1)
			{
					printf("Invalid input!\r\n");
					return;
			}
			character_t c = get_character(character_num);
			if (c.name[0] == 0)
			{
				printf("A character does not exist in this slot!\r\n");
				return;
			}
		}

		else
		{
			for (int i = 1; i < 7; i++)
			{
				character_t c = get_character(i);
				if (c.name[0] == 0)
				{
					character_num = i;
					break;
				}
			}

			if (character_num == 0)
			{
				printf("There are no free character slots remaining! Delete a character or clear all characters in order to create a new one.\r\n");
				return;
			}
		}

		printf("Enter character name (up to 64 characters): ");
		accumulate_line(input_string);

		while (data_index < 64)
		{
			character_data[data_index] = (uint8_t)input_string[data_index];
			data_index++;
		}

		printf("\r\nSpecies:\r\n");
		printf("1 - Aasimar\r\n");
		printf("2 - Dragonborn\r\n");
		printf("3 - Dwarf\r\n");
		printf("4 - Elf\r\n");
		printf("5 - Gnome\r\n");
		printf("6 - Goliath\r\n");
		printf("7 - Halfling\r\n");
		printf("8 - Human\r\n");
		printf("9 - Orc\r\n");
		printf("10 - Tiefling\r\n");
		printf("Enter a number representing character species: ");
		data_int = get_int_input(10, 1, input_string);
		if (data_int == -1000)
		{
		    return;
		}
		character_data[data_index] = (uint8_t)data_int;
		data_index++;
		
		printf("\r\nClass:\r\n");
		printf("1 - Barbarian\r\n");
		printf("2 - Bard\r\n");
		printf("3 - Cleric\r\n");
		printf("4 - Druid\r\n");
		printf("5 - Fighter\r\n");
		printf("6 - Monk\r\n");
		printf("7 - Paladin\r\n");
		printf("8 - Ranger\r\n");
		printf("9 - Rogue\r\n");
		printf("10 - Sorcerer\r\n");
		printf("11 - Warlock\r\n");
		printf("12 - Wizard\r\n");
		printf("Enter a number representing character class: ");
		data_int = get_int_input(12, 1, input_string);
		if (data_int == -1000)
		{
		    return;
		}
		character_data[data_index] = (uint8_t)data_int;
		data_index++;
		
		printf("Enter a number representing class level: ");
		data_int = get_int_input(20, 1, input_string);
		if (data_int == -1000)
		{
		    return;
		}
		character_data[data_index] = (uint8_t)data_int;
		data_index++;
		
		printf("Enter a number representing character secondary class, or enter 0 if not applicable: ");
		data_int = get_int_input(12, 0, input_string);
		if (data_int == -1000)
		{
		    return;
		}
		character_data[data_index] = (uint8_t)data_int;
		data_index++;
		if (data_int == 0)
		{
		        character_data[data_index++] = 0;
		        character_data[data_index++] = 0;
		        character_data[data_index++] = 0;
		        data_int = 1;
		}
		else
		{
		        printf("Enter a number representing secondary class level: ");
		        data_int = get_int_input(20, 1, input_string);
		        if (data_int == -1000)
		        {
		            return;
		        }
		        character_data[data_index] = (uint8_t)data_int;
		        data_index++;
		        
		        printf("Enter a number representing character tertiary class, or enter 0 if not applicable: ");
		        data_int = get_int_input(12, 0, input_string);
		        if (data_int == -1000)
		        {
		            return;
		        }
		        character_data[data_index] = (uint8_t)data_int;
		        data_index++;
		    
		        if (data_int == 0)
		        {
		                character_data[data_index++] = 0;
		                data_int = 1;
		        }
		        else
		        {

		                printf("Enter a number representing tertiary class level: ");
		                data_int = get_int_input(20, 1, input_string);
		                if (data_int == -1000)
		                {
		                    return;
		                }
		                character_data[data_index] = (uint8_t)data_int;
		                data_index++;
		        }
		}
		if (data_int == 0)
		{
		        printf("Invalid input, aborting character creation...\r\n");
		        return;
		}
		
		printf("\r\nFeats:\r\n");
		printf("Entering feats is currently unimplemented, moving on...\r\n");
		for (int i = 0; i < 8; i++)
		{
		        character_data[data_index] = 0;
		        data_index++;
		}
		
		printf("Enter character proficiency bonus: ");
		data_int = get_int_input(12, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character armor class: ");
		data_int = get_int_input(50, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character initiative: ");
		data_int = get_int_input(50, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character current hit points: ");
		data_int = get_int_input(255, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character maximum hit points: ");
		data_int = get_int_input(255, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character temporary hit points: ");
		data_int = get_int_input(255, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character strength ability modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character strength saving throw modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character dexterity ability modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character dexterity saving throw modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character constitution ability modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character constitution saving throw modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character intelligence ability modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character intelligence saving throw modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character wisdom ability modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character wisdom saving throw modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character charisma ability modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character charisma saving throw modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Entering armory and weapon proficiency is currently unimplemented, moving on...\r\n");
		printf("NOTE: character assumed to have proficiency with equipped weapons\r\n");
		for (int i = 0; i < 42; i++)
		{
		        character_data[data_index] = 1;
		        data_index++;
		}
		
		printf("Enter character acrobatics skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character animal handling skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character arcana skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character athletics skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character deception skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character history skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character insight skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character intimidation skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character investigation skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character medicine skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character nature skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;

		printf("Enter character perception skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character performance skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character persuasion skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character religion skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character sleight of hand skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character stealth skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character survival skill modifier: ");
		data_int = get_int_input(20, -20, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character acid resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character bludgeoning resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character cold resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character fire resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character force resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character lightning resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character necrotic resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character piercing resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character poison resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character psychic resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character radiant resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character slashing resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Enter character thunder resistance (1 = vulnerable, 2 = normal, 3 = resistant, 4 = immune): ");
		data_int = get_int_input(4, 1, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		
		printf("Entering status conditions is currently unimplemented, moving on...\r\n");
		for (int i = 0; i < 14; i++)
		{
		        character_data[data_index] = 0;
		        data_index++;
		}
		
		printf("Equipped Weapons:\r\n");
		printf("1 - Club           \t 20 - Halberd\r\n");
		printf("2 - Dagger         \t 21 - Lance\r\n");
		printf("3 - Greatclub      \t 22 - Longsword\r\n");
		printf("4 - Handaxe        \t 23 - Maul\r\n");
		printf("5 - Javelin        \t 24 - Morningstar\r\n");
		printf("6 - Light Hammer   \t 25 - Pike\r\n");
		printf("7 - Mace           \t 26 - Rapier\r\n");
		printf("8 - Quarterstaff   \t 27 - Scimitar\r\n");
		printf("9 - Sickle         \t 28 - Shortsword\r\n");
		printf("10 - Spear         \t 29 - Trident\r\n");
		printf("11 - Dart          \t 30 - Warhammer\r\n");
		printf("12 - Light Crossbow\t 31 - War Pick\r\n");
		printf("13 - Shortbow      \t 32 - Whip\r\n");
		printf("14 - Sling         \t 33 - Blowgun\r\n");
		printf("15 - Battleaxe     \t 34 - Hand Crossbow\r\n");
		printf("16 - Flail         \t 35 - Heavy Crossbow\r\n");
		printf("17 - Glaive        \t 36 - Longbow\r\n");
		printf("18 - Greataxe      \t 37 - Musket\r\n");
		printf("19 - Greatsword    \t 38 - Pistol\r\n");
		printf("Enter a number representing first equipped weapon (enter 0 if no weapons equipped): ");
		data_int = get_int_input(38, 0, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		if (data_int == 0)
		{
		        for(int i = 0; i < 25; i++)
		        {
		                character_data[data_index] = (uint8_t)data_int;
		                data_index++;
		        }
		        set_character(character_num, character_data);
		        printf("Character created!");
		        return;
		}
		
		printf("Enter a number representing second equipped weapon (enter 0 if no other weapons equipped): ");
		data_int = get_int_input(38, 0, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		if (data_int == 0)
		{
		        for(int i = 0; i < 24; i++)
		        {
		                character_data[data_index] = (uint8_t)data_int;
		                data_index++;
		        }
		        set_character(character_num, character_data);
		        printf("Character created!");
		        return;
		}
		
		printf("Enter a number representing third equipped weapon (enter 0 if no other weapons equipped): ");
		data_int = get_int_input(38, 0, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		if (data_int == 0)
		{
		        for(int i = 0; i < 23; i++)
		        {
		                character_data[data_index] = (uint8_t)data_int;
		                data_index++;
		        }
		        set_character(character_num, character_data);
		        printf("Character created!");
		        return;
		}
		
		printf("Enter a number representing fourth equipped weapon (enter 0 if no other weapons equipped): ");
		data_int = get_int_input(38, 0, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		if (data_int == 0)
		{
		        for(int i = 0; i < 22; i++)
		        {
		                character_data[data_index] = (uint8_t)data_int;
		                data_index++;
		        }
		        set_character(character_num, character_data);
		        printf("Character created!");
		        return;
		}
		
		printf("Enter a number representing fifth equipped weapon (enter 0 if no other weapons equipped): ");
		data_int = get_int_input(38, 0, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		if (data_int == 0)
		{
		        for(int i = 0; i < 21; i++)
		        {
		                character_data[data_index] = (uint8_t)data_int;
		                data_index++;
		        }
		        set_character(character_num, character_data);
		        printf("Character created!");
		        return;
		}
		
		printf("Enter a number representing sixth equipped weapon (enter 0 if no other weapons equipped): ");
		data_int = get_int_input(38, 0, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		if (data_int == 0)
		{
		        for(int i = 0; i < 20; i++)
		        {
		                character_data[data_index] = (uint8_t)data_int;
		                data_index++;
		        }
		        set_character(character_num, character_data);
		        printf("Character created!");
		        return;
		}
		printf("Enter a number representing seventh equipped weapon (enter 0 if no other weapons equipped): ");
		data_int = get_int_input(38, 0, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
		if (data_int == 0)
		{
		        for(int i = 0; i < 19; i++)
		        {
		                character_data[data_index] = (uint8_t)data_int;
		                data_index++;
		        }
		        set_character(character_num, character_data);
		        printf("Character created!");
		        return;
		}
		printf("Enter a number representing eighth equipped weapon (enter 0 if no other weapons equipped): ");
		data_int = get_int_input(38, 0, input_string);
	        if (data_int == -1000)
	        {
	            return;
	        }
	        character_data[data_index] = (uint8_t)data_int;
	        data_index++;
	        
		for(int i = 0; i < 18; i++)
		{
				character_data[data_index] = (uint8_t)data_int;
				data_index++;
		}
		printf("Writing to memory, be patient...\r\n");
		int result = set_character(character_num, character_data);
		if (result == 1)
		{
			printf("Character created!\r\n");

		}
		else
		{
			printf("Something went wrong!\r\n");
		}

		return;
	}

	if (strcasecmp(argv[1], "READ") == 0)
	{
		if (argc < 3)
		{
			unknown_command(argc, argv);
			return;
		}
		int character_num = atoi(argv[2]);
		if (character_num > 6 || character_num < 1)
		{
		        printf("Invalid input!\r\n");
		        return;
		}
		character_t c = get_character(character_num);
		if (c.name[0] == 0)
		{
			printf("A character does not exist in this slot!\r\n");
			return;
		}
		printf("===General Info===\r\n");
		printf("Name: %s\r\n", c.name);
		printf("Species: %d\r\n", (int)c.species); //implement species lookup table
		printf("Primary Class & Subclass: %d, Level: %d\r\n", (int)c.primary_class_subclass, (int)c.primary_level); //implement class_subclass lookup table
		printf("Secondary Class & Subclass: %d, Level: %d\r\n", (int)c.secondary_class_subclass, (int)c.secondary_level);
		printf("Tertiary Class & Subclass: %d, Level: %d\r\n", (int)c.tertiary_class_subclass, (int)c.tertiary_level);
		printf("Proficiency Bonus: %d   Armor Class: %d    Initiative: %d\r\n", (int)c.proficiency_bonus, (int)c.armor_class, (int)c.initiative);
		printf("Current Hit Points: %d   Max Hit Points: %d  Temporary Hit Points: %d\r\n", (int)c.current_hit_points, (int)c.maximum_hit_points, (int)c.temporary_hit_points);
		printf("Strength Ability Modifier: %d   Strength Saving Throw Modifier: %d\r\n", (int)c.strength_ability_modifier, (int)c.strength_saving_throw_modifier);
		printf("Dexterity Ability Modifier: %d   Dexterity Saving Throw Modifier: %d\r\n", (int)c.dexterity_ability_modifier, (int)c.dexterity_saving_throw_modifier);
		printf("Constitution Ability Modifier: %d  Constitution Saving Throw Modifier: %d\r\n", (int)c.constitution_ability_modifier, (int)c.constitution_saving_throw_modifier);
		printf("Intelligence Ability Modifier: %d  Intelligence Saving Throw Modifier: %d\r\n", (int)c.intelligence_ability_modifier, (int)c.intelligence_saving_throw_modifier);
		printf("Wisdom Ability Modifier: %d  Wisdom Saving Throw Modifier: %d\r\n", (int)c.wisdom_ability_modifier, (int)c.wisdom_saving_throw_modifier);
		printf("Charisma Ability Modifier: %d  Charisma Saving Throw Modifier: %d\r\n", (int)c.charisma_ability_modifier, (int)c.charisma_saving_throw_modifier);
		printf("===Skills===\r\n");
		printf("Acrobatics Skill Modifier: %d\r\n", (int)c.acrobatics_skill_modifier);
		printf("Animal Handling Skill Modifier: %d\r\n", (int)c.animal_handling_skill_modifier);
		printf("Arcana Skill Modifier: %d\r\n", (int)c.arcana_skill_modifier);
		printf("Athletics Skill Modifier: %d\r\n", (int)c.athletics_skill_modifier);
		printf("Deception Skill Modifier: %d\r\n", (int)c.deception_skill_modifier);
		printf("History Skill Modifier: %d\r\n", (int)c.history_skill_modifier);
		printf("Insight Skill Modifier: %d\r\n", (int)c.insight_skill_modifier);
		printf("Intimidation Skill Modifier: %d\r\n", (int)c.intimidation_skill_modifier);
		printf("Investigation Skill Modifier: %d\r\n", (int)c.investigation_skill_modifier);
		printf("Medicine Skill Modifier: %d\r\n", (int)c.medicine_skill_modifier);
		printf("Nature Skill Modifier: %d\r\n", (int)c.nature_skill_modifier);
		printf("Perception Skill Modifier: %d\r\n", (int)c.perception_skill_modifier);
		printf("Performance Skill Modifier: %d\r\n", (int)c.performance_skill_modifier);
		printf("Persuasion Skill Modifier: %d\r\n", (int)c.persuasion_skill_modifier);
		printf("Religion Skill Modifier: %d\r\n", (int)c.religion_skill_modifier);
		printf("Sleight of Hand Skill Modifier: %d\r\n", (int)c.sleight_of_hand_skill_modifier);
		printf("Stealth Skill Modifier: %d\r\n", (int)c.stealth_skill_modifier);
		printf("Survival Skill Modifier: %d\r\n", (int)c.survival_skill_modifier);
		printf("===Resistances===\r\n");
		printf("Acid Resistance: %d\r\n", (int)c.acid_resistance);
		printf("Bludgeoning Resistance: %d\r\n", (int)c.bludgeoning_resistance);
		printf("Cold Resistance: %d\r\n", (int)c.cold_resistance);
		printf("Fire Resistance: %d\r\n", (int)c.fire_resistance);
		printf("Force Resistance: %d\r\n", (int)c.force_resistance);
		printf("Lightning Resistance: %d\r\n", (int)c.lightning_resistance);
		printf("Necrotic Resistance: %d\r\n", (int)c.necrotic_resistance);
		printf("Piercing Resistance: %d\r\n", (int)c.piercing_resistance);
		printf("Poison Resistance: %d\r\n", (int)c.poison_resistance);
		printf("Psychic Resistance: %d\r\n", (int)c.psychic_resistance);
		printf("Radiant Resistance: %d\r\n", (int)c.radiant_resistance);
		printf("Slashing Resistance: %d\r\n", (int)c.slashing_resistance);
		printf("Thunder Resistance: %d\r\n", (int)c.thunder_resistance);
		printf("===Equipment===\r\n");

		for (int i = 0; i < 8; i++)
		{
			if (c.equipped_weapons[i] == 0)
			{
				break;
			}
			printf("Weapon %d: %d\r\n", (i + 1), c.equipped_weapons[i]);
		}

		return;
	}

	if (strcasecmp(argv[1], "DELETE") == 0)
	{
		if (argc < 3)
		{
			unknown_command(argc, argv);
			return;
		}

		int character_num = atoi(argv[2]);
		if (character_num > 6 || character_num < 1)
		{
		        printf("Invalid input!\r\n");
		        return;
		}

		character_t c = get_character(character_num);
		if (c.name[0] == 0)
		{
			printf("A character does not exist in this slot!\r\n");
			return;
		}

		for (int i = 0; i < 0x100; i++)
		{
			character_data[i] = 0;
		}

		printf("Writing to memory, be patient...");
		set_character(character_num, character_data);

		printf("Character deleted!\r\n");
		return;
	}

	if (strcasecmp(argv[1], "LIST") == 0)
	{
		if (argc == 3)
		{
			unknown_command(argc, argv);
			return;
		}

		printf("The following characters are present:\r\n");

		char character_name[64];
		character_t c;

		for (int i = 1; i < 7; i++)
		{
			c = get_character(i);

			for (int j = 0; j < 64; j++)
			{
				character_name[j] = (char)c.name[j];
			}

			printf("%d - %s\r\n", i, character_name);
		}

		return;
	}

	if (strcasecmp(argv[1], "CLEAR") == 0)
	{
		if (argc == 3)
		{
			unknown_command(argc, argv);
			return;
		}

		printf("Clearing characters, please do not turn off the device...\r\n");

		erase_characters();

		printf("All characters have been cleared!\r\n");
		return;
	}
}

//sets the appropriate button flag to exit the serial state
void handle_exit(int argc, char argv[][MAX_STR_LEN])
{
	if (argc > 1)
	{
		unknown_command(argc, argv);
		return;
	}

	flag_ok = 0;
}


//This constant definition is attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
//The actual contents are the commands array are modified to suit the requirements of this project.
static const command_table_t commands[] =
{
	{"character", handle_character},
	{"author", handle_author},
	{"test", handle_test},
	{"hexedit", handle_hexedit},
	{"exit", handle_exit}
};

//This constant is attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
static const int num_commands = sizeof(commands) / sizeof(command_table_t);



//This function is attributed to Kenneth Alcineus' Assignment 6 Submission in Principles of Embedded Systems ECEN5813.
//the following process_command function is attributed to PES 21 Command Processing lecture slides
void process_command(char *input)
{
	char *p = input;
	char *end;
	// find end of string
	for (end = input; *end != '\0'; end++);
	// Tokenize input in place
	char argv[10][MAX_STR_LEN];
	int argc = 0;
	int whitespace_flag = 0;
	int argstrlen = 0;
	memset(argv, 0, sizeof(argv));
	for (p = input; p < end; p++)
	{
		if (argc == 10 || *p == '\r')
		{
			break;
		}

		if (*p == ' ' || *p == '\t' || *p == '\v')
		{
			if (whitespace_flag)
			{
				continue;
			}
			whitespace_flag = 1;
			argstrlen = 0;
			argc++;
			*p = '\0';
			continue;
		}

		if (whitespace_flag)
		{
			*--p = '\0';
			whitespace_flag = *p++; //done this way to avoid unused value error upon compiling
			whitespace_flag = 0;
		}

		argv[argc][argstrlen] = *p;
		argstrlen++;
	}

	argc++;

	int command_flag = 0;

	for (int i = 0; i < num_commands; i++)
	{
		if (strcasecmp(argv[0], commands[i].name) == 0)
		{
			command_flag = 1;
			commands[i].handler(argc, argv);
			break;
		}
	}

	if (!command_flag)
	{
		unknown_command(argc, argv);
	}
}
