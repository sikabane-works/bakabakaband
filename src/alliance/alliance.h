#pragma once
#include "system/angband.h"
#include "util/flag-group.h"
#include <string>
#include <vector>

typedef int ALLIANCE_ID;
class player_type;

enum class alliance_types {
    NONE = 0, //!< 無所属 
    AMBER = 1, //!< アンバー
    COURT_OF_CHAOS = 2, //!< 混沌の宮廷
    VALINOR = 3,        //!< ヴァリノール
    UTUMNO = 4,         //!< ウトゥムノ
    MAX,
};

enum alliance_flags
{
    ALLF_ORDER, //!< 秩序の陣営
    ALLF_CHAOS,  //!< 混沌の陣営
    MAX,
};


class Alliance {
public:
    int id; //!< ID
    std::string tag; //!< タグ
    std::string name; //!< 陣営名
    Alliance(int id, std::string tag, std::string name);
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    virtual int calcImplessionPoint(player_type *creature_ptr) const = 0;
    virtual ~Alliance() = default;
};

class AllianceNone : public Alliance {
public:
    using Alliance::Alliance;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint([[maybe_unused]] player_type *creature_ptr) const override;
    virtual ~AllianceNone() = default;
};

class AllianceAmber : public Alliance {
public:
    using Alliance::Alliance;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceAmber() = default;
};

class AllianceCourtOfChaos : public Alliance {
public:
    using Alliance::Alliance;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceCourtOfChaos() = default;
};

class AllianceValinor : public Alliance {
public:
    using Alliance::Alliance;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceValinor() = default;
};

class AllianceUtumno : public Alliance {
public:
    using Alliance::Alliance;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceUtumno() = default;
};

extern const std::vector<std::shared_ptr<Alliance>> alliance_list;
