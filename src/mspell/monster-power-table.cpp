﻿#include "mspell/monster-power-table.h"
#include "monster-race/race-ability-flags.h"
#include "player-ability/player-ability-types.h"

/*!
 * @brief 青魔法テーブル
 * @details
 * level,  smana,  %fail,  manedam,  %manefail,  use_stat, name
 */
const std::map<MonsterAbilityType, const monster_power> monster_powers = {
    { MonsterAbilityType::SHRIEK, { 1, 1, 10, 0, 15, A_CON, _("叫ぶ", "shriek") } },
    { MonsterAbilityType::XXX1, { 10, 4, 35, 89, 40, A_INT, _("何か", "something") } },
    { MonsterAbilityType::DISPEL, { 40, 35, 85, 0, 40, A_INT, _("魔力消去", "dispel-magic") } },
    { MonsterAbilityType::ROCKET, { 35, 30, 80, 800, 70, A_STR, _("ロケット", "rocket") } },
    { MonsterAbilityType::SHOOT, { 5, 1, 20, 18, 15, A_DEX, _("射撃", "arrow") } },
    { MonsterAbilityType::XXX2, { 10, 4, 35, 89, 40, A_INT, _("何か", "arrows") } },
    { MonsterAbilityType::XXX3, { 10, 4, 35, 89, 40, A_INT, _("何か", "missile") } },
    { MonsterAbilityType::XXX4, { 10, 4, 35, 89, 40, A_INT, _("何か", "missiles") } },
    { MonsterAbilityType::BR_ACID, { 20, 15, 55, 1600, 95, A_CON, _("酸のブレス", "breathe acid") } },
    { MonsterAbilityType::BR_ELEC, { 20, 15, 55, 1600, 95, A_CON, _("電撃のブレス", "breathe lightning") } },
    { MonsterAbilityType::BR_FIRE, { 20, 15, 55, 1600, 95, A_CON, _("炎のブレス", "breathe fire") } },
    { MonsterAbilityType::BR_COLD, { 20, 15, 55, 1600, 95, A_CON, _("冷気のブレス", "breathe cold") } },
    { MonsterAbilityType::BR_POIS, { 20, 15, 55, 800, 95, A_CON, _("毒のブレス", "breathe poison") } },
    { MonsterAbilityType::BR_NETH, { 20, 15, 70, 550, 95, A_CON, _("地獄のブレス", "breathe nether") } },
    { MonsterAbilityType::BR_LITE, { 20, 16, 70, 400, 95, A_CON, _("閃光のブレス", "breathe light") } },
    { MonsterAbilityType::BR_DARK, { 20, 16, 70, 400, 95, A_CON, _("暗黒のブレス", "breathe dark") } },
    { MonsterAbilityType::BR_CONF, { 20, 20, 70, 450, 95, A_CON, _("混乱のブレス", "breathe confusion") } },
    { MonsterAbilityType::BR_SOUN, { 20, 20, 70, 450, 95, A_CON, _("轟音のブレス", "breathe sound") } },
    { MonsterAbilityType::BR_CHAO, { 20, 20, 70, 600, 95, A_CON, _("カオスのブレス", "breathe chaos") } },
    { MonsterAbilityType::BR_DISE, { 20, 16, 70, 500, 95, A_CON, _("劣化のブレス", "breathe disenchantment") } },
    { MonsterAbilityType::BR_NEXU, { 30, 25, 80, 250, 95, A_CON, _("因果混乱のブレス", "breathe nexus") } },
    { MonsterAbilityType::BR_TIME, { 35, 18, 80, 150, 95, A_CON, _("時間逆転のブレス", "breathe time") } },
    { MonsterAbilityType::BR_INER, { 30, 25, 80, 200, 95, A_CON, _("遅鈍のブレス", "breathe inertia") } },
    { MonsterAbilityType::BR_GRAV, { 30, 28, 90, 200, 95, A_CON, _("重力のブレス", "breathe gravity") } },
    { MonsterAbilityType::BR_SHAR, { 20, 15, 70, 500, 95, A_CON, _("破片のブレス", "breathe shards") } },
    { MonsterAbilityType::BR_PLAS, { 35, 15, 80, 150, 95, A_CON, _("プラズマのブレス", "breathe plasma") } },
    { MonsterAbilityType::BR_FORC, { 30, 18, 70, 200, 95, A_CON, _("フォースのブレス", "breathe force") } },
    { MonsterAbilityType::BR_MANA, { 30, 28, 80, 250, 95, A_CON, _("魔力のブレス", "breathe mana") } },
    { MonsterAbilityType::BA_NUKE, { 25, 20, 95, 320, 80, A_INT, _("放射能球", "nuke ball") } },
    { MonsterAbilityType::BR_NUKE, { 25, 15, 70, 800, 95, A_CON, _("放射性廃棄物のブレス", "breathe nuke") } },
    { MonsterAbilityType::BA_CHAO, { 30, 32, 85, 400, 80, A_INT, _("純ログルス", "raw Logrus") } },
    { MonsterAbilityType::BR_DISI, { 35, 40, 95, 150, 95, A_CON, _("分解のブレス", "breathe disintegration") } },
    { MonsterAbilityType::BR_VOID, { 40, 44, 95, 250, 95, A_CON, _("虚無のブレス", "breathe void") } },
    { MonsterAbilityType::BR_ABYSS, { 40, 44, 95, 250, 95, A_CON, _("深淵のブレス", "breathe abyss") } },

    { MonsterAbilityType::BA_ACID, { 18, 13, 55, 630, 80, A_INT, _("アシッド・ボール", "acid ball") } },
    { MonsterAbilityType::BA_ELEC, { 14, 10, 45, 316, 60, A_INT, _("サンダー・ボール", "lightning ball") } },
    { MonsterAbilityType::BA_FIRE, { 20, 14, 60, 720, 80, A_INT, _("ファイア・ボール", "fire ball") } },
    { MonsterAbilityType::BA_COLD, { 15, 11, 50, 320, 60, A_INT, _("アイス・ボール", "frost ball") } },
    { MonsterAbilityType::BA_POIS, { 5, 3, 40, 48, 20, A_INT, _("悪臭雲", "stinking cloud") } },
    { MonsterAbilityType::BA_NETH, { 25, 18, 70, 350, 80, A_INT, _("地獄球", "nether ball") } },
    { MonsterAbilityType::BA_WATE, { 30, 22, 75, 350, 80, A_INT, _("ウォーター・ボール", "water ball") } },
    { MonsterAbilityType::BA_MANA, { 44, 45, 85, 550, 95, A_INT, _("魔力の嵐", "mana storm") } },
    { MonsterAbilityType::BA_DARK, { 40, 42, 90, 550, 95, A_INT, _("暗黒の嵐", "darkness storm") } },
    { MonsterAbilityType::BA_VOID, { 38, 41, 85, 400, 90, A_INT, _("虚無の嵐", "void ball") } },
    { MonsterAbilityType::BA_ABYSS, { 39, 42, 85, 400, 90, A_INT, _("深淵の嵐", "abyss ball") } },
    { MonsterAbilityType::DRAIN_MANA, { 10, 5, 50, 0, 25, A_INT, _("魔力吸収", "drain mana") } },
    { MonsterAbilityType::MIND_BLAST, { 25, 10, 60, 0, 30, A_INT, _("精神攻撃", "mind blast") } },
    { MonsterAbilityType::BRAIN_SMASH, { 30, 14, 65, 0, 30, A_INT, _("脳攻撃", "brain smash") } },
    { MonsterAbilityType::CAUSE_1, { 3, 1, 25, 24, 20, A_INT, _("軽傷", "cause light wounds") } },
    { MonsterAbilityType::CAUSE_2, { 12, 2, 35, 64, 25, A_INT, _("重傷", "cause serious wounds") } },
    { MonsterAbilityType::CAUSE_3, { 22, 6, 50, 150, 30, A_INT, _("致命傷", "cause critical wounds") } },
    { MonsterAbilityType::CAUSE_4, { 32, 10, 70, 225, 35, A_INT, _("秘孔を突く", "cause mortal wounds") } },
    { MonsterAbilityType::BO_ACID, { 13, 7, 40, 178, 40, A_INT, _("アシッド・ボルト", "acid bolt") } },
    { MonsterAbilityType::BO_ELEC, { 10, 5, 35, 130, 35, A_INT, _("サンダー・ボルト", "lightning bolt") } },
    { MonsterAbilityType::BO_FIRE, { 15, 9, 50, 210, 45, A_INT, _("ファイア・ボルト", "fire bolt") } },
    { MonsterAbilityType::BO_COLD, { 12, 6, 35, 162, 40, A_INT, _("アイス・ボルト", "frost bolt") } },
    { MonsterAbilityType::BA_LITE, { 40, 42, 90, 550, 95, A_INT, _("スター・バースト", "starburst") } },
    { MonsterAbilityType::BO_NETH, { 25, 17, 60, 255, 60, A_INT, _("地獄の矢", "nether bolt") } },
    { MonsterAbilityType::BO_WATE, { 25, 20, 65, 250, 60, A_INT, _("ウォーター・ボルト", "water bolt") } },
    { MonsterAbilityType::BO_MANA, { 25, 24, 90, 400, 80, A_INT, _("魔力の矢", "mana bolt") } },
    { MonsterAbilityType::BO_PLAS, { 25, 20, 80, 216, 60, A_INT, _("プラズマ・ボルト", "plasma bolt") } },
    { MonsterAbilityType::BO_ICEE, { 25, 16, 60, 186, 60, A_INT, _("極寒の矢", "ice bolt") } },
    { MonsterAbilityType::BO_VOID, { 35, 31, 80, 342, 70, A_INT, _("ヴォイド・ボルト", "void bolt") } },
    { MonsterAbilityType::BO_ABYSS, { 35, 33, 80, 342, 70, A_INT, _("アビス・ボルト", "abyss bolt") } },
    { MonsterAbilityType::MISSILE, { 3, 1, 25, 12, 20, A_INT, _("マジック・ミサイル", "magic missile") } },
    { MonsterAbilityType::SCARE, { 5, 3, 35, 0, 20, A_INT, _("恐慌", "scare") } },
    { MonsterAbilityType::BLIND, { 10, 5, 40, 0, 20, A_INT, _("盲目", "blind") } },
    { MonsterAbilityType::CONF, { 10, 5, 40, 0, 20, A_INT, _("パニック・モンスター", "confuse") } },
    { MonsterAbilityType::SLOW, { 10, 5, 40, 0, 20, A_INT, _("スロウ・モンスター", "slow") } },
    { MonsterAbilityType::HOLD, { 10, 5, 40, 0, 20, A_INT, _("スリープ・モンスター", "sleep") } },

    { MonsterAbilityType::HASTE, { 20, 10, 70, 0, 40, A_INT, _("スピード", "speed") } },
    { MonsterAbilityType::HAND_DOOM, { 45, 120, 95, 0, 60, A_INT, _("破滅の手", "the Hand of Doom") } },
    { MonsterAbilityType::HEAL, { 20, 15, 70, 0, 20, A_WIS, _("体力回復", "heal-self") } },
    { MonsterAbilityType::INVULNER, { 45, 65, 80, 0, 60, A_INT, _("無傷の球", "make invulnerable") } },
    { MonsterAbilityType::BLINK, { 5, 1, 30, 0, 20, A_INT, _("ショート・テレポート", "blink-self") } },
    { MonsterAbilityType::TPORT, { 15, 8, 40, 0, 30, A_INT, _("テレポート", "teleport-self") } },
    { MonsterAbilityType::WORLD, { 40, 999, 99, 0, 80, A_INT, _("ザ・ワールド", "The world") } },
    { MonsterAbilityType::SPECIAL, { 1, 0, 0, 0, 15, A_INT, _("何か", "something") } },
    { MonsterAbilityType::TELE_TO, { 15, 8, 50, 0, 30, A_INT, _("引きよせる", "teleport to") } },
    { MonsterAbilityType::TELE_AWAY, { 20, 13, 80, 0, 30, A_INT, _("テレポート・アウェイ", "teleport away") } },
    { MonsterAbilityType::TELE_LEVEL, { 30, 40, 95, 0, 40, A_INT, _("テレポート・レベル", "teleport level") } },
    { MonsterAbilityType::PSY_SPEAR, { 35, 30, 80, 350, 70, A_INT, _("光の剣", "psycho-spear") } },
    { MonsterAbilityType::DARKNESS, { 5, 1, 20, 0, 15, A_INT, _("暗闇", "create darkness") } },
    { MonsterAbilityType::TRAPS, { 5, 1, 20, 0, 15, A_DEX, _("トラップ創造", "create traps") } },
    { MonsterAbilityType::FORGET, { 15, 3, 40, 0, 30, A_INT, _("記憶喪失", "cause amnesia") } },
    { MonsterAbilityType::RAISE_DEAD, { 30, 30, 70, 0, 40, A_INT, _("死者復活", "raise dead") } },
    { MonsterAbilityType::S_KIN, { 40, 70, 85, 0, 45, A_INT, _("援軍を呼ぶ", "summon aid") } },
    { MonsterAbilityType::S_CYBER, { 45, 90, 90, 0, 50, A_INT, _("サイバーデーモンの召喚", "summon Cyberdemons") } },
    { MonsterAbilityType::S_MONSTER, { 25, 20, 65, 0, 30, A_INT, _("モンスターの召喚", "summon a monster") } },
    { MonsterAbilityType::S_MONSTERS, { 35, 30, 75, 0, 40, A_INT, _("複数のモンスターの召喚", "summon monsters") } },
    { MonsterAbilityType::S_ANT, { 25, 25, 65, 0, 25, A_INT, _("アリの召喚", "summon ants") } },
    { MonsterAbilityType::S_SPIDER, { 25, 20, 60, 0, 25, A_INT, _("蜘蛛の召喚", "summon spiders") } },
    { MonsterAbilityType::S_HOUND, { 35, 26, 75, 0, 40, A_INT, _("ハウンドの召喚", "summon hounds") } },
    { MonsterAbilityType::S_HYDRA, { 30, 23, 70, 0, 35, A_INT, _("ヒドラの召喚", "summon hydras") } },
    { MonsterAbilityType::S_ANGEL, { 40, 50, 85, 0, 40, A_INT, _("天使の召喚", "summon an angel") } },
    { MonsterAbilityType::S_DEMON, { 35, 50, 80, 0, 35, A_INT, _("デーモンの召喚", "summon a daemon") } },
    { MonsterAbilityType::S_UNDEAD, { 30, 30, 75, 0, 35, A_INT, _("アンデッドの召喚", "summon an undead") } },
    { MonsterAbilityType::S_DRAGON, { 39, 70, 80, 0, 40, A_INT, _("ドラゴンの召喚", "summon a dragon") } },
    { MonsterAbilityType::S_HI_UNDEAD, { 43, 85, 85, 0, 45, A_INT, _("上級アンデッドの召喚", "summon Greater Undead") } },
    { MonsterAbilityType::S_HI_DRAGON, { 46, 90, 85, 0, 45, A_INT, _("古代ドラゴンの召喚", "summon Ancient Dragon") } },
    { MonsterAbilityType::S_AMBERITES, { 48, 120, 90, 0, 50, A_INT, _("アンバーの王族の召喚", "summon Lords of Amber") } },
    { MonsterAbilityType::S_UNIQUE, { 50, 150, 95, 0, 50, A_INT, _("ユニークモンスターの召喚", "summon Unique Monsters") } },
};

