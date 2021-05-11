﻿#include "info-reader/race-info-tokens-table.h"
#include "monster-attack/monster-attack-effect.h"
#include "monster-attack/monster-attack-types.h"
#include "monster-race/race-ability-flags.h"

/*!
  * モンスターの打撃手段トークンの定義 /
  * Monster Blow Methods
  */
concptr r_info_blow_method[NB_RBM_TYPE + 1] = {
	"",
	"HIT",
	"TOUCH",
	"PUNCH",
	"KICK",
	"CLAW",
	"BITE",
	"STING",
	"SLASH",
	"BUTT",
	"CRUSH",
	"ENGULF",
	"CHARGE",
	"CRAWL",
	"DROOL",
	"SPIT",
	"EXPLODE",
	"GAZE",
	"WAIL",
	"SPORE",
	"XXX4",
	"BEG",
	"INSULT",
	"MOAN",
	"SHOW",
	"SHOOT",
	NULL
};

/*!
 * モンスターの打撃属性トークンの定義 /
 * Monster Blow Effects
 */
concptr r_info_blow_effect[NB_RBE_TYPE + 1] = {
	"",
	"HURT",
	"POISON",
	"UN_BONUS",
	"UN_POWER",
	"EAT_GOLD",
	"EAT_ITEM",
	"EAT_FOOD",
	"EAT_LITE",
	"ACID",
	"ELEC",
	"FIRE",
	"COLD",
	"BLIND",
	"CONFUSE",
	"TERRIFY",
	"PARALYZE",
	"LOSE_STR",
	"LOSE_INT",
	"LOSE_WIS",
	"LOSE_DEX",
	"LOSE_CON",
	"LOSE_CHR",
	"LOSE_ALL",
	"SHATTER",
	"EXP_10",
	"EXP_20",
	"EXP_40",
	"EXP_80",
	"DISEASE",
	"TIME",
	"EXP_VAMP",
	"DR_MANA",
	"SUPERHURT",
	"INERTIA",
	"STUN",
	"FLAVOR",
	NULL
};

/*!
 * モンスター特性トークンの定義1 /
 * Monster race flags
 */
concptr r_info_flags1[NUM_R_FLAGS_1] = {
	"UNIQUE",
	"QUESTOR",
	"MALE",
	"FEMALE",
	"CHAR_CLEAR",
	"SHAPECHANGER",
	"ATTR_CLEAR",
	"ATTR_MULTI",
	"FORCE_DEPTH",
	"FORCE_MAXHP",
	"PREVENT_SUDDEN_MAGIC",
	"FORCE_EXTRA",
	"ATTR_SEMIRAND",
	"FRIENDS",
	"ESCORT",
	"ESCORTS",
	"NEVER_BLOW",
	"NEVER_MOVE",
	"RAND_25",
	"RAND_50",
	"ONLY_GOLD",
	"ONLY_ITEM",
	"DROP_60",
	"DROP_90",
	"DROP_1D2",
	"DROP_2D2",
	"DROP_3D2",
	"DROP_4D2",
	"DROP_GOOD",
	"DROP_GREAT",
	"XXX2",
	"XXX3"
};

/*!
 * モンスター特性トークンの定義2 /
 * Monster race flags
 */
concptr r_info_flags2[NUM_R_FLAGS_2] = {
	"STUPID",
	"SMART",
	"CAN_SPEAK",
	"REFLECTING",
	"INVISIBLE",
	"COLD_BLOOD",
	"EMPTY_MIND",
	"WEIRD_MIND",
	"MULTIPLY",
	"REGENERATE",
	"CHAR_MULTI",
	"ATTR_ANY",
	"POWERFUL",
	"ELDRITCH_HORROR",
	"AURA_FIRE",
	"AURA_ELEC",
	"OPEN_DOOR",
	"BASH_DOOR",
	"PASS_WALL",
	"KILL_WALL",
	"MOVE_BODY",
	"KILL_BODY",
	"TAKE_ITEM",
	"KILL_ITEM",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"HUMAN",
	"QUANTUM"
};

