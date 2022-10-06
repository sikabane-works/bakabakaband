﻿#pragma once

#include "system/angband.h"
#include "alliance/alliance.h"

#include <vector>

class store_type;

/*
 * A structure describing a town with
 * stores and buildings
 */
struct town_type {
    GAME_TEXT name[32];
    uint32_t seed; /* Seed for RNG */
    std::vector<store_type> store; /* The stores [MAX_STORES] */
    byte numstores;
    AllianceType alliance = AllianceType::NONE;
};

extern int16_t max_towns;
extern std::vector<town_type> town_info;
