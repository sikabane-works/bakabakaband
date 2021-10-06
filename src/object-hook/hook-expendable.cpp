﻿#include "object-hook/hook-expendable.h"
#include "artifact/fixed-art-types.h"
#include "core/player-update-types.h"
#include "core/window-redrawer.h"
#include "monster-race/monster-race.h"
#include "object-enchant/item-feeling.h"
#include "object-enchant/special-object-flags.h"
#include "object-enchant/tr-types.h"
#include "object-hook/hook-enchant.h"
#include "object/object-kind.h"
#include "object/object-flags.h"
#include "perception/object-perception.h"
#include "player-base/player-race.h"
#include "player-info/mimic-info-table.h"
#include "sv-definition/sv-lite-types.h"
#include "sv-definition/sv-other-types.h"
#include "system/monster-race-definition.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "util/string-processor.h"
#include "util/bit-flags-calculator.h"


/*!
 * @brief オブジェクトをプレイヤーが飲むことができるかを判定する /
 * Hook to determine if an object can be quaffed
 * @param o_ptr 判定したいオブジェクトの構造体参照ポインタ
 * @return 飲むことが可能ならばTRUEを返す
 */
bool item_tester_hook_quaff(player_type *player_ptr, const object_type *o_ptr)
{
    if (o_ptr->tval == TV_POTION)
        return true;

    if (PlayerRace(player_ptr).food() == PlayerRaceFood::OIL && o_ptr->tval == TV_FLASK && o_ptr->sval == SV_FLASK_OIL)
        return true;

    return false;
}

/*!
 * @brief 破壊可能なアイテムかを返す /
 * Determines whether an object can be destroyed, and makes fake inscription.
 * @param o_ptr 破壊可能かを確認したいオブジェクトの構造体参照ポインタ
 * @return オブジェクトが破壊可能ならばTRUEを返す
 */
bool can_player_destroy_object(player_type *player_ptr, object_type *o_ptr)
{
    auto flgs = object_flags(o_ptr);
    if (flgs.has(TR_INDESTRUCTIBLE))
        return false;


    /* Artifacts cannot be destroyed */
    if (!o_ptr->is_artifact())
        return true;

    if (!o_ptr->is_known()) {
        byte feel = FEEL_SPECIAL;
        if (o_ptr->is_cursed() || o_ptr->is_broken())
            feel = FEEL_TERRIBLE;

        o_ptr->feeling = feel;
        o_ptr->ident |= IDENT_SENSE;
        player_ptr->update |= (PU_COMBINE);
        player_ptr->window_flags |= (PW_INVEN | PW_EQUIP);
        return false;
    }

    return false;
}
