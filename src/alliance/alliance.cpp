﻿#include "alliance/alliance.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags1.h"
#include "system/monster-race-definition.h"
#include "system/player-type-definition.h"
#include "util/bit-flags-calculator.h"
#include "monster-race/race-indice-types.h"

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
    { AllianceType::TURBAN_KIDS, std::make_unique<AllianceTurbanKids>(AllianceType::TURBAN_KIDS, "TURBAN-KIDS", _("ターバンのガキ共", "Turban Kids"), 20000L) },
    { AllianceType::NAKED_KNIGHTS, std::make_unique<AllianceNakedKnights>(AllianceType::NAKED_KNIGHTS, "NAKED-KNIGHTS", _("全裸騎士団", "Naked Nights"), 3000L) },
    { AllianceType::NUMENOR, std::make_unique<AllianceNumenor>(AllianceType::NUMENOR, "NUMENOR", _("ヌメノール王国", "Numenor Kingdom"), 1500000L) },
    { AllianceType::GO, std::make_unique<AllianceGO>(AllianceType::GO, "GO", _("GO教", "GO"), 1800000L) },
    { AllianceType::THE_SHIRE, std::make_unique<AllianceTheShire>(AllianceType::THE_SHIRE, "THE-SHIRE", _("ホビット庄", "The Shire"), 5000L) },
    { AllianceType::HAKUSIN_KARATE, std::make_unique<AllianceTheShire>(AllianceType::HAKUSIN_KARATE, "HAKUSIN-KARATE", _("迫真空手部", "Hakusin Karate"), 5000L) },
    { AllianceType::DOKACHANS, std::make_unique<AllianceDokachans>(AllianceType::DOKACHANS, "DOKACHANS", _("岡山中高年男児糞尿愛好会", "Dokachans"), 69L) },
    { AllianceType::KETHOLDETH, std::make_unique<AllianceKetholdeth>(AllianceType::KETHOLDETH, "KETHOLDETH", _("ケツホルデス家", "Kethholdeth House"), 1919L) },
    { AllianceType::MELDOR, std::make_unique<AllianceMeldor>(AllianceType::MELDOR, "MELDOR", _("メルドール", "Meldor"), 5000000L) },
    { AllianceType::ANGARTHA, std::make_unique<AllianceAngartha>(AllianceType::ANGARTHA, "ANGARTHA", _("アンガルタ", "Angartha"), 900000L) },
    { AllianceType::GETTER, std::make_unique<AllianceGetter>(AllianceType::GETTER, "GETTER", _("ゲッター", "Getter"), 200000000L) },
    { AllianceType::PURE_MIRRODIN, std::make_unique<AlliancePureMirrodin>(AllianceType::PURE_MIRRODIN, "PURE-MIRRODIN", _("清純なるミラディン", "Pure Mirrodin"), 200000L) },
    { AllianceType::KING, std::make_unique<AllianceKING>(AllianceType::KING, "KING", _("KING", "KING"), 150000L) },
    { AllianceType::PHYREXIA, std::make_unique<AlliancePhyrexia>(AllianceType::PHYREXIA, "PHYREXIA", _("ファイレクシア", "Phyrexia"), 2000000L) },
    { AllianceType::AVARIN_LORDS, std::make_unique<AllianceAvarinLords>(AllianceType::AVARIN_LORDS, "AVARIN-LORDS", _("アヴァリ諸侯同盟", "Avarin Lords"), 1000000L) },
    { AllianceType::GOLAN, std::make_unique<AllianceGOLAN>(AllianceType::GOLAN, "GOLAN", _("GOLAN", "GOLAN"), 100000L) },
    { AllianceType::BINJO_BUDDHISM, std::make_unique<AllianceBinzyouBuddhism>(AllianceType::BINJO_BUDDHISM, "BINJO-BUDDHISM", _("便乗仏教", "Binjo Buddhism"), 80000L) },
    { AllianceType::ASHINA_CLAN, std::make_unique<AllianceBinzyouBuddhism>(AllianceType::ASHINA_CLAN, "ASHINA-CLAN", _("葦名一門", "Ashina Clan"), 180000L) },
    { AllianceType::SUREN, std::make_unique<AllianceSuren>(AllianceType::SUREN, "SUREN", _("スレン王国", "Suren Kingdom"), 100000L) },
    { AllianceType::FEANOR_NOLDOR, std::make_unique<AllianceFeanorNoldor>(AllianceType::FEANOR_NOLDOR, "FEANOR-NOLDOR", _("フェアノール統一ノルドール", "Feanor Noldor"), 3500000L) },
    { AllianceType::GAICHI, std::make_unique <AllianceGaichi>(AllianceType::GAICHI, "GAICHI", _("ガイチ帝国", "Gaichi Empire"), 1100000L) },
    { AllianceType::LEGEND_OF_SAVIOR, std::make_unique<AllianceLegendOfSavior>(AllianceType::LEGEND_OF_SAVIOR, "LEGEND-OF-SAVIOR", _("世紀末救世主伝説", "Legend of the Latter-day Savior"), 0L) },
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

