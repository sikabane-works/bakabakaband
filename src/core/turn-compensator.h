﻿#pragma once

#include "system/angband.h"

class player_type;
s32b turn_real(player_type *player_ptr, s32b hoge);
void prevent_turn_overflow(player_type* player_ptr);
