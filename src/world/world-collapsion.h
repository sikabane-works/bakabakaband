#pragma once
#include <cstdint>

class WorldCollapsion {
public:
    WorldCollapsion();
    int32_t collapse_degree{}; /*!< 時空崩壊度 */
};

extern WorldCollapsion world_collapsion;
extern WorldCollapsion *wc_ptr;
