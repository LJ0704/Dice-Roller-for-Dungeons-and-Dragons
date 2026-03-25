/**
  ******************************************************************************
  * @file           : game_definitions.h
  * @brief          : Static definitions of entities related to the game, including their memory mappings or lookup tables when relevant
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Kenneth Alcineus.
  * All rights reserved.
  *
  *
  ******************************************************************************
  */

#ifndef INC_GAME_DEFINITIONS_H_
#define INC_GAME_DEFINITIONS_H_

#include <stdint.h>

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

//weapon damage is in dice roll format: 0x14 represents 1d4, 0x1C represents 1d12, 0x26 represents 2d6, 0x01 represents flat damage of 1
static const uint8_t weapon_damage_lookup[0x27] =
{
		0x00, 0x14, 0x14, 0x18, 0x16, 0x16, 0x14, 0x16,
		0x16, 0x14, 0x16, 0x14, 0x18, 0x16, 0x14, 0x18,
		0x18, 0x1A, 0x1C, 0x26, 0x1A, 0x1A, 0x18, 0x26,
		0x18, 0x1A, 0x18, 0x16, 0x16, 0x18, 0x18, 0x18,
		0x14, 0x01, 0x16, 0x1A, 0x18, 0x1C, 0x1A
};

//possible damage type for weapons are bludgeoning (0x02), piercing (0x08), or slashing (0x0C)
static const uint8_t weapon_damage_type_lookup[0x27] =
{
		0x00, 0x02, 0x08, 0x02, 0x0C, 0x08, 0x02, 0x02,
		0x02, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x02, 0x0C,
		0x02, 0x0C, 0x0C, 0x0C, 0x0C, 0x08, 0x0C, 0x02,
		0x08, 0x08, 0x08, 0x0C, 0x08, 0x08, 0x02, 0x08,
		0x0C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08
};

character_t get_character(int character_num);
int set_character(int character_num, uint8_t *data_array);
int erase_characters();
int set_dummy_character();

#endif /* INC_GAME_DEFINITIONS_H_ */
