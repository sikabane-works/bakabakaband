﻿#pragma once

#include "system/angband.h"
#include "spell/spells-util.h"

class player_type;
concptr do_death_spell(player_type *caster_ptr, SPELL_IDX spell, spell_type mode);
