#pragma once

#include "util/point-2d.h"

/* Minimum & maximum town size */
constexpr int MIN_TOWN_WID = 15;
constexpr int MIN_TOWN_HGT = 15;
constexpr int MAX_TOWN_WID = 32;
constexpr int MAX_TOWN_HGT = 32;

class UndergroundBuilding {
public:
    UndergroundBuilding();
    Pos2D pick_door_direction() const;
    const Pos2D &get_north_west() const;
    const Pos2D &get_south_east() const;
    void set_area(int height, int width, int max_height, int max_width);

private:
    Pos2D north_west; // 地下店舗左上座標.
    Pos2D south_east; // 地下店舗右下座標.
};

struct dun_data_type;
class PlayerType;
bool build_type16(PlayerType *player_ptr, dun_data_type *dd_ptr);
