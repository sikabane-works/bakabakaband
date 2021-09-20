#pragma once
/*
 * Windowsのコードからは呼ばれない。よってVSからは見えない
 */

#include "system/angband.h"

class player_type;
void exit_game_panic(player_type *creature_ptr);
