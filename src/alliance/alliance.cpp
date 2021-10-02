#include "alliance/alliance.h"
#include "system/player-type-definition.h"

const std::vector<std::shared_ptr<Alliance>> alliance_list = {
    std::make_unique<AllianceNone>(0, "NONE", _("無所属", "None")),
    std::make_unique<AllianceAmber>(1, "AMBER", _("アンバー", "Amber")),
    std::make_unique<AllianceCourtOfChaos>(2, "COCHAOS", _("混沌の宮廷", "Court of Chaos")),
    std::make_unique<AllianceValinor>(3, "VARINOR", _("ヴァリノール", "Valinor")),
    std::make_unique<AllianceCourtOfChaos>(4, "UTUMNO", _("ウトゥムノ", "Utumno"))
};

Alliance::Alliance(int id, std::string tag, std::string name)
    : id(id)
    , tag(tag)
    , name(name)
{

}

int AllianceNone::calcImplessionPoint([[maybe_unused]] player_type *creature_ptr) const
{
    return 0;
}

int AllianceAmber::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : creature_ptr->alignment;
}

int AllianceCourtOfChaos::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceValinor::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment : -creature_ptr->alignment * 2;
}

int AllianceUtumno::calcImplessionPoint(player_type *creature_ptr) const
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}
