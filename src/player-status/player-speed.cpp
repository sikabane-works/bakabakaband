﻿#include "player-status/player-speed.h"
#include "artifact/fixed-art-types.h"
#include "grid/feature-flag-types.h"
#include "grid/feature.h"
#include "inventory/inventory-slot-types.h"
#include "monster-race/monster-race.h"
#include "monster/monster-status.h"
#include "mutation/mutation-flag-types.h"
#include "object-enchant/tr-types.h"
#include "object/object-flags.h"
#include "player-base/player-race.h"
#include "player-info/equipment-info.h"
#include "player-info/race-info.h"
#include "player/attack-defense-types.h"
#include "player/digestion-processor.h"
#include "player/player-skill.h"
#include "player/player-status-flags.h"
#include "player/player-status.h"
#include "player/special-defense-types.h"
#include "realm/realm-hex-numbers.h"
#include "realm/realm-types.h"
#include "spell-realm/spells-hex.h"
#include "system/floor-type-definition.h"
#include "system/grid-type-definition.h"
#include "system/monster-race-definition.h"
#include "system/monster-type-definition.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "util/bit-flags-calculator.h"

/*!
 * @brief 速度 - 初期値、下限、上限
 * @details
 * * 初期値110 - 加速+0に相当
 * * 上限209 - 加速+99相当
 * * 下限11 - 加速-99相当
 */
void PlayerSpeed::set_locals()
{
    this->default_value = 110;
    this->min_value = 11;
    this->max_value = 209;
    this->tr_flag = TR_SPEED;
    this->tr_bad_flag = TR_SPEED;
}

/*!
 * @brief 速度計算 - 種族
 * @return 速度値の増減分
 * @details
 * ** クラッコンと妖精に加算(+レベル/10)
 * ** 悪魔変化/吸血鬼変化で加算(+3)
 * ** 魔王変化で加算(+5)
 * ** マーフォークがFF_WATER地形にいれば加算(+2+レベル/10)
 * ** そうでなく浮遊を持っていないなら減算(-2)
 */
int16_t PlayerSpeed::race_value()
{
    int16_t result = 0;
    floor_type *floor_ptr = this->player_ptr->current_floor_ptr;
    feature_type *f_ptr = &f_info[floor_ptr->grid_array[this->player_ptr->y][this->player_ptr->x].feat];

    if (PlayerRace(this->player_ptr).equals(player_race_type::KLACKON) || PlayerRace(this->player_ptr).equals(player_race_type::SPRITE))
        result += (this->player_ptr->lev) / 10;

    if (PlayerRace(this->player_ptr).equals(player_race_type::MERFOLK)) {
        floor_type *floor_ptr = this->player_ptr->current_floor_ptr;
        feature_type *f_ptr = &f_info[floor_ptr->grid_array[this->player_ptr->y][this->player_ptr->x].feat];
        if (f_ptr->flags.has(FF::WATER)) {
            result += (2 + this->player_ptr->lev / 10);
        } else if (!this->player_ptr->levitation) {
            result -= 2;
        }
    }

    if (f_ptr->flags.has(FF::SLOW)) {
        result -= 5;
    }


    if (this->player_ptr->mimic_form) {
        switch (this->player_ptr->mimic_form) {
        case MIMIC_DEMON:
            result += 3;
            break;
        case MIMIC_DEMON_LORD:
            result += 5;
            break;
        case MIMIC_VAMPIRE:
            result += 3;
            break;
        }
    }
    return result;
}

/*!
 * @brief 速度計算 - 職業
 * @return 速度値の増減分
 * @details
 * ** 忍者の装備が重ければ減算(-レベル/10)
 * ** 忍者の装備が適正ならば加算(+3)さらにクラッコン、妖精、いかさま以外なら加算(+レベル/10)
 * ** 錬気術師で装備が重くなくクラッコン、妖精、いかさま以外なら加算(+レベル/10)
 * ** 狂戦士なら加算(+3),レベル20/30/40/50ごとに+1
 */
