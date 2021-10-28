#include "world/world-collapsion.h"

WorldCollapsion world_collapsion;
WorldCollapsion *wc_ptr = &world_collapsion;

WorldCollapsion::WorldCollapsion()
{
    this->collapse_degree = 0;
}
