#pragma once

#include "system/angband.h"

/* Minimum & maximum town size */
constexpr int MIN_TOWN_WID = 15;
constexpr int MIN_TOWN_HGT = 15;
constexpr int MAX_TOWN_WID = 32;
constexpr int MAX_TOWN_HGT = 32;

/* Struct for build underground buildings */
struct ugbldg_type {
    POSITION y0, x0; /* North-west corner (relative) */
    POSITION y1, x1; /* South-east corner (relative) */
};

struct dun_data_type;
class PlayerType;
bool build_type16(PlayerType *player_ptr, dun_data_type *dd_ptr);
