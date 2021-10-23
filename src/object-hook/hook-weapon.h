#pragma once

class player_type;
struct object_type;
bool object_is_orthodox_melee_weapons(const object_type *o_ptr);
bool object_is_broken_weapon(const object_type *o_ptr);
bool object_is_boomerang(const object_type *o_ptr);
bool object_is_mochikae(const object_type *o_ptr);
bool object_is_favorite(player_type *player_ptr, const object_type *o_ptr);
