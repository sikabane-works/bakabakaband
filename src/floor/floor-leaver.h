#pragma once
#include "system/angband.h"

class player_type;
void leave_floor(player_type *creature_ptr);
void floor_warp(player_type *creature_ptr, DUNGEON_IDX dun_idx, DEPTH depth);