/*!
 * モンスター特性トークンの定義3 /
 * Monster race flags
 */
concptr r_info_flags3[NUM_R_FLAGS_3] = {
	"ORC",
	"TROLL",
	"GIANT",
	"DRAGON",
	"DEMON",
	"UNDEAD",
	"EVIL",
	"ANIMAL",
	"AMBERITE",
	"GOOD",
	"AURA_COLD",
	"NONLIVING",
	"HURT_LITE",
	"HURT_ROCK",
	"HURT_FIRE",
	"HURT_COLD",
	"ANGEL",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"NO_FEAR",
	"NO_STUN",
	"NO_CONF",
	"NO_SLEEP"
};

/*!
 * モンスター特性トークン (発動型能力) /
 * Monster race flags
 */
/* clang-format off */
const std::unordered_map<std::string_view, RF_ABILITY> r_info_ability_flags = {
	{"SHRIEK", RF_ABILITY::SHRIEK },
	{"XXX1", RF_ABILITY::XXX1 },
	{"DISPEL", RF_ABILITY::DISPEL },
	{"ROCKET", RF_ABILITY::ROCKET },
	{"SHOOT", RF_ABILITY::SHOOT },
	{"XXX2", RF_ABILITY::XXX2 },
	{"XXX3", RF_ABILITY::XXX3 },
	{"XXX4", RF_ABILITY::XXX4 },
	{"BR_ACID", RF_ABILITY::BR_ACID },
	{"BR_ELEC", RF_ABILITY::BR_ELEC },
	{"BR_FIRE", RF_ABILITY::BR_FIRE },
	{"BR_COLD", RF_ABILITY::BR_COLD },
	{"BR_POIS", RF_ABILITY::BR_POIS },
	{"BR_NETH", RF_ABILITY::BR_NETH },
	{"BR_LITE", RF_ABILITY::BR_LITE },
	{"BR_DARK", RF_ABILITY::BR_DARK },
	{"BR_CONF", RF_ABILITY::BR_CONF },
	{"BR_SOUN", RF_ABILITY::BR_SOUN },
	{"BR_CHAO", RF_ABILITY::BR_CHAO },
	{"BR_DISE", RF_ABILITY::BR_DISE },
	{"BR_NEXU", RF_ABILITY::BR_NEXU },
	{"BR_TIME", RF_ABILITY::BR_TIME },
	{"BR_INER", RF_ABILITY::BR_INER },
	{"BR_GRAV", RF_ABILITY::BR_GRAV },
	{"BR_SHAR", RF_ABILITY::BR_SHAR },
	{"BR_PLAS", RF_ABILITY::BR_PLAS },
	{"BR_FORC", RF_ABILITY::BR_FORC },
	{"BR_MANA", RF_ABILITY::BR_MANA },
	{"BA_NUKE", RF_ABILITY::BA_NUKE },
	{"BR_NUKE", RF_ABILITY::BR_NUKE },
	{"BA_CHAO", RF_ABILITY::BA_CHAO },
	{"BR_DISI", RF_ABILITY::BR_DISI },

	{"BA_ACID", RF_ABILITY::BA_ACID },
	{"BA_ELEC", RF_ABILITY::BA_ELEC },
	{"BA_FIRE", RF_ABILITY::BA_FIRE },
	{"BA_COLD", RF_ABILITY::BA_COLD },
	{"BA_POIS", RF_ABILITY::BA_POIS },
	{"BA_NETH", RF_ABILITY::BA_NETH },
	{"BA_WATE", RF_ABILITY::BA_WATE },
	{"BA_MANA", RF_ABILITY::BA_MANA },
	{"BA_DARK", RF_ABILITY::BA_DARK },
	{"DRAIN_MANA", RF_ABILITY::DRAIN_MANA },
	{"MIND_BLAST", RF_ABILITY::MIND_BLAST },
	{"BRAIN_SMASH", RF_ABILITY::BRAIN_SMASH },
	{"CAUSE_1", RF_ABILITY::CAUSE_1 },
	{"CAUSE_2", RF_ABILITY::CAUSE_2 },
	{"CAUSE_3", RF_ABILITY::CAUSE_3 },
	{"CAUSE_4", RF_ABILITY::CAUSE_4 },
	{"BO_ACID", RF_ABILITY::BO_ACID },
	{"BO_ELEC", RF_ABILITY::BO_ELEC },
	{"BO_FIRE", RF_ABILITY::BO_FIRE },
	{"BO_COLD", RF_ABILITY::BO_COLD },
	{"BA_LITE", RF_ABILITY::BA_LITE },
	{"BO_NETH", RF_ABILITY::BO_NETH },
	{"BO_WATE", RF_ABILITY::BO_WATE },
	{"BO_MANA", RF_ABILITY::BO_MANA },
	{"BO_PLAS", RF_ABILITY::BO_PLAS },
	{"BO_ICEE", RF_ABILITY::BO_ICEE },
	{"MISSILE", RF_ABILITY::MISSILE },
	{"SCARE", RF_ABILITY::SCARE },
	{"BLIND", RF_ABILITY::BLIND },
	{"CONF", RF_ABILITY::CONF },
	{"SLOW", RF_ABILITY::SLOW },
	{"HOLD", RF_ABILITY::HOLD },

	{"HASTE", RF_ABILITY::HASTE },
	{"HAND_DOOM", RF_ABILITY::HAND_DOOM },
	{"HEAL", RF_ABILITY::HEAL },
	{"INVULNER", RF_ABILITY::INVULNER },
	{"BLINK", RF_ABILITY::BLINK },
	{"TPORT", RF_ABILITY::TPORT },
	{"WORLD", RF_ABILITY::WORLD },
	{"SPECIAL", RF_ABILITY::SPECIAL },
	{"TELE_TO", RF_ABILITY::TELE_TO },
	{"TELE_AWAY", RF_ABILITY::TELE_AWAY },
	{"TELE_LEVEL", RF_ABILITY::TELE_LEVEL },
	{"PSY_SPEAR", RF_ABILITY::PSY_SPEAR },
	{"DARKNESS", RF_ABILITY::DARKNESS },
	{"TRAPS", RF_ABILITY::TRAPS },
	{"FORGET", RF_ABILITY::FORGET },
	{"ANIM_DEAD", RF_ABILITY::RAISE_DEAD /* ToDo: Implement ANIM_DEAD */ },
	{"S_KIN", RF_ABILITY::S_KIN },
	{"S_CYBER", RF_ABILITY::S_CYBER },
	{"S_MONSTER", RF_ABILITY::S_MONSTER },
	{"S_MONSTERS", RF_ABILITY::S_MONSTERS },
	{"S_ANT", RF_ABILITY::S_ANT },
	{"S_SPIDER", RF_ABILITY::S_SPIDER },
	{"S_HOUND", RF_ABILITY::S_HOUND },
	{"S_HYDRA", RF_ABILITY::S_HYDRA },
	{"S_ANGEL", RF_ABILITY::S_ANGEL },
	{"S_DEMON", RF_ABILITY::S_DEMON },
	{"S_UNDEAD", RF_ABILITY::S_UNDEAD },
	{"S_DRAGON", RF_ABILITY::S_DRAGON },
	{"S_HI_UNDEAD", RF_ABILITY::S_HI_UNDEAD },
	{"S_HI_DRAGON", RF_ABILITY::S_HI_DRAGON },
	{"S_AMBERITES", RF_ABILITY::S_AMBERITES },
	{"S_UNIQUE", RF_ABILITY::S_UNIQUE },
};
/* clang-format on */

