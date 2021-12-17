#pragma once
#include "system/angband.h"
#include "util/flag-group.h"
#include <string>
#include <map>

typedef int ALLIANCE_ID;
class PlayerType;

enum class AllianceType : int {
    NONE = 0, //!< 無所属 
    AMBER = 1, //!< アンバー
    COCHAOS = 2, //!< 混沌の宮廷
    VALINOR = 3,        //!< ヴァリノール
    UTUMNO = 4,         //!< ウトゥムノ
    JURAL = 5,          //!< ジュラル星人
    CHINCHINTEI = 6,    //!< ちんちん亭
    ODIO = 7,           //!< オディオ
    KENOHGUN = 8,       //!< 拳王軍
    FANG_FAMILY = 9,    //!< 牙一族
    KOGAN_RYU = 10,     //!< 虎眼流
    ELDRAZI = 11,     //!< エルドラージ
    UNGOLIANT = 12,     //!< ウンゴリアント一族
    SHITTO_DAN = 13,     //!< しっと団
    GE_ORLIC = 14,     //!< オーリック朝銀河帝国（超人ロック）
    TURBAN_KIDS = 15, //!< ターバンのガキ共
    NAKED_KNIGHTS = 16, //!< 全裸騎士団
    NUMENOR = 17, //!< ヌメノール王国
    GO = 18, //!< GO教
    THE_SHIRE = 19, //!< ホビット庄
    HAKUSIN_KARATE = 20, //!< 迫真空手部
    DOKACHANS = 21, //!< 岡山中高年男児糞尿愛好会
    KETHOLDETH = 22, //!< ケツホルデス
    MELDOR = 23, //!< メルドール
    ANGARTHA = 24, //!< アンガルタ
    GETTER = 25, //!< ゲッター
    PURE_MIRRODIN = 26, //!< 清純なるミラディン
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
    AllianceType id; //!< ID
    std::string tag; //!< タグ
    std::string name; //!< 陣営名
    int64_t base_power; //!< 基本勢力指数
    Alliance(AllianceType id, std::string tag, std::string name, int64_t base_power);
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int64_t calcCurrentPower();
    bool isAnnihilated();
    virtual int calcImplessionPoint(PlayerType *creature_ptr) const = 0;
    virtual ~Alliance() = default;
};

class AllianceNone : public Alliance {
public:
    using Alliance::Alliance;
    AllianceNone() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const override;
    virtual ~AllianceNone() = default;
};

class AllianceAmber : public Alliance {
public:
    using Alliance::Alliance;
    AllianceAmber() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceAmber() = default;
};

class AllianceCourtOfChaos : public Alliance {
public:
    using Alliance::Alliance;
    AllianceCourtOfChaos() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceCourtOfChaos() = default;
};

class AllianceValinor : public Alliance {
public:
    using Alliance::Alliance;
    AllianceValinor() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceValinor() = default;
};

class AllianceUtumno : public Alliance {
public:
    using Alliance::Alliance;
    AllianceUtumno() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceUtumno() = default;
};

class AllianceJural : public Alliance {
public:
    using Alliance::Alliance;
    AllianceJural() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceJural() = default;
};

class AllianceChinChinTei : public Alliance {
public:
    using Alliance::Alliance;
    AllianceChinChinTei() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceChinChinTei() = default;
};

class AllianceOdio : public Alliance {
public:
    using Alliance::Alliance;
    AllianceOdio() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceOdio() = default;
};

class AllianceKenohgun : public Alliance {
public:
    using Alliance::Alliance;
    AllianceKenohgun() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceKenohgun() = default;
};

class AllianceFangFamily : public Alliance {
public:
    using Alliance::Alliance;
    AllianceFangFamily() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceFangFamily() = default;
};

class AllianceKoganRyu : public Alliance {
public:
    using Alliance::Alliance;
    AllianceKoganRyu() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceKoganRyu() = default;
};

class AllianceEldrazi : public Alliance {
public:
    using Alliance::Alliance;
    AllianceEldrazi() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceEldrazi() = default;
};

class AllianceUngoliant : public Alliance {
public:
    using Alliance::Alliance;
    AllianceUngoliant() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceUngoliant() = default;
};

class AllianceShittoDan: public Alliance {
public:
    using Alliance::Alliance;
    AllianceShittoDan() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceShittoDan() = default;
};

class AllianceGEOrlic : public Alliance {
public:
    using Alliance::Alliance;
    AllianceGEOrlic() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceGEOrlic() = default;
};

class AllianceTurbanKids : public Alliance {
public:
    using Alliance::Alliance;
    AllianceTurbanKids() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceTurbanKids() = default;
};

class AllianceNakedKnights : public Alliance {
public:
    using Alliance::Alliance;
    AllianceNakedKnights() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceNakedKnights() = default;
};

class AllianceNumenor : public Alliance {
public:
    using Alliance::Alliance;
    AllianceNumenor() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceNumenor() = default;
};

class AllianceGO : public Alliance {
public:
    using Alliance::Alliance;
    AllianceGO() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceGO() = default;
};

class AllianceTheShire : public Alliance {
public:
    using Alliance::Alliance;
    AllianceTheShire() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceTheShire() = default;
};

class AllianceHakushinKarate : public Alliance {
public:
    using Alliance::Alliance;
    AllianceHakushinKarate() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceHakushinKarate() = default;
};

class AllianceDokachans : public Alliance {
public:
    using Alliance::Alliance;
    AllianceDokachans() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceDokachans() = default;
};

class AllianceKetholdeth : public Alliance {
public:
    using Alliance::Alliance;
    AllianceKetholdeth() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceKetholdeth() = default;
};

class AllianceMeldor : public Alliance {
public:
    using Alliance::Alliance;
    AllianceMeldor() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceMeldor() = default;
};

class AllianceAngartha : public Alliance {
public:
    using Alliance::Alliance;
    AllianceAngartha() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType* creature_ptr) const override;
    virtual ~AllianceAngartha() = default;
};

class AllianceGetter : public Alliance {
public:
    using Alliance::Alliance;
    AllianceGetter() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AllianceGetter() = default;
};

class AlliancePureMirrodin : public Alliance {
public:
    using Alliance::Alliance;
    AlliancePureMirrodin() = delete;
    EnumClassFlagGroup<alliance_flags> alliFlags; //!< 陣営特性フラグ
    int calcImplessionPoint(PlayerType *creature_ptr) const override;
    virtual ~AlliancePureMirrodin() = default;
};


extern const std::map<AllianceType, std::shared_ptr<Alliance>> alliance_list;
extern const std::map<std::tuple<AllianceType, AllianceType>, int> each_alliance_implession;
