﻿#include "world/world-collapsion.h"
#include "world/world.h"
#include "object-enchant/tr-flags.h"
#include "player/player-status-flags.h"
#include "system/player-type-definition.h"
#include "market/arena-info-table.h"

WorldCollapsion world_collapsion;
WorldCollapsion *wc_ptr = &world_collapsion;

WorldCollapsion::WorldCollapsion()
{
    this->collapse_degree = 0;
}

bool WorldCollapsion::is_blown_away()
{
    return this->collapse_degree >= 100000000;
}


/*!
 * @brief 時空崩壊度自然進行度計算
 */
void WorldCollapsion::plus_timed_world_collapsion(world_type *w_ptr, player_type *player_ptr, int multi)
{
    if (w_ptr->total_winner && player_ptr->arena_number > MAX_ARENA_MONS + 2) return;
    if (get_player_flags(player_ptr, TR_WORLD_END)) multi *= 2;
    if (w_ptr->total_winner) multi /= 3;
    this->collapse_degree += (std::min(1, mysqrt(w_ptr->game_turn / 2000)) * multi);
}

/*!
 * @brief 時空崩壊度単純増減
 */
void WorldCollapsion::plus_collapsion(int value)
{
    this->collapse_degree += value;
}
