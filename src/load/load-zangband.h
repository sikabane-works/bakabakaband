#pragma once

#include "dungeon/quest.h"
#include "system/angband.h"

class PlayerType;
struct QuestType;
void set_zangband_special_attack(PlayerType *player_ptr);
void set_zangband_special_defense(PlayerType *player_ptr);
void set_zangband_action(PlayerType *player_ptr);
void set_zangband_learnt_spells(PlayerType *player_ptr);
void set_zangband_pet(PlayerType *player_ptr);