/*!
 * @brief モンスター魔法名テーブル
 */
const std::map<MonsterAbilityType, concptr> monster_powers_short = {
    { MonsterAbilityType::SHRIEK, _("叫ぶ", "Shriek") },
    { MonsterAbilityType::XXX1, _("何か", "Something") },
    { MonsterAbilityType::DISPEL, _("魔力消去", "Dispel-magic") },
    { MonsterAbilityType::ROCKET, _("ロケット", "Rocket") },
    { MonsterAbilityType::SHOOT, _("射撃", "Arrow") },
    { MonsterAbilityType::XXX2, _("何か", "Arrows") },
    { MonsterAbilityType::XXX3, _("何か", "Missile") },
    { MonsterAbilityType::XXX4, _("何か", "Missiles") },
    { MonsterAbilityType::BR_ACID, _("酸", "Acid") },
    { MonsterAbilityType::BR_ELEC, _("電撃", "Lightning") },
    { MonsterAbilityType::BR_FIRE, _("火炎", "Fire") },
    { MonsterAbilityType::BR_COLD, _("冷気", "Cold") },
    { MonsterAbilityType::BR_POIS, _("毒", "Poison") },
    { MonsterAbilityType::BR_NETH, _("地獄", "Nether") },
    { MonsterAbilityType::BR_LITE, _("閃光", "Light") },
    { MonsterAbilityType::BR_DARK, _("暗黒", "Dark") },
    { MonsterAbilityType::BR_CONF, _("混乱", "Confusion") },
    { MonsterAbilityType::BR_SOUN, _("轟音", "Sound") },
    { MonsterAbilityType::BR_CHAO, _("カオス", "Chaos") },
    { MonsterAbilityType::BR_DISE, _("劣化", "Disenchantment") },
    { MonsterAbilityType::BR_NEXU, _("因果混乱", "Nexus") },
    { MonsterAbilityType::BR_TIME, _("時間逆転", "Time") },
    { MonsterAbilityType::BR_INER, _("遅鈍", "Inertia") },
    { MonsterAbilityType::BR_GRAV, _("重力", "Gravity") },
    { MonsterAbilityType::BR_SHAR, _("破片", "Shards") },
    { MonsterAbilityType::BR_PLAS, _("プラズマ", "Plasma") },
    { MonsterAbilityType::BR_FORC, _("フォース", "Force") },
    { MonsterAbilityType::BR_MANA, _("魔力", "Mana") },
    { MonsterAbilityType::BA_NUKE, _("放射能球", "Nuke") },
    { MonsterAbilityType::BR_NUKE, _("放射性廃棄物", "Nuke") },
    { MonsterAbilityType::BA_CHAO, _("純ログルス", "Logrus") },
    { MonsterAbilityType::BR_DISI, _("分解", "Disintegration") },
    { MonsterAbilityType::BR_VOID, _("虚無", "Void") },
    { MonsterAbilityType::BR_ABYSS, _("深淵", "Abyss") },

    { MonsterAbilityType::BA_ACID, _("酸", "Acid") },
    { MonsterAbilityType::BA_ELEC, _("電撃", "Lightning") },
    { MonsterAbilityType::BA_FIRE, _("火炎", "Fire") },
    { MonsterAbilityType::BA_COLD, _("冷気", "Frost") },
    { MonsterAbilityType::BA_POIS, _("悪臭雲", "Stinking Cloud") },
    { MonsterAbilityType::BA_NETH, _("地獄球", "Nether") },
    { MonsterAbilityType::BA_WATE, _("ウォーター", "Water") },
    { MonsterAbilityType::BA_MANA, _("魔力の嵐", "Mana storm") },
    { MonsterAbilityType::BA_DARK, _("暗黒の嵐", "Darkness storm") },
    { MonsterAbilityType::BA_VOID, _("虚無", "Void") },
    { MonsterAbilityType::BA_ABYSS, _("深淵", "Abyss") },
    { MonsterAbilityType::DRAIN_MANA, _("魔力吸収", "Drain mana") },
    { MonsterAbilityType::MIND_BLAST, _("精神攻撃", "Mind blast") },
    { MonsterAbilityType::BRAIN_SMASH, _("脳攻撃", "Brain smash") },
    { MonsterAbilityType::CAUSE_1, _("軽傷", "Cause Light Wound") },
    { MonsterAbilityType::CAUSE_2, _("重傷", "Cause Serious Wound") },
    { MonsterAbilityType::CAUSE_3, _("致命傷", "Cause Critical Wound") },
    { MonsterAbilityType::CAUSE_4, _("秘孔を突く", "Cause Mortal Wound") },
    { MonsterAbilityType::BO_ACID, _("酸", "Acid") },
    { MonsterAbilityType::BO_ELEC, _("電撃", "Lightning") },
    { MonsterAbilityType::BO_FIRE, _("火炎", "Fire") },
    { MonsterAbilityType::BO_COLD, _("冷気", "Frost") },
    { MonsterAbilityType::BA_LITE, _("スターバースト", "Starburst") },
    { MonsterAbilityType::BO_NETH, _("地獄の矢", "Nether") },
    { MonsterAbilityType::BO_WATE, _("ウォーター", "Water") },
    { MonsterAbilityType::BO_MANA, _("魔力の矢", "Mana") },
    { MonsterAbilityType::BO_PLAS, _("プラズマ", "Plasma") },
    { MonsterAbilityType::BO_ICEE, _("極寒", "Ice") },
    { MonsterAbilityType::BO_VOID, _("ヴォイド", "Void") },
    { MonsterAbilityType::BO_ABYSS, _("アビス", "Abyss") },
    { MonsterAbilityType::MISSILE, _("マジックミサイル", "Magic missile") },
    { MonsterAbilityType::SCARE, _("恐慌", "Scare") },
    { MonsterAbilityType::BLIND, _("盲目", "Blind") },
    { MonsterAbilityType::CONF, _("混乱", "Confuse") },
    { MonsterAbilityType::SLOW, _("減速", "Slow") },
    { MonsterAbilityType::HOLD, _("睡眠", "Sleep") },

    { MonsterAbilityType::HASTE, _("加速", "Speed") },
    { MonsterAbilityType::HAND_DOOM, _("破滅の手", "Hand of doom") },
    { MonsterAbilityType::HEAL, _("体力回復", "Heal-self") },
    { MonsterAbilityType::INVULNER, _("無傷の球", "Invulnerable") },
    { MonsterAbilityType::BLINK, _("ショートテレポート", "Blink") },
    { MonsterAbilityType::TPORT, _("テレポート", "Teleport") },
    { MonsterAbilityType::WORLD, _("時を止める", "The world") },
    { MonsterAbilityType::SPECIAL, _("何か", "Something") },
    { MonsterAbilityType::TELE_TO, _("引きよせる", "Teleport to") },
    { MonsterAbilityType::TELE_AWAY, _("テレポートアウェイ", "Teleport away") },
    { MonsterAbilityType::TELE_LEVEL, _("テレポートレベル", "Teleport level") },
    { MonsterAbilityType::PSY_SPEAR, _("光の剣", "Psycho-spear") },
    { MonsterAbilityType::DARKNESS, _("暗闇", "Create darkness") },
    { MonsterAbilityType::TRAPS, _("トラップ創造", "Create traps") },
    { MonsterAbilityType::FORGET, _("記憶喪失", "Amnesia") },
    { MonsterAbilityType::RAISE_DEAD, _("死者復活", "Raise dead") },
    { MonsterAbilityType::S_KIN, _("援軍", "Aid") },
    { MonsterAbilityType::S_CYBER, _("サイバーデーモン", "Cyberdeamons") },
    { MonsterAbilityType::S_MONSTER, _("モンスター", "A monster") },
    { MonsterAbilityType::S_MONSTERS, _("複数のモンスター", "Monsters") },
    { MonsterAbilityType::S_ANT, _("蟻", "Ants") },
    { MonsterAbilityType::S_SPIDER, _("蜘蛛", "Spiders") },
    { MonsterAbilityType::S_HOUND, _("ハウンド", "Hounds") },
    { MonsterAbilityType::S_HYDRA, _("ヒドラ", "Hydras") },
    { MonsterAbilityType::S_ANGEL, _("天使", "Angel") },
    { MonsterAbilityType::S_DEMON, _("悪魔", "Daemon") },
    { MonsterAbilityType::S_UNDEAD, _("アンデッド", "Undead") },
    { MonsterAbilityType::S_DRAGON, _("ドラゴン", "Dragon") },
    { MonsterAbilityType::S_HI_UNDEAD, _("上級アンデッド", "Greater Undead") },
    { MonsterAbilityType::S_HI_DRAGON, _("古代ドラゴン", "Ancient Dragon") },
    { MonsterAbilityType::S_AMBERITES, _("アンバーの王族", "Lords of Amber") },
    { MonsterAbilityType::S_UNIQUE, _("ユニーク", "Unique monsters") },
};
