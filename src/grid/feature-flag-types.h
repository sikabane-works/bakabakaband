﻿#pragma once

#include "util/enum-converter.h"

/*!
 * @brief Feature flags - should be used instead of feature indexes unless generating.
 * Originally from UnAngband, and modified into TR-like style in Hengband
 */
enum class TerrainCharacteristics {
    LOS = 0, /*!< 視界が通る地形である */
    PROJECT = 1, /*!< 飛び道具が通過できる地形である */
    MOVE = 2, /*!< 移動可能な地形である */
    PLACE = 3, /*!< モンスター配置をしても良い地形である(cave_empty_bold/cave_empty_gridで利用) */
    DROP = 4, /*!< アイテムを落としてよい地形である */
    SECRET = 5, /*!< 隠し扉やトラップが潜んでいる地形である */
    NOTICE = 6, /*!< 何か興味を引くものがある地形である(シフトキー＋方向で走行中の時に止まる基準) */
    REMEMBER = 7, /*!< 常に記憶対象となる地形である(記憶喪失時に忘れたりしなくなる) */
    OPEN = 8, /*!< 開けるコマンドの対象となる地形である */
    CLOSE = 9, /*!< 閉じるコマンドの対象となる地形である */
    BASH = 10, /*!< 体当たりコマンドの対象となる地形である */
    SPIKE = 11, /*!< くさびを打つコマンドの対象となる地形である */
    DISARM = 12, /*!< 解除コマンドの対象となる地形である */
    STORE = 13, /*!< 店舗の入口となる地形である */
    TUNNEL = 14, /*!< 魔王変化などで掘り進められる地形である */
    MAY_HAVE_GOLD = 15, /*!< 何か財宝を隠した可能性のある地形である？(TerrainDefinitionsに使用している地形なし) */
    HAS_GOLD = 16, /*!< 財宝を含んだ地形である */
    HAS_ITEM = 17, /*!< アイテムを含んだ地形である */
    DOOR = 18, /*!< ドアのある地形である */
    TRAP = 19, /*!< トラップのある地形である */
    STAIRS = 20, /*!< 階段のある地形である */
    RUNE_PROTECTION = 21, /*!< 守りのルーンが張られた地形である */
    LESS = 22, /*!< 階上に通じる地形である */
    MORE = 23, /*!< 階下に通じる地形である */
    AVOID_RUN = 24, /*!< 自動移動機能時に障害として迂回すべき地形である */
    FLOOR = 25, /*!< 床のある地形である */
    WALL = 26, /*!< 壁のある地形である */
    PERMANENT = 27, /*!< 絶対に破壊できない永久地形である */
    CHAOS_TAINTED = 28, /*!< カオスに汚染されている */
    VOID = 29, /*!< 虚空である */
    HIT_TRAP = 31, /*!< トラップのある地形である(TRAPと常に重複している？) */
    GLOW = 37, /*!< 常に光っている地形である */
    ENSECRET = 38, /*!< 不明(TerrainDefinitions上で利用している地形がない) */
    WATER = 39, /*!< 水のある地形である */
    LAVA = 40, /*!< 溶岩のある地形である */
    SHALLOW = 41, /*!< 浅い地形である */
    DEEP = 42, /*!< 深い地形である */
    POISON_PUDDLE = 43, /*!< 毒溜まりがある */
    HURT_ROCK = 44, /*!< 岩石溶解の対象となる地形である */
    COLD_PUDDLE = 48, /*!< 冷気溜まりがある */
    ACID_PUDDLE = 49, /*!< 酸溜まりがある */
    ELEC_PUDDLE = 51, /*!< 接地部が帯電している */
    CAN_FLY = 53, /*!< 飛行可能な地形である */
    CAN_SWIM = 54, /*!< 泳ぐことが可能な地形である */
    CAN_PASS = 55, /*!< 通過可能な地形である */
    CAN_DIG = 57, /*!< 掘削コマンドの対象となる地形である */
    TREE = 83, /*!< 木の生えた地形である */
    PLANT = 88, //!< 植物の生えた地形である
    SPECIAL = 96, /*!< クエストやダンジョンに関わる特別な地形である */
    HURT_DISI = 97, /*!< 分解属性の対象となる地形である */
    QUEST_ENTER = 98, /*!< クエストの入り口である */
    QUEST_EXIT = 99, /*!< クエストの出口である */
    QUEST = 100, /*!< クエストに関する地形である */
    SHAFT = 101, /*!< 坑道である。(2階層移動する階段である) */
    MOUNTAIN = 102, /*!< ダンジョンの山地形である */
    BLDG = 103, /*!< 施設の入り口である */
    RUNE_EXPLOSION = 104, /*!< 爆発のルーンのある地形である */
    PATTERN = 105, /*!< パターンのある地形である */
    TOWN = 106, /*!< 広域マップ用の街がある地形である */
    ENTRANCE = 107, /*!< 広域マップ用のダンジョンがある地形である */
    MIRROR = 108, /*!< 鏡使いの鏡が張られた地形である */
    UNPERM = 109, /*!< 破壊不能な地形である(K:フラグ向け？) */
    TELEPORTABLE = 110, /*!< テレポート先の対象となる地形である */
    CONVERT = 111, /*!< 地形生成処理中の疑似フラグ */
    GLASS = 112, /*!< ガラス製の地形である */
    DUNG_POOL = 113, /*!< 糞溜まりである */
    PLASMA = 114, /*!< プラズマに覆われている */
    RUNE_HEALING = 115, /*!< 癒しのルーンが張られた地形である */
    SLOW = 116, /*!< 減速地形である */
    THORN = 117, /*!< 棘地形である */
    TENTACLE = 118, /*!< 触手地形である */
    MAX,
};

constexpr auto FF_FLAG_MAX = enum2i(TerrainCharacteristics::MAX);