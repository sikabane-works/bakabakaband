#pragma once

#include "system/angband.h"
#include <string>
#include <vector>
#include <map>

typedef struct vault_type {
    int16_t idx;

    std::string name; /* Name (offset) */
    std::string text; /* Text (offset) */

    byte typ{}; /* Vault type */
    PROB rat{}; /* Vault rating (unused) */
    POSITION hgt{}; /* Vault height */
    POSITION wid{}; /* Vault width */
    std::map<char, FEAT_IDX> feature_list;
    std::map<char, FEAT_IDX> feature_ap_list;

    int min_depth = 0;
    int max_depth = 999;
    int rarity = 1;
} vault_type;

extern std::vector<vault_type> v_info;

struct dun_data_type;
class PlayerType;
bool build_type7(PlayerType *player_ptr, dun_data_type *dd_ptr);
bool build_type8(PlayerType *player_ptr, dun_data_type *dd_ptr);
bool build_type10(PlayerType *player_ptr, dun_data_type *dd_ptr);
bool build_type17(PlayerType *player_ptr, dun_data_type *dd_ptr);
