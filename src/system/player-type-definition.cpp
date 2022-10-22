﻿#include "system/player-type-definition.h"
#include "system/floor-type-definition.h"
#include "market/arena-info-table.h"
#include "timed-effect/player-confusion.h"
#include "timed-effect/player-cut.h"
#include "timed-effect/player-hallucination.h"
#include "timed-effect/player-paralysis.h"
#include "timed-effect/player-stun.h"
#include "timed-effect/timed-effects.h"
#include "world/world.h"

/*!
 * @brief プレイヤー構造体実体 / Static player info record
 */
PlayerType p_body;

/*!
 * @brief プレイヤー構造体へのグローバル参照ポインタ / Pointer to the player info
 */
PlayerType *p_ptr = &p_body;

PlayerType::PlayerType()
    : timed_effects(std::make_shared<TimedEffects>())
{
}

bool PlayerType::is_true_winner() const
{
    return (w_ptr->total_winner > 0) && (this->arena_number > MAX_ARENA_MONS + 2);
}

std::shared_ptr<TimedEffects> PlayerType::effects() const
{
    return this->timed_effects;
}

bool PlayerType::is_vaild_position() const
{
    floor_type *floor_ptr = this->current_floor_ptr;
    return this->x > 0 && this->y > 0 && this->x <= floor_ptr->width - 1 && this->y <= floor_ptr->height - 1;
}


/*!
 * @brief 自身の状態が全快で、かつフロアに影響を与えないかを検証する
 * @return 上記の通りか
 * @todo 時限効果系に分類されるものはいずれTimedEffectsクラスのメソッドとして繰り込みたい
 */
bool PlayerType::is_fully_healthy() const
{
    auto effects = this->effects();
    auto is_fully_healthy = this->chp == this->mhp;
    is_fully_healthy &= this->csp >= this->msp;
    is_fully_healthy &= !this->blind;
    is_fully_healthy &= !effects->confusion()->is_confused();
    is_fully_healthy &= !this->poisoned;
    is_fully_healthy &= !this->afraid;
    is_fully_healthy &= !effects->stun()->is_stunned();
    is_fully_healthy &= !effects->cut()->is_cut();
    is_fully_healthy &= !this->slow;
    is_fully_healthy &= !effects->paralysis()->is_paralyzed();
    is_fully_healthy &= !effects->hallucination()->is_hallucinated();
    is_fully_healthy &= !this->word_recall;
    is_fully_healthy &= !this->alter_reality;
    return is_fully_healthy;
}
