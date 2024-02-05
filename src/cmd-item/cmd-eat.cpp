/*!
 * @brief プレイヤーの食べるコマンド実装
 * @date 2018/09/07
 * @author deskull
 */

#include "cmd-item/cmd-eat.h"
#include "avatar/avatar.h"
#include "core/player-update-types.h"
#include "core/window-redrawer.h"
#include "flavor/flavor-describer.h"
#include "flavor/object-flavor-types.h"
#include "floor/floor-object.h"
#include "hpmp/hp-mp-processor.h"
#include "inventory/inventory-object.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags9.h"
#include "object-enchant/special-object-flags.h"
#include "object-hook/hook-expendable.h"
#include "object/item-tester-hooker.h"
#include "object/item-use-flags.h"
#include "object/object-info.h"
#include "object/object-kind-hook.h"
#include "perception/object-perception.h"
#include "player-base/player-class.h"
#include "player-base/player-race.h"
#include "player-info/class-info.h"
#include "player-info/mimic-info-table.h"
#include "player-info/samurai-data-type.h"
#include "player-status/player-energy.h"
#include "player/attack-defense-types.h"
#include "player/digestion-processor.h"
#include "player/eldritch-horror.h"
#include "player/player-damage.h"
#include "player/player-skill.h"
#include "player/player-status-flags.h"
#include "player/special-defense-types.h"
#include "spell-kind/spells-launcher.h"
#include "spell-realm/spells-hex.h"
#include "spell-realm/spells-song.h"
#include "spell/spell-types.h"
#include "spell/spells-status.h"
#include "status/action-setter.h"
#include "status/bad-status-setter.h"
#include "status/base-status.h"
#include "status/buff-setter.h"
#include "status/element-resistance.h"
#include "status/experience.h"
#include "status/shape-changer.h"
#include "sv-definition/sv-food-types.h"
#include "sv-definition/sv-junk-types.h"
#include "sv-definition/sv-other-types.h"
#include "system/baseitem-info.h"
#include "system/item-entity.h"
#include "system/monster-race-info.h"
#include "system/player-type-definition.h"
#include "util/string-processor.h"
#include "view/display-messages.h"
#include "view/object-describer.h"

/*!
 * @brief ゴミみてえなものを食べたときの効果を発動
 * @param player_ptr プレイヤー情報への参照ポインタ
 * @param o_ptr 食べるオブジェクト
 * @return 鑑定されるならTRUE、されないならFALSE
 */
static bool exe_eat_junk_type_object(PlayerType *player_ptr, ItemEntity *o_ptr)
{
    if (o_ptr->bi_key.tval() != ItemKindType::JUNK) {
        return false;
    }

    if (o_ptr->bi_key.sval() == SV_JUNK_FECES || o_ptr->bi_key.sval() == SV_KMR_CURRY) {
        msg_print("ワーォ！貴方は糞を喰った！");
        msg_print("『涙が出るほどうめぇ……』");
        if (!(has_resist_pois(player_ptr) || is_oppose_pois(player_ptr))) {
            (void)BadStatusSetter(player_ptr).mod_poison(10 + randint1(10));
        }
        player_ptr->plus_incident(INCIDENT::EAT_FECES, 1);
        PlayerSkill(player_ptr).gain_riding_skill_exp_on_gross_eating();
        return true;
    }
    if (o_ptr->bi_key.sval() == SV_JUNK_VOMITTING) {
        msg_print("ワーォ！貴方はゲロを喰った！");
        msg_print("『涙が出るほどうめぇ……』");
        if (!(has_resist_pois(player_ptr) || is_oppose_pois(player_ptr))) {
            (void)BadStatusSetter(player_ptr).mod_poison(10 + randint1(10));
        }
        player_ptr->plus_incident(INCIDENT::EAT_FECES, 1);
        PlayerSkill(player_ptr).gain_riding_skill_exp_on_gross_eating();
        return true;
    }
    return false;
}

/*!
 * @brief ソウルを食べたときの効果を発動
 * @param player_ptr プレイヤー情報への参照ポインタ
 * @param o_ptr 食べるオブジェクト
 * @return 鑑定されるならTRUE、されないならFALSE
 */
