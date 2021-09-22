﻿#pragma once

#include "system/angband.h"
#include <string>
#include <vector>
#include <map>

typedef struct vault_type {
    std::string name; /* Name (offset) */
    std::string text; /* Text (offset) */

    byte typ{}; /* Vault type */
    PROB rat{}; /* Vault rating (unused) */
    POSITION hgt{}; /* Vault height */
    POSITION wid{}; /* Vault width */
    std::map<char, FEAT_IDX> feature_list;
} vault_type;

extern std::vector<vault_type> v_info;
extern int16_t max_v_idx;

typedef struct dun_data_type dun_data_type;
class player_type;
bool build_type7(player_type *player_ptr, dun_data_type *dd_ptr);
bool build_type8(player_type *player_ptr, dun_data_type *dd_ptr);
bool build_type10(player_type *player_ptr, dun_data_type *dd_ptr);
bool build_type17(player_type *player_ptr, dun_data_type *dd_ptr);