/*!
* @brief プレイヤーのレベル自体を印象値に加減算する処理
* @param player_ptr 評価対象とするプレイヤー
* @param bias 倍率
* @param min_level 評価基準最低レベル
*/
int Alliance::calcPlayerPower(PlayerType const &player_ptr, const int bias, const int min_level)
{
    if (min_level > player_ptr.lev) {
        return 0;
    }
    return (2000 + 10 * (player_ptr.lev - min_level + 1) * (player_ptr.lev - min_level + 1)) * bias / 100;
}

int64_t Alliance::calcCurrentPower()
{
    int64_t res = this->base_power;
    for (auto r : r_info) {
        if (r.alliance_idx == this->id) {
            if (r.mob_num > 0) {
                res += calc_monrace_eval(&r) * r.mob_num;
            }
        }
    }
    if (this->isAnnihilated()) {
        res /= this->AnihilatedPowerdownDiv;
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
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 10, 35);
    return impression;
}

int AllianceCourtOfChaos::calcImplessionPoint(PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 10, 35);
    return impression;
}

int AllianceValinor::calcImplessionPoint(PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += (creature_ptr->alignment > 0) ? creature_ptr->alignment : -creature_ptr->alignment * 3;
    impression += Alliance::calcPlayerPower(*creature_ptr, -16, 30);
    return impression;
}

int AllianceUtumno::calcImplessionPoint(PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 10, 30);
    return impression;
}

int AllianceJural::calcImplessionPoint(PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 5, 10);
    return impression;
}

bool AllianceJural::isAnnihilated()
{
    return r_info[MON_JURAL_WITCHKING].mob_num == 0;
}

int AllianceChinChinTei::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceOdio::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceKenohgun::calcImplessionPoint(PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 15, 20);
    return impression;
}

bool AllianceKenohgun::isAnnihilated()
{
    return r_info[MON_RAOU].mob_num == 0;
}

int AllianceFangFamily::calcImplessionPoint(PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 5, 10);
    return impression;
}

int AllianceKoganRyu::calcImplessionPoint(PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 15, 18);
    return impression;
}

int AllianceEldrazi::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceUngoliant::calcImplessionPoint(PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 8, 30);
    return impression;
}

int AllianceShittoDan::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

bool AllianceShittoDan::isAnnihilated()
{
    return r_info[MON_SHITTO_MASK].mob_num == 0;
}

int AllianceGEOrlic::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 10, 30);
    return impression;
}

int AllianceTurbanKids::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceNakedKnights::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceNumenor::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 10, 20);
    return impression;
}

int AllianceGO::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceTheShire::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, -10, 1);
    return impression;
}

int AllianceDokachans::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

bool AllianceDokachans::isAnnihilated()
{
    return r_info[MON_DOKACHAN].mob_num == 0;
}

int AllianceKetholdeth::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

bool AllianceKetholdeth::isAnnihilated()
{
    return r_info[MON_PRINCESS_KETHOLDETH].mob_num == 0;
}

int AllianceMeldor::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 13, 28);
    return impression;
}

int AllianceAngartha::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 11, 20);
    return impression;
}

int AllianceGetter::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 20, 1);
    return impression;
}

int AlliancePureMirrodin::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 10, 12);
    return impression;
}

int AllianceKING::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 15, 15);
    return impression;
}

int AlliancePhyrexia::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 15, 21);
    return impression;
}

int AllianceAvarinLords::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 10, 23);
    return impression;
}

int AllianceGOLAN::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 15, 12);
    return impression;
}

int AllianceBinzyouBuddhism::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

bool AllianceBinzyouBuddhism::isAnnihilated()
{
    return r_info[MON_BINZYOU_MUR].mob_num == 0;
}

int AllianceAshinaClan::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceSuren::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}

int AllianceFeanorNoldor::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 19, 26);
    return impression;
}

int AllianceGaichi::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    int impression = 0;
    impression += Alliance::calcPlayerPower(*creature_ptr, 4, 10);
    return impression;
}

int AllianceLegendOfSavior::calcImplessionPoint([[maybe_unused]] PlayerType *creature_ptr) const
{
    return 0;
}
