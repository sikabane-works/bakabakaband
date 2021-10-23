#pragma once
/*!
 * @file game-data-initializer.h
 * @brief 馬鹿馬鹿蛮怒のゲームデータ初期化ヘッダファイル
 */

#include "system/angband.h"

class player_type;
void init_quests(void);
void init_other(player_type *player_ptr);
void init_alloc(void);
