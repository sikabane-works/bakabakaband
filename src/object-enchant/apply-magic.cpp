﻿/*!
 * @brief ベースアイテムを強化する処理
 * @date 2020/06/03
 * @author Hourier
 * @todo 少し長い。要分割
 */

#include "object-enchant/apply-magic.h"
#include "artifact/fixed-art-generator.h"
#include "artifact/fixed-art-types.h"
#include "dungeon/dungeon.h"
#include "mutation/mutation-flag-types.h"
#include "object-enchant/item-apply-magic.h"
#include "object-enchant/object-curse.h"
#include "object-enchant/object-ego.h"
#include "object-enchant/others/apply-magic-amulet.h"
#include "object-enchant/others/apply-magic-lite.h"
#include "object-enchant/others/apply-magic-others.h"
#include "object-enchant/others/apply-magic-ring.h"
#include "object-enchant/protector/apply-magic-armor.h"
#include "object-enchant/protector/apply-magic-boots.h"
#include "object-enchant/protector/apply-magic-cloak.h"
#include "object-enchant/protector/apply-magic-crown.h"
#include "object-enchant/protector/apply-magic-dragon-armor.h"
#include "object-enchant/protector/apply-magic-gloves.h"
#include "object-enchant/protector/apply-magic-hard-armor.h"
#include "object-enchant/protector/apply-magic-helm.h"
#include "object-enchant/protector/apply-magic-shield.h"
#include "object-enchant/special-object-flags.h"
#include "object-enchant/tr-types.h"
#include "object-enchant/trc-types.h"
#include "object-enchant/trg-types.h"
#include "object-enchant/weapon/apply-magic-weapon.h"
#include "object/object-kind.h"
#include "player/player-status-flags.h"
#include "sv-definition/sv-armor-types.h"
#include "sv-definition/sv-protector-types.h"
#include "sv-definition/sv-weapon-types.h"
#include "system/artifact-type-definition.h"
#include "system/floor-type-definition.h"
#include "system/player-type-definition.h"
#include "util/bit-flags-calculator.h"
#include "world/world.h"

/*!
 * @brief 生成されたベースアイテムに魔法的な強化を与えるメインルーチン
 * Complete the "creation" of an object by applying "magic" to the item
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param o_ptr 強化を与えたいオブジェクトの構造体参照ポインタ
 * @param lev 生成基準階
 * @param mode 生成オプション
 * @details
 * エゴ＆アーティファクトの生成、呪い、pval強化
 */
