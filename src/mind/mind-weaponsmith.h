﻿#pragma once

typedef struct object_type object_type;
class player_type;
bool item_tester_hook_melee_ammo(player_type *player_ptr, object_type *o_ptr);
bool object_is_smith(player_type *player_ptr, object_type *o_ptr);
