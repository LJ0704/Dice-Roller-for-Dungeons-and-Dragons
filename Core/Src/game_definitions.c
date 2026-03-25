/**
  ******************************************************************************
  * @file           : game_definitions.c
  * @brief          : Static definitions of entities related to the game, including their memory mappings or lookup tables when relevant
  ******************************************************************************
  * @attention
  *
  * Copyright (c) December 1st, 2025 Kenneth Alcineus.
  * All rights reserved.
  *
  *
  ******************************************************************************
  */

#include "main.h"
#include <stdio.h>
#include <uart.h>
#include "EEPROM.h"
#include "i2c.h"
#include "sys_timer_delay.h"

#define CHARACTER_OFFSET 0 //determines starting address where characters will be written (0x000 address)

#define CHARACTER_SIZE 0x100 //determines size of each character in memory (256 bytes)

#define CHARACTER_LIMIT 6 //determines maximum number of characters (6 blocks)

//characters are memory mapped sector 0 starting at address 0x08000000
typedef struct {
	uint8_t name[64];
	uint8_t species;
	uint8_t primary_class_subclass;
	uint8_t primary_level;
	uint8_t secondary_class_subclass;
	uint8_t secondary_level;
	uint8_t tertiary_class_subclass;
	uint8_t tertiary_level;
	uint8_t feats[8];
	uint8_t proficiency_bonus;
	uint8_t armor_class;
	uint8_t initiative;
	uint8_t current_hit_points;
	uint8_t maximum_hit_points;
	uint8_t temporary_hit_points;
	uint8_t strength_ability_modifier;
	uint8_t strength_saving_throw_modifier;
	uint8_t dexterity_ability_modifier;
	uint8_t dexterity_saving_throw_modifier;
	uint8_t constitution_ability_modifier;
	uint8_t constitution_saving_throw_modifier;
	uint8_t intelligence_ability_modifier;
	uint8_t intelligence_saving_throw_modifier;
	uint8_t wisdom_ability_modifier;
	uint8_t wisdom_saving_throw_modifier;
	uint8_t charisma_ability_modifier;
	uint8_t charisma_saving_throw_modifier;
	uint8_t armor_proficiencies[4];
	uint8_t weapon_proficiencies[38];
	uint8_t acrobatics_skill_modifier;
	uint8_t animal_handling_skill_modifier;
	uint8_t arcana_skill_modifier;
	uint8_t athletics_skill_modifier;
	uint8_t deception_skill_modifier;
	uint8_t history_skill_modifier;
	uint8_t insight_skill_modifier;
	uint8_t intimidation_skill_modifier;
	uint8_t investigation_skill_modifier;
	uint8_t medicine_skill_modifier;
	uint8_t nature_skill_modifier;
	uint8_t perception_skill_modifier;
	uint8_t performance_skill_modifier;
	uint8_t persuasion_skill_modifier;
	uint8_t religion_skill_modifier;
	uint8_t sleight_of_hand_skill_modifier;
	uint8_t stealth_skill_modifier;
	uint8_t survival_skill_modifier;
	uint8_t acid_resistance;
	uint8_t bludgeoning_resistance;
	uint8_t cold_resistance;
	uint8_t fire_resistance;
	uint8_t force_resistance;
	uint8_t lightning_resistance;
	uint8_t necrotic_resistance;
	uint8_t piercing_resistance;
	uint8_t poison_resistance;
	uint8_t psychic_resistance;
	uint8_t radiant_resistance;
	uint8_t slashing_resistance;
	uint8_t thunder_resistance;
	uint8_t status_conditions[14];
	uint8_t equipped_weapons[8];
	uint8_t equipped_spells[16];
	uint8_t experience_points[3];
} character_t;

//items are memory mapped sector 1 starting at address 0x08004000
typedef struct {
	uint8_t name[64];
	uint8_t item_type;
	uint8_t item_id;
	uint8_t item_properties[8];
} item_t;

