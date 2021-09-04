#pragma once

#include <stdint.h>

enum target_type : uint32_t;
class player_type;
bool target_set(player_type *creature_ptr, target_type mode);
