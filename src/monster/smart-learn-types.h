﻿#pragma once

enum class MonsterSmartLearnType {
    RES_ACID = 0, /*!< モンスターの学習フラグ: プレイヤーに酸耐性あり */
    RES_ELEC = 1, /*!< モンスターの学習フラグ: プレイヤーに電撃耐性あり */
    RES_FIRE = 2, /*!< モンスターの学習フラグ: プレイヤーに火炎耐性あり */
    RES_COLD = 3, /*!< モンスターの学習フラグ: プレイヤーに冷気耐性あり */
    RES_POIS = 4, /*!< モンスターの学習フラグ: プレイヤーに毒耐性あり */
    RES_NETH = 5, /*!< モンスターの学習フラグ: プレイヤーに地獄耐性あり */
    RES_LITE = 6, /*!< モンスターの学習フラグ: プレイヤーに閃光耐性あり */
    RES_DARK = 7, /*!< モンスターの学習フラグ: プレイヤーに暗黒耐性あり */
    RES_FEAR = 8, /*!< モンスターの学習フラグ: プレイヤーに恐怖耐性あり */
    RES_CONF = 9, /*!< モンスターの学習フラグ: プレイヤーに混乱耐性あり */
    RES_CHAOS = 10, /*!< モンスターの学習フラグ: プレイヤーにカオス耐性あり */
    RES_DISEN = 11, /*!< モンスターの学習フラグ: プレイヤーに劣化耐性あり */
    RES_BLIND = 12, /*!< モンスターの学習フラグ: プレイヤーに盲目耐性あり */
    RES_NEXUS = 13, /*!< モンスターの学習フラグ: プレイヤーに因果混乱耐性あり */
    RES_SOUND = 14, /*!< モンスターの学習フラグ: プレイヤーに轟音耐性あり */
    RES_SHARD = 15, /*!< モンスターの学習フラグ: プレイヤーに破片耐性あり */
    OPP_ACID = 16, /*!< モンスターの学習フラグ: プレイヤーに二重酸耐性あり */
    OPP_ELEC = 17, /*!< モンスターの学習フラグ: プレイヤーに二重電撃耐性あり */
    OPP_FIRE = 18, /*!< モンスターの学習フラグ: プレイヤーに二重火炎耐性あり */
    OPP_COLD = 19, /*!< モンスターの学習フラグ: プレイヤーに二重冷気耐性あり */
    OPP_POIS = 20, /*!< モンスターの学習フラグ: プレイヤーに二重毒耐性あり */
    UNUSED_21 = 21, /*!< 未使用 / (unused) */
    UNUSED_22 = 22, /*!< 未使用 / (unused) */
    UNUSED_23 = 23, /*!< 未使用 / (unused) */
    IMM_ACID = 24, /*!< モンスターの学習フラグ: プレイヤーに酸免疫あり */
    IMM_ELEC = 25, /*!< モンスターの学習フラグ: プレイヤーに電撃免疫あり */
    IMM_FIRE = 26, /*!< モンスターの学習フラグ: プレイヤーに火炎免疫あり */
    IMM_COLD = 27, /*!< モンスターの学習フラグ: プレイヤーに冷気免疫あり */
    UNUSED_28 = 28, /*!< 未使用 / (unused) */
    IMM_REFLECT = 29, /*!< モンスターの学習フラグ: プレイヤーに反射あり */
    IMM_FREE = 30, /*!< モンスターの学習フラグ: プレイヤーに麻痺耐性あり */
    IMM_MANA = 31, /*!< モンスターの学習フラグ: プレイヤーにMPがない */
    MAX,
};