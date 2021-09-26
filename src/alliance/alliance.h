#pragma once
#include "system/angband.h"
#include "util/flag-group.h"
#include <string>
#include <vector>

typedef int ALLIANCE_ID;
class player_type;

enum class alliance_types {
    AMBER = 0,          //!< アンバー
    COURT_OF_CHAOS = 1, //!< 混沌の宮廷
    VALINOR = 2,        //!< ヴァリノール
    UTUMNO = 3,         //!< ウトゥムノ
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
    std::string name; //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    virtual int calcImplessionPoint(player_type *creature_ptr) const = 0;
    virtual ~Alliance() = default;
};

class AllianceAmber : public Alliance {
public:
    std::string name = _("アンバー", "Amber"); //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceAmber() = default;
};

class AllianceCourtOfChaos : public Alliance {
public:
    std::string name = _("混沌の宮廷", "Court of Chaos"); //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceCourtOfChaos() = default;
};

class AllianceValinor : public Alliance {
public:
    std::string name = _("ヴァリノール", "Valinor"); //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceValinor() = default;
};

class AllianceUtumno : public Alliance {
public:
    std::string name = _("ウトゥムノ", "Utumno"); //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceUtumno() = default;
};

const std::vector<Alliance> alliance_list;
