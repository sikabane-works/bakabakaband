﻿#pragma once

#include "player-status/player-basic-statistics.h"

class PlayerType;
class PlayerConstitution : public PlayerBasicStatistics {
public:
    PlayerConstitution(PlayerType *player_ptr);

protected:
    void set_locals() override;
    int16_t race_bonus() override;
    int16_t time_effect_value() override;
    int16_t stance_value() override;
    int16_t mutation_value() override;
};