void apply_magic_to_object(PlayerType *player_ptr, ObjectType *o_ptr, DEPTH lev, BIT_FLAGS mode)
{
    if (player_ptr->ppersonality == PERSONALITY_MUNCHKIN) {
        lev += randint0(player_ptr->lev / 2 + 10);
    }
    if (lev > MAX_DEPTH - 1) {
        lev = MAX_DEPTH - 1;
    }

    int f1 = lev + 10;
    if (f1 > d_info[player_ptr->dungeon_idx].obj_good) {
        f1 = d_info[player_ptr->dungeon_idx].obj_good;
    }

    int f2 = f1 * 2 / 3;
    if ((player_ptr->ppersonality != PERSONALITY_MUNCHKIN) && (f2 > d_info[player_ptr->dungeon_idx].obj_great)) {
        f2 = d_info[player_ptr->dungeon_idx].obj_great;
    }

    if (has_good_luck(player_ptr)) {
        f1 += 5;
        f2 += 2;
    } else if (player_ptr->muta.has(PlayerMutationType::BAD_LUCK)) {
        f1 -= 5;
        f2 -= 2;
    }

    int power = 0;
    if ((mode & AM_GOOD) || magik(f1)) {
        power = 1;
        if ((mode & AM_GREAT) || magik(f2)) {
            power = 2;
            if (mode & AM_SPECIAL) {
                power = 3;
            }
        }
    } else if (magik(f1)) {
        power = -1;
        if (magik(f2)) {
            power = -2;
        }
    }
    if (mode & AM_CURSED) {
        if (power > 0) {
            power = 0 - power;
        } else {
            power--;
        }
    }

    int rolls = 0;
    if (power >= 2) {
        rolls = 1;
    }

    if (mode & (AM_GREAT | AM_SPECIAL)) {
        rolls = 4;
    }
    if ((mode & AM_NO_FIXED_ART) || o_ptr->fixed_artifact_idx) {
        rolls = 0;
    }

    for (int i = 0; i < rolls; i++) {
        if (make_artifact(player_ptr, o_ptr)) {
            break;
        }
        if (has_good_luck(player_ptr) && one_in_(77)) {
            if (make_artifact(player_ptr, o_ptr)) {
                break;
            }
        }
    }

    if (o_ptr->is_fixed_artifact()) {
        auto *a_ptr = apply_artifact(player_ptr, o_ptr);
        a_ptr->cur_num = 1;
        if (w_ptr->character_dungeon) {
            a_ptr->floor_id = player_ptr->floor_id;
        }
        return;
    }

    // @todo ファクトリパターンで抽象化する.
    switch (o_ptr->tval) {
    case ItemKindType::DIGGING:
    case ItemKindType::HAFTED:
    case ItemKindType::BOW:
    case ItemKindType::SHOT:
    case ItemKindType::ARROW:
    case ItemKindType::BOLT:
        WeaponEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::POLEARM:
        WeaponEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::SWORD:
        // @todo いずれSwordEnchanter等作って分離する.
        WeaponEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::SHIELD:
        ShieldEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::CLOAK:
        CloakEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::HELM:
        HelmEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::CROWN:
        CrownEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::BOOTS:
        BootsEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::DRAG_ARMOR:
        DragonArmorEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::HARD_ARMOR:
        HardArmorEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::SOFT_ARMOR:
        // @todo いずれSoftArmorEnchanter等作って分離する.
        ArmorEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::GLOVES:
        GlovesEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::RING:
        RingEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::AMULET:
        AmuletEnchanter(player_ptr, o_ptr, lev, power).apply_magic();
        break;
    case ItemKindType::LITE:
        LiteEnchanter(player_ptr, o_ptr, power).apply_magic();
        break;
    default:
        OtherItemsEnchanter(player_ptr, o_ptr).apply_magic();
        break;
    }

    if (o_ptr->is_ego()) {
        apply_ego(o_ptr, lev);
        return;
    }

    if (o_ptr->k_idx) {
        auto *k_ptr = &k_info[o_ptr->k_idx];
        if (!k_info[o_ptr->k_idx].cost) {
            o_ptr->ident |= (IDENT_BROKEN);
        }

        if (k_ptr->gen_flags.has(ItemGenerationTraitType::CURSED)) {
            o_ptr->curse_flags.set(CurseTraitType::CURSED);
        }
        if (k_ptr->gen_flags.has(ItemGenerationTraitType::HEAVY_CURSE)) {
            o_ptr->curse_flags.set(CurseTraitType::HEAVY_CURSE);
        }
        if (k_ptr->gen_flags.has(ItemGenerationTraitType::PERMA_CURSE)) {
            o_ptr->curse_flags.set(CurseTraitType::PERMA_CURSE);
        }
        if (k_ptr->gen_flags.has(ItemGenerationTraitType::RANDOM_CURSE0)) {
            o_ptr->curse_flags.set(get_curse(0, o_ptr));
        }
        if (k_ptr->gen_flags.has(ItemGenerationTraitType::RANDOM_CURSE1)) {
            o_ptr->curse_flags.set(get_curse(1, o_ptr));
        }
        if (k_ptr->gen_flags.has(ItemGenerationTraitType::RANDOM_CURSE2)) {
            o_ptr->curse_flags.set(get_curse(2, o_ptr));
        }
    }
}