int16_t PlayerSpeed::class_value()
{
    SPEED result = 0;

    if (this->player_ptr->pclass == CLASS_NINJA) {
        if (heavy_armor(this->player_ptr)) {
            result -= (this->player_ptr->lev) / 10;
        } else if ((!this->player_ptr->inventory_list[INVEN_MAIN_HAND].k_idx || can_attack_with_main_hand(this->player_ptr))
            && (!this->player_ptr->inventory_list[INVEN_SUB_HAND].k_idx || can_attack_with_sub_hand(this->player_ptr))) {
            result += 3;
            if (!(PlayerRace(this->player_ptr).equals(player_race_type::KLACKON) || PlayerRace(this->player_ptr).equals(player_race_type::SPRITE)
                    || (this->player_ptr->ppersonality == PERSONALITY_MUNCHKIN)))
                result += (this->player_ptr->lev) / 10;
        }
    }

    if ((this->player_ptr->pclass == CLASS_MONK || this->player_ptr->pclass == CLASS_FORCETRAINER) && !(heavy_armor(this->player_ptr))) {
        if (!(PlayerRace(this->player_ptr).equals(player_race_type::KLACKON) || PlayerRace(this->player_ptr).equals(player_race_type::SPRITE)
                || (this->player_ptr->ppersonality == PERSONALITY_MUNCHKIN)))
            result += (this->player_ptr->lev) / 10;
    }

    if (this->player_ptr->pclass == CLASS_BERSERKER) {
        result += 2;
        if (this->player_ptr->lev > 29)
            result++;
        if (this->player_ptr->lev > 39)
            result++;
        if (this->player_ptr->lev > 44)
            result++;
        if (this->player_ptr->lev > 49)
            result++;
    }
    return result;
}

/*!
 * @brief 速度計算 - 性格
 * @return 速度値の増減分
 * @details
 * ** いかさまでクラッコン/妖精以外なら加算(+5+レベル/10)
 */
int16_t PlayerSpeed::personality_value()
{
    int16_t result = 0;
    if (this->player_ptr->ppersonality == PERSONALITY_MUNCHKIN && this->player_ptr->prace != player_race_type::KLACKON
        && this->player_ptr->prace != player_race_type::SPRITE) {
        result += (this->player_ptr->lev) / 10 + 5;
    }
    return result;
}

/*!
 * @brief 速度計算 - 装備品特殊セット
 * @return 速度値の増減分
 * @details
 * ** 棘セット装備中ならば加算(+7)
 * ** アイシングデス-トゥインクル装備中ならば加算(+7)
 */
int16_t PlayerSpeed::special_weapon_set_value()
{
    int16_t result = 0;
    if (has_melee_weapon(this->player_ptr, INVEN_MAIN_HAND) && has_melee_weapon(this->player_ptr, INVEN_SUB_HAND)) {
        if ((this->player_ptr->inventory_list[INVEN_MAIN_HAND].name1 == ART_QUICKTHORN)
            && (this->player_ptr->inventory_list[INVEN_SUB_HAND].name1 == ART_TINYTHORN)) {
            result += 7;
        }

        if ((this->player_ptr->inventory_list[INVEN_MAIN_HAND].name1 == ART_ICINGDEATH)
            && (this->player_ptr->inventory_list[INVEN_SUB_HAND].name1 == ART_TWINKLE)) {
            result += 5;
        }
    }
    return result;
}

/*!
 * @brief 速度計算 - 装備品
 * @return 速度値の増減分
 * @details
 * ** 装備品にTR_SPEEDがあれば加算(+pval+1
 * ** セットで加速増減があるものを計算
 */
int16_t PlayerSpeed::equipments_value()
{
    int16_t result = PlayerStatusBase::equipments_value();
    result += this->special_weapon_set_value();

    return result;
}

/*!
 * @brief 速度計算 - 一時的効果
 * @return 速度値の増減分
 * @details
 * ** 加速状態中なら加算(+10)
 * ** 減速状態中なら減算(-10)
 * ** 呪術「衝撃のクローク」で加算(+3)
 * ** 食い過ぎなら減算(-10)
 * ** 光速移動中は+999(最終的に+99になる)
 */
int16_t PlayerSpeed::time_effect_value()
{
    int16_t result = 0;

    if (is_fast(this->player_ptr)) {
        result += 10;
    }

    if (this->player_ptr->slow) {
        result -= 10;
    }

    if (this->player_ptr->realm1 == REALM_HEX) {
        if (SpellHex(this->player_ptr).is_spelling_specific(HEX_SHOCK_CLOAK)) {
            result += 3;
        }
    }

    if (this->player_ptr->food >= PY_FOOD_MAX)
        result -= 10;

    /* Temporary lightspeed forces to be maximum speed */
    if (this->player_ptr->lightspeed)
        result += 999;

    return result;
}

/*!
 * @brief 速度計算 - 型
 * @return 速度値の増減分
 * @details
 * ** 朱雀の構えなら加算(+10)
 */
