#pragma once
#include "system/h-type.h"

typedef struct player_type player_type;
typedef struct floor_type floor_type;
typedef struct saved_floor_type saved_floor_type;

void locate_connected_stairs(player_type *creature_ptr, floor_type *floor_ptr, saved_floor_type *sf_ptr, BIT_FLAGS floor_mode);
void floor_warp(player_type *creature_ptr, DUNGEON_IDX dun_idx, DEPTH depth);
void leave_floor(player_type *creature_ptr);
void preserve_pet(player_type *master_ptr);
