#pragma once
#include "system/angband.h"
#include "util/flag-group.h"
#include <string>
#include <map>

typedef int ALLIANCE_ID;
class player_type;

enum class alliance_types {
    NONE = 0, //!< 無所属 
    AMBER = 1, //!< アンバー
    COURT_OF_CHAOS = 2, //!< 混沌の宮廷
    VALINOR = 3,        //!< ヴァリノール
    UTUMNO = 4,         //!< ウトゥムノ
    JURAL = 5,         //!< ジュラル星人
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
    int64_t base_power; //!< 基本勢力指数
    Alliance(int id, std::string tag, std::string name, int64_t base_power);
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int64_t calcCurrentPower();
    virtual int calcImplessionPoint(player_type *creature_ptr) const = 0;
    virtual ~Alliance() = default;
};

class AllianceNone : public Alliance {
public:
    using Alliance::Alliance;
    AllianceNone() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint([[maybe_unused]] player_type *creature_ptr) const override;
    virtual ~AllianceNone() = default;
};

class AllianceAmber : public Alliance {
public:
    using Alliance::Alliance;
    AllianceAmber() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceAmber() = default;
};

class AllianceCourtOfChaos : public Alliance {
public:
    using Alliance::Alliance;
    AllianceCourtOfChaos() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceCourtOfChaos() = default;
};

class AllianceValinor : public Alliance {
public:
    using Alliance::Alliance;
    AllianceValinor() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceValinor() = default;
};

class AllianceUtumno : public Alliance {
public:
    using Alliance::Alliance;
    AllianceUtumno() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceUtumno() = default;
};

class AllianceJural : public Alliance {
public:
    using Alliance::Alliance;
    AllianceJural() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceJural() = default;
};

class AllianceChinChinTei : public Alliance {
public:
    using Alliance::Alliance;
    AllianceChinChinTei() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceChinChinTei() = default;
};

extern const std::map<int, std::shared_ptr<Alliance>> alliance_list;
