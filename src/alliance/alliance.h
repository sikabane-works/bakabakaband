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
    int id; //!< ID
    std::string tag; //!< タグ
    std::string name; //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    virtual int calcImplessionPoint(player_type *creature_ptr) const = 0;
    virtual ~Alliance() = default;
};

class AllianceAmber : public Alliance {
public:
    const int id = 1; //!< ID
    const std::string tag = "AMBER"; //!< タグ
    const std::string name = _("アンバー", "Amber"); //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceAmber() = default;
};

class AllianceCourtOfChaos : public Alliance {
public:
    const int id = 2; //!< ID
    const std::string tag = "COCHAOS"; //!< タグ
    const std::string name = _("混沌の宮廷", "Court of Chaos"); //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceCourtOfChaos() = default;
};

class AllianceValinor : public Alliance {
public:
    int id = 3; //!< ID
    const std::string tag = "VALINOR"; //!< タグ
    const std::string name = _("ヴァリノール", "Valinor"); //!< 陣営名
    const Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceValinor() = default;
};

class AllianceUtumno : public Alliance {
public:
    int id = 4; //!< ID
    std::string tag = "UTUMNO"; //!< タグ
    std::string name = _("ウトゥムノ", "Utumno"); //!< 陣営名
    Alliance *suzerain = nullptr; //!< 宗主アライアンス
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(player_type *creature_ptr) const override;
    virtual ~AllianceUtumno() = default;
};

extern const std::vector<std::shared_ptr<Alliance>> alliance_list;
