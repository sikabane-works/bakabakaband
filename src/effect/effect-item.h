﻿#pragma once

#include "system/angband.h"

class PlayerType;
bool affect_item(PlayerType *player_ptr, MONSTER_IDX who, POSITION r, POSITION y, POSITION x, HIT_POINT dam, AttributeType typ);
