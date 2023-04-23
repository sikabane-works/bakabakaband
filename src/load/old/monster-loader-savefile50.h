#pragma once

#include "load/monster/monster-loader-base.h"

struct monster_type;
class MonsterLoader50 : public MonsterLoaderBase {
public:
    MonsterLoader50();
    void rd_monster(monster_type *m_ptr) override;

private:
    monster_type *m_ptr = nullptr;
};
