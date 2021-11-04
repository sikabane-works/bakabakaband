#include "alliance/alliance.h"
#include "system/player-type-definition.h"

const std::map<int, std::shared_ptr<Alliance>> alliance_list = {
    { 0, std::make_unique<AllianceNone>(0, "NONE", _("無所属", "None"), 0) },
    { 1, std::make_unique<AllianceAmber>(1, "AMBER", _("アンバー", "Amber"), 350000000L) },
    { 2, std::make_unique<AllianceCourtOfChaos>(2, "COCHAOS", _("混沌の宮廷", "Court of Chaos"), 200000000L) },
    { 3, std::make_unique<AllianceValinor>(3, "VARINOR", _("ヴァリノール", "Valinor"), 4000000L) },
    { 4, std::make_unique<AllianceCourtOfChaos>(4, "UTUMNO", _("ウトゥムノ", "Utumno"), 3000000L) },
    { 5, std::make_unique<AllianceJural>(5, "JURAL", _("ジュラル星人", "Jural"), 5500L) },
    { 6, std::make_unique<AllianceChinChinTei>(6, "CHINCHINTEI", _("ちんちん亭", "Chin-Chin-Tei"), 191919L) },
    { 7, std::make_unique<AllianceOdio>(7, "ODIO", _("オディオ", "Odio"), 300000L) }
};

Alliance::Alliance(int id, std::string tag, std::string name, int64_t base_power)
    : id(id)
    , tag(tag)
    , name(name)
    , base_power(base_power)
{

}

int64_t Alliance::calcCurrentPower()
{
    return this->base_power;
}

int AllianceNone::calcImplessionPoint([[maybe_unused]] player_type *creature_ptr) const
{
    return 0;
}

int AllianceAmber::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment;
}

int AllianceCourtOfChaos::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceValinor::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment : creature_ptr->alignment * 3;
}

int AllianceUtumno::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceJural::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceChinChinTei::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceOdio::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}