static bool exe_eat_soul(PlayerType *player_ptr, ItemEntity *o_ptr)
{
    if (!(o_ptr->bi_key.tval() == ItemKindType::CORPSE && o_ptr->bi_key.sval() == SV_SOUL)) {
        return false;
    }

    if (player_ptr->prace == PlayerRaceType::ANDROID) {
        return false;
    }

    MonsterRaceInfo *r_ptr = &monraces_info[i2enum<MonsterRaceId, int>(o_ptr->pval)];
    EXP max_exp = r_ptr->level * r_ptr->level * 10;

    chg_virtue(player_ptr, Virtue::ENLIGHTEN, 1);
    if (player_ptr->exp < PY_MAX_EXP) {
        EXP ee = (player_ptr->exp / 2) + 10;
        if (ee > max_exp) {
            ee = max_exp;
        }
        msg_print(_("更に経験を積んだような気がする。", "You feel more experienced."));
        gain_exp(player_ptr, ee);
    }
    return true;
}

/*!
 * @brief 死体を食べたときの効果を発動
 * @param player_ptr プレイヤー情報への参照ポインタ
 * @param o_ptr 食べるオブジェクト
 * @return 鑑定されるならTRUE、されないならFALSE
 */
static bool exe_eat_corpse_type_object(PlayerType *player_ptr, ItemEntity *o_ptr)
{
    if (!(o_ptr->bi_key.tval() == ItemKindType::CORPSE && o_ptr->bi_key.sval() == SV_CORPSE)) {
        return false;
    }

    MonsterRaceInfo *r_ptr = &monraces_info[i2enum<MonsterRaceId, int>(o_ptr->pval)];

    if (r_ptr->flags9 & RF9_EAT_BLIND) {
        BadStatusSetter(player_ptr).mod_blindness(200 + randint1(200));
    }

    if (r_ptr->flags9 & RF9_EAT_CONF) {
        BadStatusSetter(player_ptr).mod_confusion(200 + randint1(200));
    }

    if (r_ptr->flags9 & RF9_EAT_MANA) {
        restore_mana(player_ptr, false);
    }

    if (r_ptr->flags9 & RF9_EAT_NEXUS) {
        do_poly_self(player_ptr);
    }

    if (r_ptr->flags9 & RF9_EAT_SLEEP) {
        if (!player_ptr->free_act) {
            BadStatusSetter(player_ptr).set_paralysis(10 + randint1(10));
        }
    }

    if (r_ptr->flags9 & RF9_EAT_BERSERKER) {
        set_shero(player_ptr, player_ptr->shero + randint1(10) + 10, false);
    }

    if (r_ptr->flags9 & RF9_EAT_ACIDIC) {
    }

    if (r_ptr->flags9 & RF9_EAT_SPEED) {
        (void)set_acceleration(player_ptr, randint1(20) + 20, false);
    }

    if (r_ptr->flags9 & RF9_EAT_CURE) {
        true_healing(player_ptr, 50);
    }

    if (r_ptr->flags9 & RF9_EAT_FIRE_RES) {
        set_oppose_fire(player_ptr, randint1(20) + 20, false);
    }

    if (r_ptr->flags9 & RF9_EAT_COLD_RES) {
        set_oppose_cold(player_ptr, randint1(20) + 20, false);
    }

    if (r_ptr->flags9 & RF9_EAT_ELEC_RES) {
        set_oppose_elec(player_ptr, randint1(20) + 20, false);
    }

    if (r_ptr->flags9 & RF9_EAT_POIS_RES) {
        set_oppose_pois(player_ptr, randint1(20) + 20, false);
    }

    if (r_ptr->flags9 & RF9_EAT_INSANITY) {
        sanity_blast(player_ptr, NULL, false);
    }

    if (r_ptr->flags9 & RF9_EAT_DRAIN_EXP) {
    }

    if (r_ptr->flags9 & RF9_EAT_POISONOUS) {
        if (!(has_resist_pois(player_ptr) || is_oppose_pois(player_ptr))) {
            (void)BadStatusSetter(player_ptr).mod_poison(10 + randint1(15));
        }
    }

    if (r_ptr->flags9 & RF9_EAT_GIVE_STR) {
        do_inc_stat(player_ptr, A_STR);
    }

    if (r_ptr->flags9 & RF9_EAT_GIVE_INT) {
        do_inc_stat(player_ptr, A_INT);
    }

    if (r_ptr->flags9 & RF9_EAT_GIVE_WIS) {
        do_inc_stat(player_ptr, A_WIS);
    }

    if (r_ptr->flags9 & RF9_EAT_GIVE_DEX) {
        do_inc_stat(player_ptr, A_DEX);
    }

    if (r_ptr->flags9 & RF9_EAT_GIVE_CON) {
        do_inc_stat(player_ptr, A_CON);
    }

    if (r_ptr->flags9 & RF9_EAT_GIVE_CHR) {
        do_inc_stat(player_ptr, A_CHR);
    }

    if (r_ptr->flags9 & RF9_EAT_LOSE_STR) {
        do_dec_stat(player_ptr, A_STR);
    }

    if (r_ptr->flags9 & RF9_EAT_LOSE_INT) {
        do_dec_stat(player_ptr, A_INT);
    }

    if (r_ptr->flags9 & RF9_EAT_LOSE_WIS) {
        do_dec_stat(player_ptr, A_WIS);
    }

    if (r_ptr->flags9 & RF9_EAT_LOSE_DEX) {
        do_dec_stat(player_ptr, A_DEX);
    }

    if (r_ptr->flags9 & RF9_EAT_LOSE_CON) {
        do_dec_stat(player_ptr, A_CON);
    }

    if (r_ptr->flags9 & RF9_EAT_LOSE_CHR) {
        do_dec_stat(player_ptr, A_CHR);
    }

    if (r_ptr->flags9 & RF9_EAT_DRAIN_MANA) {
        player_ptr->csp -= 30;
        if (player_ptr->csp < 0) {
            player_ptr->csp = 0;
            player_ptr->csp_frac = 0;
        }
    }

    return true;
}

