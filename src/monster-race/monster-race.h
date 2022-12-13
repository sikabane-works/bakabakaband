#pragma once

#include "system/angband.h"

#include <map>

enum class MonsterRaceId : int16_t;
struct monster_race;
extern std::map<MonsterRaceId, monster_race> r_info;
int calc_monrace_power(monster_race *r_ptr);
int calc_monrace_eval(monster_race *r_ptr);
bool is_valid_monster_race(MonsterRaceId r_idx);

class MonsterRace {
public:
    MonsterRace(MonsterRaceId r_idx);

    static MonsterRaceId pick_one_at_random();

    bool is_valid() const;

private:
    const MonsterRaceId r_idx;
};
