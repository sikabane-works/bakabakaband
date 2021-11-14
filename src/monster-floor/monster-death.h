#pragma once

#include "system/angband.h"
#include "monster-floor/monster-death-util.h"
#include "spell/spell-types.h"

class PlayerType;
void monster_death(PlayerType *player_ptr, MONSTER_IDX m_idx, bool drop_item, EffectFlags effect_flags);
void monster_death(PlayerType *player_ptr, MONSTER_IDX m_idx, bool drop_item, EFFECT_ID effect_type);
bool drop_single_artifact(PlayerType *player_ptr, monster_death_type *md_ptr, ARTIFACT_IDX a_idx);
concptr extract_note_dies(MONRACE_IDX r_idx);
