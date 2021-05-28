#pragma once
#include "system/angband.h"
#include "util/flag-group.h"
#include <string>
#include <vector>

typedef int ALLIANCE_ID;

enum alliance_flags
{
    ALLF_ORDER, //!< 秩序の陣営
    ALLF_CHAOS,  //!< 混沌の陣営
    MAX,
};

class alliance {
public:
    std::string name; //!< 陣営名
    // = FlagGroup<FlagType, FlagType::MAX>;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
};

std::vector<alliance> alliance_list;
