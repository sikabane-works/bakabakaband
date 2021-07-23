#pragma once

enum target_type : unsigned int;
class player_type;
bool target_set(player_type *creature_ptr, target_type mode);