int16_t PlayerSpeed::battleform_value()
{
    int16_t result = 0;
    if (any_bits(this->player_ptr->special_defense, KAMAE_SUZAKU))
        result += 10;
    return result;
}

/*!
 * @brief 速度計算 - 変異
 * @return 速度値の増減分
 * @details
 * ** 変異MUT3_XTRA_FATなら減算(-2)
 * ** 変異MUT3_XTRA_LEGなら加算(+3)
 * ** 変異MUT3_SHORT_LEGなら減算(-3)
 */
int16_t PlayerSpeed::mutation_value()
{
    SPEED result = 0;

    const auto &muta = this->player_ptr->muta;
    if (muta.has(MUTA::XTRA_FAT)) {
        result -= 2;
    }

    if (muta.has(MUTA::XTRA_LEGS)) {
        result += 3;
    }

    if (muta.has(MUTA::SHORT_LEG)) {
        result -= 3;
    }

    return result;
}

/*!
 * @brief 速度計算 - 乗馬
 * @return 速度値の増減分
 * @details
 * * 騎乗中ならばモンスターの加速に準拠、ただし騎乗技能値とモンスターレベルによるキャップ処理あり
 */
int16_t PlayerSpeed::riding_value()
{
    monster_type *riding_m_ptr = &(this->player_ptr)->current_floor_ptr->m_list[this->player_ptr->riding];
    SPEED speed = riding_m_ptr->mspeed;
    SPEED result = 0;

    if (!this->player_ptr->riding) {
        return 0;
    }

    if (riding_m_ptr->mspeed > 110) {
        result = (int16_t)((speed - 110) * (this->player_ptr->skill_exp[SKILL_RIDING] * 3 + this->player_ptr->lev * 160L - 10000L) / (22000L));
        if (result < 0)
            result = 0;
    } else {
        result = speed - 110;
    }

    result += (this->player_ptr->skill_exp[SKILL_RIDING] + this->player_ptr->lev * 160L) / 3200;

    if (monster_fast_remaining(riding_m_ptr))
        result += 10;
    if (monster_slow_remaining(riding_m_ptr))
        result -= 10;

    return result;
}

/*!
 * @brief 速度計算 - 重量
 * @return 速度値の増減分
 * @details
 * * 所持品の重量による減速処理。乗馬時は別計算。
 */
int16_t PlayerSpeed::inventory_weight_value()
{
    auto result = 0;
    auto weight = calc_inventory_weight(this->player_ptr);
    if (this->player_ptr->riding) {
        auto *riding_m_ptr = &(this->player_ptr)->current_floor_ptr->m_list[this->player_ptr->riding];
        auto *riding_r_ptr = &r_info[riding_m_ptr->r_idx];
        auto count = 1500 + riding_r_ptr->level * 25;
        if (weight > count) {
            result -= ((weight - count) / (count / 5));
        }
    } else {
        auto count = calc_weight_limit(this->player_ptr);
        if (weight > count) {
            result -= ((weight - count) / (count / 5));
        }
    }

    return result;
}

/*!
 * @brief 速度計算 - ACTION
 * @return 速度値の増減分
 * @details
 * * 探索中なら減算(-10)
 */
int16_t PlayerSpeed::action_value()
{
    SPEED result = 0;
    if (this->player_ptr->action == ACTION_SEARCH)
        result -= 10;
    return result;
}

/*!
 * @brief 速度フラグ - 装備
 * @return 加速修正が0でない装備に対応したBIT_FLAG
 * @details
 * * セット二刀流は両手のフラグをONにする
 */
BIT_FLAGS PlayerSpeed::equipments_flags(tr_type check_flag)
{
    BIT_FLAGS result = PlayerStatusBase::equipments_flags(check_flag);

    if (this->special_weapon_set_value() != 0)
        set_bits(result, FLAG_CAUSE_INVEN_MAIN_HAND | FLAG_CAUSE_INVEN_SUB_HAND);

    return result;
}

/*!
 * @brief 速度計算 - 乗馬時の例外処理
 * @return 計算済の速度値
 * @details
 * * 非乗馬時 - ここまでの修正値合算をそのまま使用
 * * 乗馬時 - 乗馬の速度と重量減衰のみを計算
 */
int16_t PlayerSpeed::set_exception_value(int16_t value)
{
    if (this->player_ptr->riding) {
        value = this->default_value;
        value += this->riding_value();
        value += this->inventory_weight_value();
        value += this->action_value();
    }
    return value;
}
