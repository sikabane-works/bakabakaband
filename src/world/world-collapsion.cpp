#include "world/world-collapsion.h"
#include "world/world.h"

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
void WorldCollapsion::plus_timed_world_collapsion(world_type *w_ptr, int multi)
{
    this->collapse_degree += (std::min(1, mysqrt(w_ptr->game_turn / 2000)) * multi);
}
