#pragma once
#include <cstdint>

struct world_type;

class WorldCollapsion {
public:
    WorldCollapsion();
    void plus_timed_world_collapsion(world_type *w_ptr, int multi);
    bool is_blown_away();

    int32_t collapse_degree{}; /*!< 時空崩壊度 */
};

extern WorldCollapsion world_collapsion;
extern WorldCollapsion *wc_ptr;