/*!
 * モンスター特性トークンの定義7 /
 * Monster race flags
 * "GUARDIAN" ... init.c d_infoの FINAL_GUARDIAN_* にて自動指定
 */
concptr r_info_flags7[NUM_R_FLAGS_7] = {
	"AQUATIC",
	"CAN_SWIM",
	"CAN_FLY",
	"FRIENDLY",
	"NAZGUL",
	"UNIQUE2",
	"RIDING",
	"KAGE",
	"HAS_LITE_1",
	"SELF_LITE_1",
	"HAS_LITE_2",
	"SELF_LITE_2",
	"XXX7X12",
	"CHAMELEON",
	"XXXX4XXX",
	"TANUKI",
	"HAS_DARK_1",
	"SELF_DARK_1",
	"HAS_DARK_2",
	"SELF_DARK_2",
	"XXX7X20",
	"XXX7X21",
	"XXX7X22",
	"XXX7X23",
	"XXX7X24",
	"XXX7X25",
	"XXX7X26",
	"XXX7X27",
	"XXX7X28",
	"XXX7X29",
	"XXX7X30",
	"XXX7X31",
};

/*!
 * モンスター特性トークンの定義8 /
 * Monster race flags
 */
concptr r_info_flags8[NUM_R_FLAGS_8] = {
	"WILD_ONLY",
	"WILD_TOWN",
	"XXX8X02",
	"WILD_SHORE",
	"WILD_OCEAN",
	"WILD_WASTE",
	"WILD_WOOD",
	"WILD_VOLCANO",
	"XXX8X08",
	"WILD_MOUNTAIN",
	"WILD_GRASS",
	"XXX8X11",
	"XXX8X12",
	"XXX8X13",
	"XXX8X14",
	"XXX8X15",
	"XXX8X16",
	"XXX8X17",
	"XXX8X18",
	"XXX8X19",
	"XXX8X20",
	"XXX8X21",
	"XXX8X22",
	"KARATEKA",
    "NINJA",
	"SUMOU_WRESTLER",
	"YAKUZA",
	"HOMO_SEXUAL",
	"JOKE",
	"NASTY",
	"WILD_SWAMP",	/* ToDo: Implement Swamp */
	"WILD_ALL",
};

