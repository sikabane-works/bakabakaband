﻿#pragma once

#include "system/angband.h"

typedef struct effect_monster_type effect_monster_type;
class player_type;
process_result effect_monster_drain_mana(player_type *caster_ptr, effect_monster_type *em_ptr);
process_result effect_monster_mind_blast(player_type *caster_ptr, effect_monster_type *em_ptr);
process_result effect_monster_brain_smash(player_type *caster_ptr, effect_monster_type *em_ptr);
