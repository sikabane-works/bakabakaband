﻿#pragma once

#include "system/angband.h"

class PlayerType;
bool hypodynamic_bolt(PlayerType *player_ptr, DIRECTION dir, int dam);
bool death_ray(PlayerType *player_ptr, DIRECTION dir, PLAYER_LEVEL plev);
