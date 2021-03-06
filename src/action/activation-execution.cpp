﻿/*!
 * @file activation-execution.cpp
 * @brief アイテムの発動実行定義
 */

#include "action/activation-execution.h"
#include "action/action-limited.h"
#include "artifact/random-art-effects.h"
#include "artifact/artifact-info.h"
#include "core/window-redrawer.h"
#include "effect/spells-effect-util.h"
#include "floor/geometry.h"
#include "flavor/flavor-describer.h"
#include "flavor/object-flavor-types.h"
#include "game-option/disturbance-options.h"
#include "game-option/input-options.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "monster-floor/monster-generator.h"
#include "monster-floor/place-monster-types.h"
#include "monster-race/monster-race.h"
#include "monster/monster-info.h"
#include "monster/monster-util.h"
#include "object-activation/activation-switcher.h"
#include "object-activation/activation-util.h"
#include "object-enchant/activation-info-table.h"
#include "object-enchant/object-ego.h"
#include "object-hook/hook-enchant.h"
#include "object/object-info.h"
#include "object/object-kind.h"
#include "player-status/player-energy.h"
#include "racial/racial-android.h"
#include "specific-object/monster-ball.h"
#include "spell-kind/spells-launcher.h"
#include "spell-kind/spells-teleport.h"
#include "spell-realm/spells-hex.h"
#include "spell-realm/spells-song.h"
#include "spell/spell-types.h"
#include "spell/spells-object.h"
#include "sv-definition/sv-bow-types.h"
#include "sv-definition/sv-lite-types.h"
#include "sv-definition/sv-ring-types.h"
#include "sv-definition/sv-junk-types.h"
#include "system/artifact-type-definition.h"
#include "system/floor-type-definition.h"
#include "system/monster-type-definition.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "target/target-getter.h"
#include "term/screen-processor.h"
#include "util/quarks.h"
#include "util/sort.h"
#include "util/bit-flags-calculator.h"
#include "view/display-messages.h"
#include "world/world.h"
#include "inventory/inventory-slot-types.h"

/*!
 * @brief アイテムの発動難度の設定 
 * @todo ランダムアーティファクトに破損率を指定する実装をそのうちするかも知れない。当分は0に。
 */
static void decide_activation_level(player_type *user_ptr, ae_type *ae_ptr)
{
    if (object_is_fixed_artifact(ae_ptr->o_ptr)) {
        ae_ptr->lev = a_info[ae_ptr->o_ptr->name1].level;
        ae_ptr->broken = a_info[ae_ptr->o_ptr->name1].broken_rate;
        return;
    }

    if (object_is_random_artifact(ae_ptr->o_ptr)) {
        const activation_type *const act_ptr = find_activation_info(user_ptr, ae_ptr->o_ptr);
        if (act_ptr != NULL)
            ae_ptr->lev = act_ptr->level;
        ae_ptr->broken = 0;
        return;
    }

    if (((ae_ptr->o_ptr->tval == TV_RING) || (ae_ptr->o_ptr->tval == TV_AMULET)) && ae_ptr->o_ptr->name2) {
        ae_ptr->lev = e_info[ae_ptr->o_ptr->name2].level;
        ae_ptr->broken = e_info[ae_ptr->o_ptr->name2].broken_rate;
    }
}

static void decide_chance_fail(player_type *user_ptr, ae_type *ae_ptr)
{
    ae_ptr->chance = user_ptr->skill_dev;
    if (user_ptr->confused)
        ae_ptr->chance = ae_ptr->chance / 2;

    ae_ptr->fail = ae_ptr->lev + 5;
    if (ae_ptr->chance > ae_ptr->fail)
        ae_ptr->fail -= (ae_ptr->chance - ae_ptr->fail) * 2;
    else
        ae_ptr->chance -= (ae_ptr->fail - ae_ptr->chance) * 2;

    if (ae_ptr->fail < USE_DEVICE)
        ae_ptr->fail = USE_DEVICE;

    if (ae_ptr->chance < USE_DEVICE)
        ae_ptr->chance = USE_DEVICE;
}

static void decide_activation_success(player_type *user_ptr, ae_type *ae_ptr)
{
    if (user_ptr->pclass == CLASS_BERSERKER) {
        ae_ptr->success = false;
        return;
    }

    if (ae_ptr->chance > ae_ptr->fail) {
        ae_ptr->success = randint0(ae_ptr->chance * 2) >= ae_ptr->fail;
        return;
    }

    ae_ptr->success = randint0(ae_ptr->fail * 2) < ae_ptr->chance;
}

