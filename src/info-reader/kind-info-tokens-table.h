#pragma once

#include "system/angband.h"
#include "object-enchant/trg-types.h"
#include <string>
#include <unordered_map>

#define NUM_K_FLAGS 149
#define NUM_K_GENERATION_FLAGS 32

extern concptr k_info_flags[NUM_K_FLAGS];
extern std::unordered_map<std::string_view, TRG> k_info_gen_flags;