/*!
 * モンスター特性トークンの定義9 /
 * Monster race flags
 */
concptr r_info_flags9[NUM_R_FLAGS_9] = {
	"DROP_CORPSE",
	"DROP_SKELETON",
	"EAT_BLIND",
	"EAT_CONF",
	"EAT_MANA",
	"EAT_NEXUS",
	"EAT_BLINK",
	"EAT_SLEEP",
	"EAT_BERSERKER",
	"EAT_ACIDIC",
	"EAT_SPEED",
	"EAT_CURE",
	"EAT_FIRE_RES",
	"EAT_COLD_RES",
	"EAT_ACID_RES",
	"EAT_ELEC_RES",
	"EAT_POIS_RES",
	"EAT_INSANITY",
	"EAT_DRAIN_EXP",
	"EAT_POISONOUS",
	"EAT_GIVE_STR",
	"EAT_GIVE_INT",
	"EAT_GIVE_WIS",
	"EAT_GIVE_DEX",
	"EAT_GIVE_CON",
	"EAT_GIVE_CHR",
	"EAT_LOSE_STR",
	"EAT_LOSE_INT",
	"EAT_LOSE_WIS",
	"EAT_LOSE_DEX",
	"EAT_LOSE_CON",
	"EAT_LOSE_CHR",
	"EAT_DRAIN_MANA",
};

/*!
 * モンスター特性トークンの定義R(耐性) /
 * Monster race flags
 */
concptr r_info_flagsr[NUM_R_FLAGS_R] = {
	"IM_ACID",
	"IM_ELEC",
	"IM_FIRE",
	"IM_COLD",
	"IM_POIS",
	"RES_LITE",
	"RES_DARK",
	"RES_NETH",
	"RES_WATE",
	"RES_PLAS",
	"RES_SHAR",
	"RES_SOUN",
	"RES_CHAO",
	"RES_NEXU",
	"RES_DISE",
	"RES_WALL",
	"RES_INER",
	"RES_TIME",
	"RES_GRAV",
	"RES_ALL",
	"RES_TELE",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
	"XXX",
};