static bool check_activation_success(ae_type *ae_ptr)
{
    if (ae_ptr->success)
        return true;

    if (flush_failure)
        flush();

    msg_print(_("うまく始動させることができなかった。", "You failed to activate it properly."));
    sound(SOUND_FAIL);
    return false;
}

static bool check_activation_conditions(player_type *user_ptr, ae_type *ae_ptr)
{
    if (!check_activation_success(ae_ptr))
        return false;

    if (ae_ptr->o_ptr->timeout) {
        msg_print(_("それは微かに音を立て、輝き、消えた...", "It whines, glows and fades..."));
        return false;
    }

    if (!ae_ptr->o_ptr->xtra4 && (ae_ptr->o_ptr->tval == TV_FLASK) && ((ae_ptr->o_ptr->sval == SV_LITE_TORCH) || (ae_ptr->o_ptr->sval == SV_LITE_LANTERN))) {
        msg_print(_("燃料がない。", "It has no fuel."));
        PlayerEnergy(user_ptr).reset_player_turn();
        return false;
    }

    return true;
}

/*!
 * @brief アイテムの発動効果を処理する。
 * @param user_ptr プレーヤーへの参照ポインタ
 * @param o_ptr 対象のオブジェクト構造体ポインタ
 * @return 発動実行の是非を返す。
 */
static bool activate_artifact(player_type *user_ptr, object_type *o_ptr)
{
    concptr name = k_info[o_ptr->k_idx].name.c_str();
    const activation_type *const act_ptr = find_activation_info(user_ptr, o_ptr);
    if (!act_ptr) {
        msg_print("Activation information is not found.");
        return false;
    }

    if (!switch_activation(user_ptr, &o_ptr, act_ptr, name))
        return false;

    if (act_ptr->timeout.constant >= 0) {
        o_ptr->timeout = (s16b)act_ptr->timeout.constant;
        if (act_ptr->timeout.dice > 0)
            o_ptr->timeout += randint1(act_ptr->timeout.dice);

        return true;
    }

    switch (act_ptr->index) {
    case ACT_BR_FIRE:
        o_ptr->timeout = ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_FLAMES)) ? 200 : 250;
        return true;
    case ACT_BR_COLD:
        o_ptr->timeout = ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ICE)) ? 200 : 250;
        return true;
    case ACT_TERROR:
        o_ptr->timeout = 3 * (user_ptr->lev + 10);
        return true;
    case ACT_MURAMASA:
        return true;
    default:
        msg_format("Special timeout is not implemented: %d.", act_ptr->index);
        return false;
    }
}

static bool activate_whistle(player_type *user_ptr, ae_type *ae_ptr)
{
    if (ae_ptr->o_ptr->tval != TV_WHISTLE)
        return false;

    if (music_singing_any(user_ptr))
        stop_singing(user_ptr);

    if (hex_spelling_any(user_ptr))
        stop_hex_spell_all(user_ptr);

    MONSTER_IDX pet_ctr;
    MONSTER_IDX *who;
    int max_pet = 0;
    C_MAKE(who, current_world_ptr->max_m_idx, MONSTER_IDX);
    for (pet_ctr = user_ptr->current_floor_ptr->m_max - 1; pet_ctr >= 1; pet_ctr--)
        if (is_pet(&user_ptr->current_floor_ptr->m_list[pet_ctr]) && (user_ptr->riding != pet_ctr))
            who[max_pet++] = pet_ctr;

    u16b dummy_why;
    ang_sort(user_ptr, who, &dummy_why, max_pet, ang_sort_comp_pet, ang_sort_swap_hook);
    for (MONSTER_IDX i = 0; i < max_pet; i++) {
        pet_ctr = who[i];
        teleport_monster_to(user_ptr, pet_ctr, user_ptr->y, user_ptr->x, 100, TELEPORT_PASSIVE);
    }

    C_KILL(who, current_world_ptr->max_m_idx, MONSTER_IDX);
    ae_ptr->o_ptr->timeout = 100 + randint1(100);
    return true;
}

static bool activate_firethrowing(player_type *user_ptr, ae_type *ae_ptr)
{
    if (ae_ptr->o_ptr->tval != TV_BOW || ae_ptr->o_ptr->sval != SV_FLAMETHROWER)
        return false;

    DIRECTION dir;
    if (!get_aim_dir(user_ptr, &dir))
        return false;

    msg_print(_("汚物は消毒だあ！", "The filth must be disinfected!"));
    fire_breath(user_ptr, GF_FIRE, dir, 20 + user_ptr->lev * 5, 2);
    return true;
}

