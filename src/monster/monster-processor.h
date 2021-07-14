#pragma once

#include "system/angband.h"

class player_type;
void process_monsters(player_type *target_ptr);
void process_monster(player_type *target_ptr, MONSTER_IDX m_idx);
