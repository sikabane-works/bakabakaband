#pragma once
#include "system/angband.h"
#include "spell/spell-types.h"

typedef struct monster_death_type monster_death_type;
class PlayerType;
void switch_special_death(PlayerType *player_ptr, monster_death_type *md_ptr, EffectFlags effect_flags);
