﻿#pragma once

#include "system/h-type.h"
#define VERSION_NAME "Bakabakaband" /*!< バリアント名称 / Name of the version/variant */

/*!
 * @brief セーブファイル上のバージョン定義(メジャー番号) / "Savefile Version Number" for Hengband 1.1.1 and later
 * @details
 * Zang以降整合性を合わせるための疑似的処理のためFAKE_VER_MAJORは実値-FAKE_VERSION_MINUSが該当のバージョン番号となる。
 * <pre>
 * FAKE_VER_MAJOR=1,2 were reserved for ZAngband version 1.x.x/2.x.x .
 * Program Version of Bakabakaband version is
 *   "(FAKE_VER_MAJOR-10).(FAKE_VER_MINOR).(FAKE_VER_PATCH)".
 * </pre>
 */
#define FAKE_VERSION_MINUS 10
#define VIEW_VERSION_MINUS 20 /*! < 表記上のバージョンマイナス */

#define FAKE_VER_MAJOR 20 /*!< ゲームのバージョン番号定義(メジャー番号 + 20) */
#define FAKE_VER_MINOR 0 /*!< ゲームのバージョン番号定義(マイナー番号) */
#define FAKE_VER_PATCH 0 /*!< ゲームのバージョン番号定義(パッチ番号) */
#define FAKE_VER_EXTRA 17 /*!< ゲームのバージョン番号定義(エクストラ番号) */

/*!
 * @brief セーブファイルのバージョン(3.0.0から導入)
 */
constexpr u32b SAVEFILE_VERSION = 7;

/*!
 * @brief バージョンが開発版が安定版かを返す(廃止予定)
 */
#define IS_STABLE_VERSION (H_VER_MINOR % 2 == 0 && H_VER_EXTRA == 0)

/*!
 * @brief 状態がアルファ版かどうかを返す
 * @note アルファ版はエクストラ番号一定値までをアルファとし、一定まで進めて安定次第ベータ版、さらにそれも解除して無印版とする。
 */
#define IS_ALPHA_VERSION 1

/*!
 * @brief ゲームのバージョン番号定義 / "Program Version Number" of the game
 * @details
 * 本FAKE_VERSIONそのものは未使用である。Zangと整合性を合わせるための疑似的処理のためFAKE_VER_MAJORは実値-10が該当のバージョン番号となる。
 * <pre>
 * First three digits may be same as the Program Version.  But not
 * always same.  It means that newer version may preserves lower
 * compatibility with the older version.
 * For example, newer Bakabakaband 1.4.4 creates savefiles marked with
 * Savefile Version 1.4.0.0 .  It means that Bakabakaband 1.4.0 can load a
 * savefile of Bakabakaband 1.4.4 (lower compatibility!).
 * Upper compatibility is always guaranteed.
 * </pre>
 */
#define FAKE_VER_PLUS 20 //!< 偽バージョン番号としていくつ足すか
#define H_VER_MAJOR (FAKE_VER_MAJOR - FAKE_VERSION_MINUS) /*!< セーブファイル上のバージョン定義(メジャー番号) */
#define H_VER_MINOR FAKE_VER_MINOR /*!< セーブファイル上のバージョン定義(マイナー番号) */
#define H_VER_PATCH FAKE_VER_PATCH /*!< セーブファイル上のバージョン定義(パッチ番号) */
#define H_VER_EXTRA FAKE_VER_EXTRA /*!< セーブファイル上のバージョン定義(エクストラ番号) */


void put_version(char *buf);
