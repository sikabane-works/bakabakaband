﻿#include "cmd-item/cmd-refill.h"
#include "core/player-redraw-types.h"
#include "core/player-update-types.h"
#include "floor/floor-object.h"
#include "inventory/inventory-object.h"
#include "inventory/inventory-slot-types.h"
#include "object-enchant/object-ego.h"
#include "object-enchant/tr-types.h"
#include "object-hook/hook-expendable.h"
#include "object/item-tester-hooker.h"
#include "object/item-use-flags.h"
#include "object/object-flags.h"
#include "player-base/player-class.h"
#include "player-info/samurai-data-type.h"
#include "player-status/player-energy.h"
#include "player/attack-defense-types.h"
#include "player/special-defense-types.h"
#include "status/action-setter.h"
#include "sv-definition/sv-lite-types.h"
#include "system/item-entity.h"
#include "system/player-type-definition.h"
#include "util/bit-flags-calculator.h"
#include "view/display-messages.h"

/*!
 * @brief ランタンに燃料を加えるコマンドのメインルーチン
 * Refill the players lamp (from the pack or floor)
 */
static void do_cmd_refill_lamp(PlayerType *player_ptr)
{
    OBJECT_IDX item;
    ItemEntity *o_ptr;
    ItemEntity *j_ptr;
    concptr q = _("どの油つぼから注ぎますか? ", "Refill with which flask? ");
    concptr s = _("油つぼがない。", "You have no flasks of oil.");
    o_ptr = choose_object(player_ptr, &item, q, s, USE_INVEN | USE_FLOOR, FuncItemTester(&ItemEntity::can_refill_lantern));
    if (!o_ptr) {
        return;
    }

    auto flags = object_flags(o_ptr);

    PlayerEnergy(player_ptr).set_player_turn_energy(50);
    j_ptr = &player_ptr->inventory_list[INVEN_LITE];
    auto flags2 = object_flags(j_ptr);
    j_ptr->fuel += o_ptr->fuel;
    msg_print(_("ランプに油を注いだ。", "You fuel your lamp."));
    if (flags.has(TR_DARK_SOURCE) && (j_ptr->fuel > 0)) {
        j_ptr->fuel = 0;
        msg_print(_("ランプが消えてしまった！", "Your lamp has gone out!"));
    } else if (flags.has(TR_DARK_SOURCE) || flags2.has(TR_DARK_SOURCE)) {
        j_ptr->fuel = 0;
        msg_print(_("しかしランプは全く光らない。", "Curiously, your lamp doesn't light."));
    } else if (j_ptr->fuel >= FUEL_LAMP) {
        j_ptr->fuel = FUEL_LAMP;
        msg_print(_("ランプの油は一杯だ。", "Your lamp is full."));
    }

    vary_item(player_ptr, item, -1);
    player_ptr->update |= PU_TORCH;
}

/*!
 * @brief 松明を束ねるコマンドのメインルーチン
 * Refuel the players torch (from the pack or floor)
 */
static void do_cmd_refill_torch(PlayerType *player_ptr)
{
    OBJECT_IDX item;
    ItemEntity *o_ptr;
    ItemEntity *j_ptr;
    concptr q = _("どの松明で明かりを強めますか? ", "Refuel with which torch? ");
    concptr s = _("他に松明がない。", "You have no extra torches.");
    o_ptr = choose_object(player_ptr, &item, q, s, USE_INVEN | USE_FLOOR, FuncItemTester(&ItemEntity::can_refill_torch));
    if (!o_ptr) {
        return;
    }

    auto flags = object_flags(o_ptr);

    PlayerEnergy(player_ptr).set_player_turn_energy(50);
    j_ptr = &player_ptr->inventory_list[INVEN_LITE];
    auto flags2 = object_flags(j_ptr);
    j_ptr->fuel += o_ptr->fuel + 5;
    msg_print(_("松明を結合した。", "You combine the torches."));
    if (flags.has(TR_DARK_SOURCE) && (j_ptr->fuel > 0)) {
        j_ptr->fuel = 0;
        msg_print(_("松明が消えてしまった！", "Your torch has gone out!"));
    } else if (flags.has(TR_DARK_SOURCE) || flags2.has(TR_DARK_SOURCE)) {
        j_ptr->fuel = 0;
        msg_print(_("しかし松明は全く光らない。", "Curiously, your torch doesn't light."));
    } else if (j_ptr->fuel >= FUEL_TORCH) {
        j_ptr->fuel = FUEL_TORCH;
        msg_print(_("松明の寿命は十分だ。", "Your torch is fully fueled."));
    } else {
        msg_print(_("松明はいっそう明るく輝いた。", "Your torch glows more brightly."));
    }

    vary_item(player_ptr, item, -1);
    player_ptr->update |= PU_TORCH;
}

/*!
 * @brief 燃料を補充するコマンドのメインルーチン
 * Refill the players lamp, or restock his torches
 */
void do_cmd_refill(PlayerType *player_ptr)
{
    ItemEntity *o_ptr;
    o_ptr = &player_ptr->inventory_list[INVEN_LITE];

    PlayerClass(player_ptr).break_samurai_stance({ SamuraiStanceType::MUSOU });

    if (o_ptr->tval != ItemKindType::LITE) {
        msg_print(_("光源を装備していない。", "You are not wielding a light."));
    } else if (o_ptr->sval == SV_LITE_LANTERN) {
        do_cmd_refill_lamp(player_ptr);
    } else if (o_ptr->sval == SV_LITE_TORCH) {
        do_cmd_refill_torch(player_ptr);
    } else {
        msg_print(_("この光源は寿命を延ばせない。", "Your light cannot be refilled."));
    }
}