/*!
 * @brief 食料タイプの食料を食べたときの効果を発動
 * @param player_ptr プレイヤー情報への参照ポインタ
 * @param o_ptr 食べるオブジェクト
 * @return 鑑定されるならTRUE、されないならFALSE
 */
static bool exe_eat_food_type_object(PlayerType *player_ptr, const BaseitemKey &bi_key)
{
    if (bi_key.tval() != ItemKindType::FOOD) {
        return false;
    }

    BadStatusSetter bss(player_ptr);
    switch (bi_key.sval().value()) {
    case SV_FOOD_POISON:
        if (!(has_resist_pois(player_ptr) || is_oppose_pois(player_ptr))) {
            if (bss.mod_poison(randint0(10) + 10)) {
                player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
                return true;
            }
        }
        break;
    case SV_FOOD_BLINDNESS:
        if (!has_resist_blind(player_ptr)) {
            if (bss.mod_blindness(randint0(200) + 200)) {
                player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
                return true;
            }
        }
        break;
    case SV_FOOD_PARANOIA:
        if (!has_resist_fear(player_ptr)) {
            if (bss.mod_fear(randint0(10) + 10)) {
                player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
                return true;
            }
        }
        break;
    case SV_FOOD_CONFUSION:
        if (!has_resist_conf(player_ptr)) {
            if (bss.mod_confusion(randint0(10) + 10)) {
                player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
                return true;
            }
        }
        break;
    case SV_FOOD_HALLUCINATION:
        if (!has_resist_chaos(player_ptr)) {
            if (bss.mod_hallucination(randint0(250) + 250)) {
                player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
                return true;
            }
        }
        break;
    case SV_FOOD_PARALYSIS:
        if (!player_ptr->free_act) {
            if (bss.set_paralysis(10 + randint1(10))) {
                player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
                return true;
            }
        }
        break;
    case SV_FOOD_WEAKNESS:
        player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
        take_hit(player_ptr, DAMAGE_NOESCAPE, damroll(6, 6), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(player_ptr, A_STR);
        return true;
    case SV_FOOD_SICKNESS:
        player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
        take_hit(player_ptr, DAMAGE_NOESCAPE, damroll(6, 6), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(player_ptr, A_CON);
        return true;
    case SV_FOOD_STUPIDITY:
        player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
        take_hit(player_ptr, DAMAGE_NOESCAPE, damroll(8, 8), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(player_ptr, A_INT);
        return true;
    case SV_FOOD_NAIVETY:
        player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
        take_hit(player_ptr, DAMAGE_NOESCAPE, damroll(8, 8), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(player_ptr, A_WIS);
        return true;
    case SV_FOOD_UNHEALTH:
        player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
        take_hit(player_ptr, DAMAGE_NOESCAPE, damroll(10, 10), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(player_ptr, A_CON);
        return true;
    case SV_FOOD_DISEASE:
        player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
        take_hit(player_ptr, DAMAGE_NOESCAPE, damroll(10, 10), _("毒入り食料", "poisonous food"));
        (void)do_dec_stat(player_ptr, A_STR);
        return true;
    case SV_FOOD_CURE_POISON:
        return bss.set_poison(0);
    case SV_FOOD_CURE_BLINDNESS:
        return bss.set_blindness(0);
    case SV_FOOD_CURE_PARANOIA:
        return bss.set_fear(0);
    case SV_FOOD_CURE_CONFUSION:
        return bss.set_confusion(0);
    case SV_FOOD_CURE_SERIOUS:
        return cure_serious_wounds(player_ptr, 4, 8);
    case SV_FOOD_RESTORE_STR:
        return do_res_stat(player_ptr, A_STR);
    case SV_FOOD_RESTORE_CON:
        return do_res_stat(player_ptr, A_CON);
    case SV_FOOD_RESTORING:
        return restore_all_status(player_ptr);
    case SV_FOOD_BISCUIT:
        msg_print(_("甘くてサクサクしてとてもおいしい。", "That is sweet, crispy, and very delicious."));
        return true;
    case SV_FOOD_JERKY:
        msg_print(_("歯ごたえがあっておいしい。", "That is chewy and delicious."));
        return true;
    case SV_FOOD_SLIME_MOLD:
        msg_print(_("これはなんとも形容しがたい味だ。", "That is an indescribable taste."));
        return true;
    case SV_FOOD_BROWNIW_OF_ALC:
        msg_print("実際に美味で「しっとりとしていて、それでいてべたつかないスッキリとした甘さ」ではあった。");
        return true;
    case SV_FOOD_RATION:
        msg_print(_("これはおいしい。", "That tastes good."));
        return true;
    case SV_FOOD_WAYBREAD:
        msg_print(_("これはひじょうに美味だ。", "That tastes very good."));
        (void)bss.set_poison(0);
        (void)hp_player(player_ptr, damroll(4, 8));
        return true;
    case SV_FOOD_PINT_OF_ALE:
    case SV_FOOD_PINT_OF_WINE:
        msg_print(_("のどごし爽やかだ。", "That is refreshing and warms the innards."));
        return true;
    case SV_FOOD_WELCOME_DRINK_OF_ARE:
    case SV_FOOD_ABA_TEA:
        player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
        (void)BadStatusSetter(player_ptr).mod_poison(10);
        msg_print("「非常に新鮮で……非常においしい……」");
        player_ptr->plus_incident(INCIDENT::EAT_FECES, 1);
        return true;
    case SV_FOOD_SEED_FEA:
        msg_print("脱穀して炊いた方が良かったかもしれないが、多少空腹は収まった。");
        return true;
    case SV_FOOD_HIP:
        player_ptr->plus_incident(INCIDENT::EAT_POISON, 1);
        player_ptr->incident[INCIDENT::EAT_POISON]++;
        msg_print("ヴォエ！食ったら尻の肉だった！");
        msg_print(NULL);
        (void)BadStatusSetter(player_ptr).mod_poison(10);
        msg_print("「作者は広告で収入得てないけど、こんな卑猥なアイテム放置するなよ」");
        msg_print(NULL);
        player_ptr->plus_incident(INCIDENT::EAT_FECES, 1);
        return true;
    case SV_FOOD_SURSTROMMING:
        msg_print("悪臭が周囲を取り巻いた！");
        msg_print(NULL);
        fire_ball(player_ptr, AttributeType::POIS, 0, 30, 4);
        (void)BadStatusSetter(player_ptr).mod_poison(10);
        return true;
    case SV_FOOD_HOMOTEA:
        (void)BadStatusSetter(player_ptr).mod_stun(200);
        msg_print("「お、大丈夫か？大丈夫か？……」");
        return true;
    case SV_FOOD_GOLDEN_EGG:
        (void)do_inc_stat(player_ptr, randint0(6));
        return true;
    case SV_FOOD_ABESHI:
        gain_exp(player_ptr, player_ptr->lev * 50);
        (void)set_hero(player_ptr, randint1(10) + 10, false);
        if (one_in_(300)) {
            (void)do_inc_stat(player_ptr, A_STR);
        }
        if (one_in_(300)) {
            (void)do_inc_stat(player_ptr, A_DEX);
        }
        if (one_in_(300)) {
            (void)do_inc_stat(player_ptr, A_CON);
        }
        return true;
    case SV_FOOD_HIDEBU:
        gain_exp(player_ptr, player_ptr->lev * 100);
        (void)set_hero(player_ptr, randint1(25) + 25, false);
        if (one_in_(100)) {
            (void)do_inc_stat(player_ptr, A_STR);
        }
        if (one_in_(100)) {
            (void)do_inc_stat(player_ptr, A_DEX);
        }
        if (one_in_(100)) {
            (void)do_inc_stat(player_ptr, A_CON);
        }
        return true;
    case SV_FOOD_BASILISK_TIME:
        gain_exp(player_ptr, player_ptr->lev * 100);
        msg_print("あなたは突如狂ったように踊り始めた！");
        msg_print("「みずのよーうにのようにやさしく！はなのよーうにはげしく！ふーるえ……」");
        (void)BadStatusSetter(player_ptr).mod_stun(25 + randint1(25));
        (void)set_hero(player_ptr, randint1(25) + 25, false);
        (void)set_shero(player_ptr, randint1(25) + 25, false);
        if (one_in_(100)) {
            (void)do_inc_stat(player_ptr, A_STR);
        }
        if (one_in_(100)) {
            (void)do_inc_stat(player_ptr, A_DEX);
        }
        if (one_in_(100)) {
            (void)do_inc_stat(player_ptr, A_CON);
        }
        return true;
    default:
        return true;
    }
    return false;
}

/*!
 * @brief 魔法道具のチャージをの食料として食べたときの効果を発動
 * @param player_ptr プレイヤー情報への参照ポインタ
 * @param o_ptr 食べるオブジェクト
 * @param inventory オブジェクトのインベントリ番号
 * @return 食べようとしたらTRUE、しなかったらFALSE
 */
static bool exe_eat_charge_of_magic_device(PlayerType *player_ptr, ItemEntity *o_ptr, short inventory)
{
    if (!o_ptr->is_wand_staff() || (PlayerRace(player_ptr).food() != PlayerRaceFoodType::MANA)) {
        return false;
    }

    const auto is_staff = o_ptr->bi_key.tval() == ItemKindType::STAFF;
    if (is_staff && (inventory < 0) && (o_ptr->number > 1)) {
        msg_print(_("まずは杖を拾わなければ。", "You must first pick up the staffs."));
        return true;
    }

    const auto staff = is_staff ? _("杖", "staff") : _("魔法棒", "wand");

    /* "Eat" charges */
    if (o_ptr->pval == 0) {
        msg_format(_("この%sにはもう魔力が残っていない。", "The %s has no charges left."), staff);
        o_ptr->ident |= IDENT_EMPTY;
        player_ptr->window_flags |= PW_INVENTORY;
        return true;
    }

    msg_format(_("あなたは%sの魔力をエネルギー源として吸収した。", "You absorb mana of the %s as your energy."), staff);

    /* Use a single charge */
    o_ptr->pval--;

    /* Eat a charge */
    set_food(player_ptr, player_ptr->food + 5000);

    /* XXX Hack -- unstack if necessary */
    if (is_staff && (inventory >= 0) && (o_ptr->number > 1)) {
        auto item = *o_ptr;

        /* Modify quantity */
        item.number = 1;

        /* Restore the charges */
        o_ptr->pval++;

        /* Unstack the used item */
        o_ptr->number--;
        inventory = store_item_to_inventory(player_ptr, &item);
        msg_format(_("杖をまとめなおした。", "You unstack your staff."));
    }

    if (inventory >= 0) {
        inven_item_charges(player_ptr->inventory_list[inventory]);
    } else {
        floor_item_charges(player_ptr->current_floor_ptr, 0 - inventory);
    }

    player_ptr->window_flags |= PW_INVENTORY | PW_EQUIPMENT;
    return true;
}

/*!
 * @brief 実際にアイテムを食おうとするコマンドのサブルーチン
 * @param item 食べるオブジェクトの所持品ID
 */
void exe_eat_food(PlayerType *player_ptr, INVENTORY_IDX item)
{
    if (music_singing_any(player_ptr)) {
        stop_singing(player_ptr);
    }

    SpellHex spell_hex(player_ptr);
    if (spell_hex.is_spelling_any()) {
        (void)spell_hex.stop_all_spells();
    }

    auto *o_ptr = ref_item(player_ptr, item);

    sound(SOUND_EAT);

    PlayerEnergy(player_ptr).set_player_turn_energy(100);
    const auto lev = o_ptr->get_baseitem().level;

    /* 基本食い物でないものを喰う判定 */
    bool ate = false;
    ate = exe_eat_soul(player_ptr, o_ptr);
    if (!ate) {
        ate = exe_eat_corpse_type_object(player_ptr, o_ptr);
        ate = exe_eat_junk_type_object(player_ptr, o_ptr);
    }

    /* Identity not known yet */
    const auto &bi_key = o_ptr->bi_key;
    const auto ident = exe_eat_food_type_object(player_ptr, bi_key);

    /*
     * Store what may have to be updated for the inventory (including
     * autodestroy if set by something else).  Then turn off those flags
     * so that updates triggered by calling gain_exp() or set_food() below
     * do not rearrange the inventory before the food item is destroyed in
     * the pack.
     */
    BIT_FLAGS inventory_flags = (PU_COMBINATION | PU_REORDER | (player_ptr->update & PU_AUTO_DESTRUCTION));
    player_ptr->update &= ~(PU_COMBINATION | PU_REORDER | PU_AUTO_DESTRUCTION);

    if (!(o_ptr->is_aware())) {
        chg_virtue(player_ptr, Virtue::KNOWLEDGE, -1);
        chg_virtue(player_ptr, Virtue::PATIENCE, -1);
        chg_virtue(player_ptr, Virtue::CHANCE, 1);
    }

    /* We have tried it */
    const auto tval = bi_key.tval();
    if (tval == ItemKindType::FOOD) {
        object_tried(o_ptr);
    }

    /* The player is now aware of the object */
    if (ident && !o_ptr->is_aware()) {
        object_aware(player_ptr, o_ptr);
        gain_exp(player_ptr, (lev + (player_ptr->lev >> 1)) / player_ptr->lev);
    }

    player_ptr->window_flags |= (PW_INVENTORY | PW_EQUIPMENT | PW_PLAYER);

    /* Undeads drain recharge of magic device */
    if (exe_eat_charge_of_magic_device(player_ptr, o_ptr, item)) {
        player_ptr->update |= inventory_flags;
        return;
    }

    auto food_type = PlayerRace(player_ptr).food();

    /* Balrogs change humanoid corpses to energy */
    const auto corpse_r_idx = i2enum<MonsterRaceId>(o_ptr->pval);
    const auto search = angband_strchr("pht", monraces_info[corpse_r_idx].d_char);
    if (food_type == PlayerRaceFoodType::CORPSE && (bi_key == BaseitemKey(ItemKindType::CORPSE, SV_CORPSE)) && (search != nullptr)) {
        const auto item_name = describe_flavor(player_ptr, o_ptr, (OD_OMIT_PREFIX | OD_NAME_ONLY));
        msg_format(_("%sは燃え上り灰になった。精力を吸収した気がする。", "%s^ is burnt to ashes.  You absorb its vitality!"), item_name.data());
        (void)set_food(player_ptr, PY_FOOD_MAX - 1);

        player_ptr->update |= inventory_flags;
        vary_item(player_ptr, item, -1);
        return;
    }

    if (PlayerRace(player_ptr).equals(PlayerRaceType::SKELETON)) {
        const auto sval = bi_key.sval();
        if ((sval != SV_FOOD_WAYBREAD) && (sval >= SV_FOOD_BISCUIT)) {
            ItemEntity forge;
            auto *q_ptr = &forge;

            msg_print(_("食べ物がアゴを素通りして落ちた！", "The food falls through your jaws!"));
            q_ptr->prep(lookup_baseitem_id(bi_key));

            /* Drop the object from heaven */
            (void)drop_near(player_ptr, q_ptr, -1, player_ptr->y, player_ptr->x);

            ate = true;
        } else {
            msg_print(_("食べ物がアゴを素通りして落ち、消えた！", "The food falls through your jaws and vanishes!"));
            ate = true;
        }
    } else if (food_type == PlayerRaceFoodType::BLOOD) {
        /* Vampires are filled only by bloods, so reduced nutritional benefit */
        (void)set_food(player_ptr, player_ptr->food + (o_ptr->pval / 10));
        msg_print(_("あなたのような者にとって食糧など僅かな栄養にしかならない。", "Mere victuals hold scant sustenance for a being such as yourself."));

        if (player_ptr->food < PY_FOOD_ALERT) {
            /* Hungry */
            msg_print(_("あなたの飢えは新鮮な血によってのみ満たされる！", "Your hunger can only be satisfied with fresh blood!"));
        }
        ate = true;

    } else if (food_type == PlayerRaceFoodType::WATER) {
        msg_print(_("動物の食物はあなたにとってほとんど栄養にならない。", "The food of animals is poor sustenance for you."));
        set_food(player_ptr, player_ptr->food + ((o_ptr->pval) / 20));
        ate = true;
    } else if (food_type != PlayerRaceFoodType::RATION) {
        msg_print(_("生者の食物はあなたにとってほとんど栄養にならない。", "The food of mortals is poor sustenance for you."));
        set_food(player_ptr, player_ptr->food + ((o_ptr->pval) / 20));
        ate = true;
    } else {
        if (bi_key == BaseitemKey(ItemKindType::FOOD, SV_FOOD_WAYBREAD)) {
            /* Waybread is always fully satisfying. */
            set_food(player_ptr, std::max<short>(player_ptr->food, PY_FOOD_MAX - 1));
        } else {
            /* Food can feed the player */
            (void)set_food(player_ptr, player_ptr->food + o_ptr->pval);
        }
        ate = true;
    }
    if (!ate) {
        msg_print("流石に食べるのを躊躇した。");
        return;
    }

    player_ptr->plus_incident(INCIDENT::EAT, 1);

    player_ptr->update |= inventory_flags;
    vary_item(player_ptr, item, -1);
}

/*!
 * @brief 食料を食べるコマンドのメインルーチン /
 * Eat some food (from the pack or floor)
 */
void do_cmd_eat_food(PlayerType *player_ptr)
{
    OBJECT_IDX item;
    concptr q, s;

    PlayerClass(player_ptr).break_samurai_stance({ SamuraiStanceType::MUSOU, SamuraiStanceType::KOUKIJIN });

    q = _("どれを食べますか? ", "Eat which item? ");
    s = _("食べ物がない。", "You have nothing to eat.");

    if (!choose_object(player_ptr, &item, q, s, (USE_INVEN | USE_FLOOR))) {
        return;
    }

    exe_eat_food(player_ptr, item);
}
