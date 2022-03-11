﻿/*!
 * @brief 剣・鈍器・戦斧に耐性等の追加効果を付与する処理
 * @date 2022/03/11
 * @author Hourier
 */

#include "object-enchant/weapon/apply-magic-sword.h"
#include "artifact/random-art-generator.h"
#include "floor/floor-base-definitions.h"
#include "inventory/inventory-slot-types.h"
#include "object-enchant/object-boost.h"
#include "object/tval-types.h"
#include "sv-definition/sv-weapon-types.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "view/display-messages.h"

/*!
 * @brief 剣・鈍器・戦斧強化クラスのコンストラクタ
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param o_ptr 強化を与えたいオブジェクトの構造体参照ポインタ
 * @param level 生成基準階
 * @param power 生成ランク
 */
SwordEnchanter::SwordEnchanter(PlayerType *player_ptr, ObjectType *o_ptr, DEPTH level, int power)
    : AbstractWeaponEnchanter(o_ptr, level, power)
    , player_ptr(player_ptr)
{
}

void SwordEnchanter::apply_magic()
{
    if (this->should_skip) {
        return;
    }

    if (this->power > 1) {
        if ((this->power > 2) || one_in_(40)) {
            become_random_artifact(this->player_ptr, this->o_ptr, false);
            return;
        }

        while (true) {
            this->o_ptr->ego_idx = get_random_ego(INVEN_MAIN_HAND, true);
            if (this->o_ptr->ego_idx == EgoType::SHARPNESS && this->o_ptr->tval != ItemKindType::SWORD) {
                continue;
            }

            if (this->o_ptr->ego_idx == EgoType::EARTHQUAKES && this->o_ptr->tval != ItemKindType::HAFTED) {
                continue;
            }

            break;
        }

        switch (this->o_ptr->ego_idx) {
        case EgoType::SHARPNESS:
            this->o_ptr->pval = (PARAMETER_VALUE)m_bonus(5, this->level) + 1;
            break;
        case EgoType::EARTHQUAKES:
            if (one_in_(3) && (this->level > 60)) {
                this->o_ptr->art_flags.set(TR_BLOWS);
            } else {
                this->o_ptr->pval = (PARAMETER_VALUE)m_bonus(3, this->level);
            }

            break;
        default:
            break;
        }

        if (!this->o_ptr->art_name) {
            while (one_in_(10L * this->o_ptr->dd * this->o_ptr->ds)) {
                this->o_ptr->dd++;
            }

            if (this->o_ptr->dd > 9) {
                this->o_ptr->dd = 9;
            }
        }

        return;
    }

    if (this->power < -1) {
        if (randint0(MAX_DEPTH) < this->level) {
            auto n = 0;
            while (true) {
                this->o_ptr->ego_idx = get_random_ego(INVEN_MAIN_HAND, false);
                if (this->o_ptr->ego_idx == EgoType::WEIRD && this->o_ptr->tval != ItemKindType::SWORD) {
                    continue;
                }

                auto *e_ptr = &e_info[this->o_ptr->ego_idx];
                if (this->o_ptr->tval == ItemKindType::SWORD && this->o_ptr->sval == SV_HAYABUSA && e_ptr->max_pval < 0) {
                    if (++n > 1000) {
                        msg_print(_("エラー:隼の剣に割り当てるエゴ無し", "Error: Cannot find for Hayabusa."));
                        return;
                    }

                    continue;
                }

                break;
            }
        }
    }
}
