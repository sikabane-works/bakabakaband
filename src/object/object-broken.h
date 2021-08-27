﻿#pragma once

#include "system/angband.h"
#include "object-enchant/tr-types.h"

typedef struct object_type object_type;
typedef struct player_type player_type;
<<<<<<< HEAD
bool hates_acid(object_type *o_ptr);
bool hates_elec(object_type *o_ptr);
bool hates_fire(object_type *o_ptr);
bool hates_cold(object_type *o_ptr);
int set_acid_destroy(object_type *o_ptr);
int set_elec_destroy(object_type *o_ptr);
int set_fire_destroy(object_type *o_ptr);
int set_cold_destroy(object_type *o_ptr);
=======
>>>>>>> 25e2ef4b0 ([Refactor]set_****_destroy()が同じ動きなのでクラス化)

bool potion_smash_effect(player_type *owner_ptr, MONSTER_IDX who, POSITION y, POSITION x, KIND_OBJECT_IDX k_idx);
PERCENTAGE breakage_chance(player_type *owner_ptr, object_type *o_ptr, bool has_archer_bonus, SPELL_IDX snipe_type);

class ObjectBreaker {
public:
    ObjectBreaker(tr_type ignore_flg);
    ObjectBreaker() = delete;
    virtual ~ObjectBreaker() = default;
    int set_destroy(player_type *owner_ptr, object_type *o_ptr);
    virtual bool hates(object_type *o_ptr) = 0;

private:
    tr_type ignore_flg;
};

class BreakerAcid : public ObjectBreaker {
public:
    BreakerAcid();
    virtual ~BreakerAcid() = default;
    bool hates(object_type *o_ptr);
};

class BreakerElec : public ObjectBreaker {
public:
    BreakerElec();
    virtual ~BreakerElec() = default;
    bool hates(object_type *o_ptr);
};

class BreakerFire : public ObjectBreaker {
public:
    BreakerFire();
    virtual ~BreakerFire() = default;
    bool hates(object_type *o_ptr);
};

class BreakerCold : public ObjectBreaker {
public:
    BreakerCold();
    virtual ~BreakerCold() = default;
    bool hates(object_type *o_ptr);
};
