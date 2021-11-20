﻿#include "alliance/alliance.h"
#include "system/player-type-definition.h"
#include "monster-race/monster-race.h"
#include "system/monster-race-definition.h"
#include "util/bit-flags-calculator.h"
#include "monster-race/race-flags1.h"

const std::map<AllianceType, std::shared_ptr<Alliance>> alliance_list = {
    { AllianceType::NONE, std::make_unique<AllianceNone>(AllianceType::NONE, "NONE", _("無所属", "None"), 0) },
    { AllianceType::AMBER, std::make_unique<AllianceAmber>(AllianceType::AMBER, "AMBER", _("アンバー", "Amber"), 350000000L) },
    { AllianceType::COCHAOS, std::make_unique<AllianceCourtOfChaos>(AllianceType::COCHAOS, "COCHAOS", _("混沌の宮廷", "Court of Chaos"), 200000000L) },
    { AllianceType::VALINOR, std::make_unique<AllianceValinor>(AllianceType::VALINOR, "VARINOR", _("ヴァリノール", "Valinor"), 4000000L) },
    { AllianceType::UTUMNO, std::make_unique<AllianceCourtOfChaos>(AllianceType::UTUMNO, "UTUMNO", _("ウトゥムノ", "Utumno"), 3000000L) },
    { AllianceType::JURAL, std::make_unique<AllianceJural>(AllianceType::JURAL, "JURAL", _("ジュラル星人", "Jural"), 5500L) },
    { AllianceType::CHINCHINTEI, std::make_unique<AllianceChinChinTei>(AllianceType::CHINCHINTEI, "CHINCHINTEI", _("ちんちん亭", "Chin-Chin-Tei"), 191919L) },
    { AllianceType::ODIO, std::make_unique<AllianceOdio>(AllianceType::ODIO, "ODIO", _("オディオ", "Odio"), 300000L) },
    { AllianceType::KENOHGUN, std::make_unique<AllianceKenohgun>(AllianceType::KENOHGUN, "KENOHGUN", _("拳王軍", "Kenohgun"), 100000L) },
    { AllianceType::FANG_FAMILY, std::make_unique<AllianceFangFamily>(AllianceType::FANG_FAMILY, "FANG-FAMILY", _("牙一族", "Fang Family"), 4000L) },
    { AllianceType::KOGAN_RYU, std::make_unique<AllianceKoganRyu>(AllianceType::KOGAN_RYU, "KOGAN-RYU", _("虎眼流", "Kogan Ryu"), 10000L) },
    { AllianceType::ELDRAZI, std::make_unique<AllianceEldrazi>(AllianceType::ELDRAZI, "ELDRAZI", _("エルドラージ", "Eldrazi"), 120000000L) },
    { AllianceType::UNGOLIANT, std::make_unique<AllianceUngoliant>(AllianceType::UNGOLIANT, "UNGOLIANT", _("ウンゴリアント一族", "Ungoliant's Family"), 1500000L) },
    { AllianceType::SHITTO_DAN, std::make_unique<AllianceShittoDan>(AllianceType::SHITTO_DAN, "SHITTO-DAN", _("しっと団", "Sitto-Dan"), 1500L) },
    { AllianceType::GE_ORLIC, std::make_unique<AllianceGEOrlic>(AllianceType::GE_ORLIC, "GE-ORLIC", _("オーリック朝銀河帝国", "Galactic Empire of Orlic"), 2000000L) },
    { AllianceType::TURBAN_KIDS, std::make_unique<AllianceTurbanKids>(AllianceType::TURBAN_KIDS, "TURBAN-KIDS", _("ターバンのガキ共", "Turban Kids"), 20000L) }

};

const std::map<std::tuple<AllianceType, AllianceType>, int> each_alliance_implession = {
    { std::make_tuple(AllianceType::VALINOR, AllianceType::UTUMNO), -5000 },
    { std::make_tuple(AllianceType::UTUMNO, AllianceType::VALINOR), -1000 },
};


Alliance::Alliance(AllianceType id, std::string tag, std::string name, int64_t base_power)
    : id(id)
    , tag(tag)
    , name(name)
    , base_power(base_power)
{

}

int64_t Alliance::calcCurrentPower()
{
    int64_t res = this->base_power;
    for (auto r : r_info) {
        if (r.alliance_idx == this->id && any_bits(r.flags1, RF1_UNIQUE)) {
            res += calc_monrace_eval(&r);
        }
    }
    return res;
}

bool Alliance::isAnnihilated()
{
    return false;
}

int AllianceNone::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceAmber::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment;
}

int AllianceCourtOfChaos::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceValinor::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment : creature_ptr->alignment * 3;
}

int AllianceUtumno::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceJural::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceChinChinTei::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceOdio::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceKenohgun::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceFangFamily::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceKoganRyu::calcImplessionPoint(PlayerType *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceEldrazi::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceUngoliant::calcImplessionPoint(PlayerType* creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceShittoDan::calcImplessionPoint([[maybe_unused]] PlayerType* creature_ptr) const
{
    return 0;
}

int AllianceGEOrlic::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceTurbanKids::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}