//reads character from EEPROM
character_t get_character(int character_num)
{
	character_t c = {};

	uint16_t mem_addr = (CHARACTER_SIZE * (character_num - 1)) + CHARACTER_OFFSET;

	if (mem_addr < CHARACTER_OFFSET || mem_addr >= (CHARACTER_OFFSET + (CHARACTER_SIZE * CHARACTER_LIMIT)))
	{
		c.name[0] = 0;
		return c;
	}

	EEPROM_ReadByte(mem_addr, &c.name[0]);

	if (c.name[0] == 0)
	{
		c.name[0] = 0;
		return c;
	}

	for (int i = 0; i < 64; i++)
	{
		EEPROM_ReadByte(mem_addr++, &c.name[i]);
	}

	EEPROM_ReadByte(mem_addr++, &c.species);
	EEPROM_ReadByte(mem_addr++, &c.primary_class_subclass);
	EEPROM_ReadByte(mem_addr++, &c.primary_level);
	EEPROM_ReadByte(mem_addr++, &c.secondary_class_subclass);
	EEPROM_ReadByte(mem_addr++, &c.secondary_level);
	EEPROM_ReadByte(mem_addr++, &c.tertiary_class_subclass);
	EEPROM_ReadByte(mem_addr++, &c.tertiary_level);

	for (int i = 0; i < 8; i++)
	{
		EEPROM_ReadByte(mem_addr++, &c.feats[i]);
	}

	EEPROM_ReadByte(mem_addr++, &c.proficiency_bonus);
	EEPROM_ReadByte(mem_addr++, &c.armor_class);
	EEPROM_ReadByte(mem_addr++, &c.initiative);
	EEPROM_ReadByte(mem_addr++, &c.current_hit_points);
	EEPROM_ReadByte(mem_addr++, &c.maximum_hit_points);
	EEPROM_ReadByte(mem_addr++, &c.temporary_hit_points);
	EEPROM_ReadByte(mem_addr++, &c.strength_ability_modifier);
	EEPROM_ReadByte(mem_addr++, &c.strength_saving_throw_modifier);
	EEPROM_ReadByte(mem_addr++, &c.dexterity_ability_modifier);
	EEPROM_ReadByte(mem_addr++, &c.dexterity_saving_throw_modifier);
	EEPROM_ReadByte(mem_addr++, &c.constitution_ability_modifier);
	EEPROM_ReadByte(mem_addr++, &c.constitution_saving_throw_modifier);
	EEPROM_ReadByte(mem_addr++, &c.intelligence_ability_modifier);
	EEPROM_ReadByte(mem_addr++, &c.intelligence_saving_throw_modifier);
	EEPROM_ReadByte(mem_addr++, &c.wisdom_ability_modifier);
	EEPROM_ReadByte(mem_addr++, &c.wisdom_saving_throw_modifier);
	EEPROM_ReadByte(mem_addr++, &c.charisma_ability_modifier);
	EEPROM_ReadByte(mem_addr++, &c.charisma_saving_throw_modifier);

	for (int i = 0; i < 4; i++)
	{
		EEPROM_ReadByte(mem_addr++, &c.armor_proficiencies[i]);
	}

	for (int i = 0; i < 38; i++)
	{
		EEPROM_ReadByte(mem_addr++, &c.weapon_proficiencies[i]);
	}

	EEPROM_ReadByte(mem_addr++, &c.acrobatics_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.animal_handling_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.arcana_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.athletics_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.deception_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.history_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.insight_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.intimidation_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.investigation_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.medicine_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.nature_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.perception_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.performance_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.persuasion_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.religion_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.sleight_of_hand_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.stealth_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.survival_skill_modifier);
	EEPROM_ReadByte(mem_addr++, &c.acid_resistance);
	EEPROM_ReadByte(mem_addr++, &c.bludgeoning_resistance);
	EEPROM_ReadByte(mem_addr++, &c.cold_resistance);
	EEPROM_ReadByte(mem_addr++, &c.fire_resistance);
	EEPROM_ReadByte(mem_addr++, &c.force_resistance);
	EEPROM_ReadByte(mem_addr++, &c.lightning_resistance);
	EEPROM_ReadByte(mem_addr++, &c.necrotic_resistance);
	EEPROM_ReadByte(mem_addr++, &c.piercing_resistance);
	EEPROM_ReadByte(mem_addr++, &c.poison_resistance);
	EEPROM_ReadByte(mem_addr++, &c.psychic_resistance);
	EEPROM_ReadByte(mem_addr++, &c.radiant_resistance);
	EEPROM_ReadByte(mem_addr++, &c.slashing_resistance);
	EEPROM_ReadByte(mem_addr++, &c.thunder_resistance);

	for (int i = 0; i < 14; i++)
	{
		EEPROM_ReadByte(mem_addr++, &c.status_conditions[i]);
	}

	for (int i = 0; i < 8; i++)
	{
		EEPROM_ReadByte(mem_addr++, &c.equipped_weapons[i]);
	}

	for (int i = 0; i < 16; i++)
	{
		EEPROM_ReadByte(mem_addr++, &c.equipped_spells[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		EEPROM_ReadByte(mem_addr++, &c.experience_points[i]);
	}

	return c;
}

//writes character to EEPROM
int set_character(int character_num, uint8_t *data_array)
{
	uint16_t mem_addr = (CHARACTER_SIZE * (character_num - 1)) + CHARACTER_OFFSET;

	if (mem_addr < CHARACTER_OFFSET || mem_addr >= (CHARACTER_OFFSET + (CHARACTER_SIZE * CHARACTER_LIMIT)))
	{
		return 0;
	}

	for (int i = 0; i < CHARACTER_SIZE; i++)
	{
		EEPROM_WriteByte(mem_addr++, *data_array++);
		delay_ms(50);
	}


	return 1;
}

//clear all characters on EEPROM
int erase_characters()
{
	for (int i = CHARACTER_OFFSET; i < (CHARACTER_OFFSET + (CHARACTER_SIZE * CHARACTER_LIMIT)); i++)
	{
		EEPROM_WriteByte((uint16_t)i, 0x00);
		delay_ms(50);
	}

	return 1;
}
