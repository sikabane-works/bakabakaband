#include "alliance/alliance.h"
#include "system/player-type-definition.h"

int AllianceAmber::calcImplessionPoint(player_type* creature_ptr)
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : creature_ptr->alignment;
}

int AllianceCourtOfChaos::calcImplessionPoint(player_type *creature_ptr)
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}

int AllianceValinor::calcImplessionPoint(player_type *creature_ptr)
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment : -creature_ptr->alignment * 2;
}

int AllianceUtumno::calcImplessionPoint(player_type *creature_ptr)
{
    return (creature_ptr->alignment > 0) ? creature_ptr->alignment / 3 : -creature_ptr->alignment / 2;
}
