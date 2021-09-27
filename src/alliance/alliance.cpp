#include "alliance/alliance.h"
#include "system/player-type-definition.h"

const std::vector<std::shared_ptr<Alliance>> alliance_list = { std::make_unique<AllianceAmber>(), std::make_unique<AllianceCourtOfChaos>(),
    std::make_unique<AllianceValinor>(), std::make_unique<AllianceCourtOfChaos>() };

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