static bool activate_rosmarinus(player_type *user_ptr, ae_type *ae_ptr)
{
    if (ae_ptr->o_ptr->tval != TV_BOW || ae_ptr->o_ptr->sval != SV_ROSMARINUS)
        return false;

    DIRECTION dir;
    if (!get_aim_dir(user_ptr, &dir))
        return false;

    fire_breath(user_ptr, GF_MISSILE, dir, 20 + user_ptr->lev * 5, 2);
    return true;
}

static bool activate_stungun(player_type *user_ptr, ae_type *ae_ptr)
{
    if (ae_ptr->o_ptr->tval != TV_JUNK || ae_ptr->o_ptr->sval != SV_STUNGUN)
        return false;

    DIRECTION dir;
    project_length = 1;
    if (!get_aim_dir(user_ptr, &dir))
        return false;

    msg_print(_("『バチィ』", "'bzzt'"));
    fire_ball(user_ptr, GF_STUNGUN, dir, user_ptr->lev, 0);
    project_length = 0;
    return true;
}

static bool activate_raygun(player_type *user_ptr, ae_type *ae_ptr)
{
    if (ae_ptr->o_ptr->tval != TV_BOW || ae_ptr->o_ptr->sval != SV_RAYGUN)
        return false;

    DIRECTION dir;
    if (!get_aim_dir(user_ptr, &dir))
        return false;

    msg_print(_("『ビィーム！』", "'ZAP! ZAP!'"));
    fire_bolt(user_ptr, GF_MISSILE, dir, 10 + user_ptr->lev * 2);
    return true;
}

/*!
 * @brief 装備を発動するコマンドのサブルーチン /
 * Activate a wielded object.  Wielded objects never stack.
 * And even if they did, activatable objects never stack.
 * @param item 発動するオブジェクトの所持品ID
 * @details
 * <pre>
 * Currently, only (some) artifacts, and Dragon Scale Mail, can be activated.
 * But one could, for example, easily make an activatable "Ring of Plasma".
 * Note that it always takes a turn to activate an artifact, even if
 * the user hits "escape" at the "direction" prompt.
 * </pre>
 */
void exe_activate(player_type *user_ptr, INVENTORY_IDX item)
{
    bool activated = false;
    if (item <= INVEN_PACK && !has_flag(k_info[user_ptr->inventory_list[item].k_idx].flags, TR_INVEN_ACTIVATE)) {
        msg_print(_("このアイテムは装備しないと始動できない。", "That object must be activated by equipment."));
        return;
    }

    ae_type tmp_ae;
    ae_type *ae_ptr = initialize_ae_type(user_ptr, &tmp_ae, item);

    if (ae_ptr->o_ptr->name2 == EGO_SHATTERED || ae_ptr->o_ptr->name2 == EGO_BLASTED) {
        msg_print(_("このアイテムはもう壊れていて始動できない。", "That broken object can't be activated."));
        return;        
    }

    PlayerEnergy(user_ptr).set_player_turn_energy(100);

    decide_activation_level(user_ptr, ae_ptr);
    decide_chance_fail(user_ptr, ae_ptr);
    if (cmd_limit_time_walk(user_ptr))
        return;

    decide_activation_success(user_ptr, ae_ptr);
    if (!check_activation_conditions(user_ptr, ae_ptr))
        return;

    msg_print(_("始動させた...", "You activate it..."));
    sound(SOUND_ZAP);
    if (activation_index(user_ptr, ae_ptr->o_ptr)) {
        (void)activate_artifact(user_ptr, ae_ptr->o_ptr);
        user_ptr->window_flags |= PW_INVEN | PW_EQUIP;
        activated = true;
    }

    if (activate_whistle(user_ptr, ae_ptr)) {
        activated = true;
    } else if (exe_monster_capture(user_ptr, ae_ptr)) {
        activated = true;
    } else if (activate_firethrowing(user_ptr, ae_ptr)) {
        activated = true;
    } else if (activate_rosmarinus(user_ptr, ae_ptr)) {
        activated = true;
    } else if (activate_stungun(user_ptr, ae_ptr)) {
        activated = true;
    } else if (activate_raygun(user_ptr, ae_ptr)) {
        activated = true;
    }

    if (!activated) {
        msg_print(_("おっと、このアイテムは始動できない。", "Oops.  That object cannot be activated."));
        return;    
    }

    if (randint1(100) <= ae_ptr->broken) {
        char o_name[MAX_NLEN];
        describe_flavor(user_ptr, o_name, ae_ptr->o_ptr, OD_OMIT_PREFIX);

        msg_format(_("%sは壊れた！", "%s is destroyed!"), o_name);
        curse_weapon_object(user_ptr, true, ae_ptr->o_ptr);
    }

}
