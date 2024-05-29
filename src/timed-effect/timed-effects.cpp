/*!
 * @brief プレイヤーの時限効果を表すオブジェクト群を保持する
 * @date 2022/08/05
 * @author Hourier
 */

#include "timed-effect/timed-effects.h"
#include "timed-effect/player-acceleration.h"
#include "timed-effect/player-deceleration.h"
#include "timed-effect/player-poison.h"

TimedEffects::TimedEffects()
    : player_acceleration(std::make_shared<PlayerAcceleration>())
    , player_deceleration(std::make_shared<PlayerDeceleration>())
    , player_poison(std::make_shared<PlayerPoison>())
{
}

PlayerBlindness &TimedEffects::blindness()
{
    return this->player_blindness;
}

const PlayerBlindness &TimedEffects::blindness() const
{
    return this->player_blindness;
}

PlayerConfusion &TimedEffects::confusion()
{
    return this->player_confusion;
}

const PlayerConfusion &TimedEffects::confusion() const
{
    return this->player_confusion;
}

PlayerCut &TimedEffects::cut()
{
    return this->player_cut;
}

const PlayerCut &TimedEffects::cut() const
{
    return this->player_cut;
}

PlayerFear &TimedEffects::fear()
{
    return this->player_fear;
}

const PlayerFear &TimedEffects::fear() const
{
    return this->player_fear;
}

PlayerHallucination &TimedEffects::hallucination()
{
    return this->player_hallucination;
}

const PlayerHallucination &TimedEffects::hallucination() const
{
    return this->player_hallucination;
}

PlayerParalysis &TimedEffects::paralysis()
{
    return this->player_paralysis;
}

const PlayerParalysis &TimedEffects::paralysis() const
{
    return this->player_paralysis;
}

PlayerStun &TimedEffects::stun()
{
    return this->player_stun;
}

const PlayerStun &TimedEffects::stun() const
{
    return this->player_stun;
}

std::shared_ptr<PlayerAcceleration> TimedEffects::acceleration() const
{
    return this->player_acceleration;
}

std::shared_ptr<PlayerDeceleration> TimedEffects::deceleration() const
{
    return this->player_deceleration;
}

std::shared_ptr<PlayerPoison> TimedEffects::poison() const
{
    return this->player_poison;
}
