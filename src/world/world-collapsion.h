#pragma once
#include "system/h-type.h"
#include <cstdint>

struct world_type;
class PlayerType;

class WorldCollapsion {
public:
    WorldCollapsion();
    void plus_timed_world_collapsion(world_type *w_ptr, PlayerType *player_ptr, int multi);
    void plus_collapsion(int value);
    void plus_perm_collapsion(int permyriad);
    DEPTH plus_monster_level();
    bool is_blown_away();

    int32_t collapse_degree{}; /*!< 時空崩壊度 */
};

extern WorldCollapsion world_collapsion;
extern WorldCollapsion *wc_ptr;
