#pragma once
#include "system/h-type.h"

typedef struct player_type player_type;
void floor_warp(player_type *creature_ptr, DUNGEON_IDX dun_idx, DEPTH depth);
void leave_floor(player_type *creature_ptr);
