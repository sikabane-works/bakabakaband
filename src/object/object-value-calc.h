#pragma once

#include "system/angband.h"

typedef struct object_type object_type;
class player_type;
s32b flag_cost(player_type *player_ptr, object_type *o_ptr, int plusses);
