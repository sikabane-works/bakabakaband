﻿#pragma once

/*!
 * @brief ダンジョンの最深層 / Maximum dungeon level.
 * @details
 * The player can never reach this level
 * in the dungeon, and this value is used for various calculations
 * involving object and monster creation.  It must be at least 100.
 * Setting it below 128 may prevent the creation of some objects.
 */
#define MAX_DEPTH 128

/*!
 * @brief 基本的なブロック数単位(垂直方向)
 * Number of grids in each block (vertically) Probably hard-coded to 11
 */
#define BLOCK_HGT 11

/*!
 * @brief 基本的なブロック数単位(水平方向)
 * Number of grids in each block (horizontally) Probably hard-coded to 11
 */
#define BLOCK_WID 11

/*!
 * @brief 表示上の基本的なブロック単位(垂直方向、PANEL_HGTの倍数で設定すること)
 * Number of grids used to display the dungeon (vertically). Must be a multiple of 11, probably hard-coded to 22.
 */
#define SCREEN_HGT 11

/*!
 * @brief 表示上の基本的なブロック単位(水平方向、PANEL_WIDの倍数で設定すること)
 * Number of grids used to display the dungeon (horizontally). Must be a multiple of 33, probably hard-coded to 66.
 */
#define SCREEN_WID 11

/*!
 * @brief 表示上のダンジョンの最大垂直サイズ(SCREEN_HGTの3倍が望ましい)
 * Maximum dungeon height in grids, must be a multiple of SCREEN_HGT, probably hard-coded to SCREEN_HGT * 3.
 */
#define MAX_HGT 242

/*!
 * @brief 表示上のダンジョンの最大水平サイズ(SCREEN_WIDの3倍が望ましい)
 * Maximum dungeon width in grids, must be a multiple of SCREEN_WID, probably hard-coded to SCREEN_WID * 3.
 */
#define MAX_WID 242

/*! @brief 汎用ダンジョン最小縦倍率 */
#define MIN_HGT_MULTIPLE 4

/*! @brief 汎用ダンジョン最小横倍率 */
#define MIN_WID_MULTIPLE 4

/*! @brief クソデカダンジョン生成率(1/N) */
#define HUGE_DUNGEON_RATE 10

/*! @brief 大型ダンジョン生成率(1/N) */
#define LARGE_DUNGEON_RATE 4

/*! @brief アリーナ闘技場の横幅 */
const int ARENA_WID = 33;

/*! @brief アリーナ闘技場の縦幅 */
const int ARENA_HGT = 33;
