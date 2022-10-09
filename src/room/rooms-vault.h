#pragma once

#include "system/angband.h"
#include <map>
#include <string>
#include <vector>

enum class MonsterRaceId : int16_t;

struct vault_type {
    int16_t idx;

    std::string name; /* Name (offset) */
    std::string text; /* Text (offset) */

    byte typ{}; /* Vault type */
    PROB rat{}; /* Vault rating (unused) */
    POSITION hgt{}; /* Vault height */
    POSITION wid{}; /* Vault width */
    std::map<char, FEAT_IDX> feature_list;
    std::map<char, FEAT_IDX> feature_ap_list;
    std::map<char, MonsterRaceId> place_monster_list;

    int min_depth = 0;
    int max_depth = 999;
    int rarity = 1;
};

extern std::vector<vault_type> vaults_info;

struct dun_data_type;
class PlayerType;
bool build_type10(PlayerType *player_ptr, dun_data_type *dd_ptr);
bool build_fixed_room(PlayerType *player_ptr, dun_data_type *dd_ptr, int typ, bool more_space, int id);
void build_vault(vault_type *v_ptr, PlayerType *player_ptr, POSITION yval, POSITION xval, POSITION xoffset, POSITION yoffset, int transno);
