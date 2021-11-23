﻿#include "info-reader/kind-info-tokens-table.h"

// clang-format off
/*!
 * オブジェクト基本特性トークンの定義 /
 * Object flags
 */
const std::unordered_map<std::string_view, tr_type> k_info_flags = {
    { "STR", TR_STR },
    { "INT", TR_INT },
    { "WIS", TR_WIS },
    { "DEX", TR_DEX },
    { "CON", TR_CON },
    { "CHR", TR_CHR },
    { "MAGIC_MASTERY", TR_MAGIC_MASTERY },
    { "FORCE_WEAPON", TR_FORCE_WEAPON },
    { "STEALTH", TR_STEALTH },
    { "SEARCH", TR_SEARCH },
    { "INFRA", TR_INFRA },
    { "TUNNEL", TR_TUNNEL },
    { "SPEED", TR_SPEED },
    { "BLOWS", TR_BLOWS },
    { "CHAOTIC", TR_CHAOTIC },
    { "VAMPIRIC", TR_VAMPIRIC },
    { "SLAY_ANIMAL", TR_SLAY_ANIMAL },
    { "SLAY_EVIL", TR_SLAY_EVIL },
    { "SLAY_UNDEAD", TR_SLAY_UNDEAD },
    { "SLAY_DEMON", TR_SLAY_DEMON },
    { "SLAY_ORC", TR_SLAY_ORC },
    { "SLAY_TROLL", TR_SLAY_TROLL },
    { "SLAY_GIANT", TR_SLAY_GIANT },
    { "SLAY_DRAGON", TR_SLAY_DRAGON },
    { "KILL_DRAGON", TR_KILL_DRAGON },
    { "VORPAL", TR_VORPAL },
    { "EARTHQUAKE", TR_EARTHQUAKE },
    { "BRAND_POIS", TR_BRAND_POIS },
    { "BRAND_ACID", TR_BRAND_ACID },
    { "BRAND_ELEC", TR_BRAND_ELEC },
    { "BRAND_FIRE", TR_BRAND_FIRE },
    { "BRAND_COLD", TR_BRAND_COLD },

    { "SUST_STR", TR_SUST_STR },
    { "SUST_INT", TR_SUST_INT },
    { "SUST_WIS", TR_SUST_WIS },
    { "SUST_DEX", TR_SUST_DEX },
    { "SUST_CON", TR_SUST_CON },
    { "SUST_CHR", TR_SUST_CHR },
    { "RIDING", TR_RIDING },
    { "EASY_SPELL", TR_EASY_SPELL },
    { "IM_ACID", TR_IM_ACID },
    { "IM_ELEC", TR_IM_ELEC },
    { "IM_FIRE", TR_IM_FIRE },
    { "IM_COLD", TR_IM_COLD },
    { "THROW", TR_THROW },
    { "REFLECT", TR_REFLECT },
    { "FREE_ACT", TR_FREE_ACT },
    { "HOLD_EXP", TR_HOLD_EXP },
    { "RES_ACID", TR_RES_ACID },
    { "RES_ELEC", TR_RES_ELEC },
    { "RES_FIRE", TR_RES_FIRE },
    { "RES_COLD", TR_RES_COLD },
    { "RES_POIS", TR_RES_POIS },
    { "RES_FEAR", TR_RES_FEAR },
    { "RES_LITE", TR_RES_LITE },
    { "RES_DARK", TR_RES_DARK },
    { "RES_BLIND", TR_RES_BLIND },
    { "RES_CONF", TR_RES_CONF },
    { "RES_SOUND", TR_RES_SOUND },
    { "RES_SHARDS", TR_RES_SHARDS },
    { "RES_NETHER", TR_RES_NETHER },
    { "RES_NEXUS", TR_RES_NEXUS },
    { "RES_CHAOS", TR_RES_CHAOS },
    { "RES_DISEN", TR_RES_DISEN },

    { "SH_FIRE", TR_SH_FIRE },
    { "SH_ELEC", TR_SH_ELEC },
    { "SLAY_HUMAN", TR_SLAY_HUMAN },
    { "SH_COLD", TR_SH_COLD },
    { "NO_TELE", TR_NO_TELE },
    { "NO_MAGIC", TR_NO_MAGIC },
    { "DEC_MANA", TR_DEC_MANA },
    { "TY_CURSE", TR_TY_CURSE },
    { "WARNING", TR_WARNING },
    { "HIDE_TYPE", TR_HIDE_TYPE },
    { "SHOW_MODS", TR_SHOW_MODS },
    { "SLAY_GOOD", TR_SLAY_GOOD },
    { "LEVITATION", TR_LEVITATION },
    { "LITE", TR_LITE_1 }, //<! @note 古い書式
    { "SEE_INVIS", TR_SEE_INVIS },
    { "TELEPATHY", TR_TELEPATHY },
    { "SLOW_DIGEST", TR_SLOW_DIGEST },
    { "REGEN", TR_REGEN },
    { "XTRA_MIGHT", TR_XTRA_MIGHT },
    { "XTRA_SHOTS", TR_XTRA_SHOTS },
    { "IGNORE_ACID", TR_IGNORE_ACID },
    { "IGNORE_ELEC", TR_IGNORE_ELEC },
    { "IGNORE_FIRE", TR_IGNORE_FIRE },
    { "IGNORE_COLD", TR_IGNORE_COLD },
    { "ACTIVATE", TR_ACTIVATE },
    { "DRAIN_EXP", TR_DRAIN_EXP },
    { "TELEPORT", TR_TELEPORT },
    { "AGGRAVATE", TR_AGGRAVATE },
    { "BLESSED", TR_BLESSED },
    /* { "XXX3", TR_XXX3 }, Fake flag for Smith */
    /* { "XXX4", TR_XXX4 }, Fake flag for Smith */
    { "KILL_GOOD", TR_KILL_GOOD },

    { "KILL_ANIMAL", TR_KILL_ANIMAL },
    { "KILL_EVIL", TR_KILL_EVIL },
    { "KILL_UNDEAD", TR_KILL_UNDEAD },
    { "KILL_DEMON", TR_KILL_DEMON },
    { "KILL_ORC", TR_KILL_ORC },
    { "KILL_TROLL", TR_KILL_TROLL },
    { "KILL_GIANT", TR_KILL_GIANT },
    { "KILL_HUMAN", TR_KILL_HUMAN },
    { "ESP_ANIMAL", TR_ESP_ANIMAL },
    { "ESP_UNDEAD", TR_ESP_UNDEAD },
    { "ESP_DEMON", TR_ESP_DEMON },
    { "ESP_ORC", TR_ESP_ORC },
    { "ESP_TROLL", TR_ESP_TROLL },
    { "ESP_GIANT", TR_ESP_GIANT },
    { "ESP_DRAGON", TR_ESP_DRAGON },
    { "ESP_HUMAN", TR_ESP_HUMAN },
    { "ESP_EVIL", TR_ESP_EVIL },
    { "ESP_GOOD", TR_ESP_GOOD },
    { "ESP_NONLIVING", TR_ESP_NONLIVING },
    { "ESP_UNIQUE", TR_ESP_UNIQUE },
    { "FULL_NAME", TR_FULL_NAME },
    { "FIXED_FLAVOR", TR_FIXED_FLAVOR },
    { "ADD_L_CURSE", TR_ADD_L_CURSE },
    { "ADD_H_CURSE", TR_ADD_H_CURSE },
    { "DRAIN_HP", TR_DRAIN_HP },
    { "DRAIN_MANA", TR_DRAIN_MANA },

    { "LITE_1", TR_LITE_1 },
    { "LITE_2", TR_LITE_2 },
    { "LITE_3", TR_LITE_3 },
    { "LITE_M1", TR_LITE_M1 },
    { "LITE_M2", TR_LITE_M2 },
    { "LITE_M3", TR_LITE_M3 },
    { "LITE_FUEL", TR_LITE_FUEL },
    { "CALL_ANIMAL", TR_CALL_ANIMAL },
    { "CALL_DEMON", TR_CALL_DEMON },
    { "CALL_DRAGON", TR_CALL_DRAGON },
    { "CALL_UNDEAD", TR_CALL_UNDEAD },
    { "COWARDICE", TR_COWARDICE },
    { "LOW_MELEE", TR_LOW_MELEE },
    { "LOW_AC", TR_LOW_AC },
    { "HARD_SPELL", TR_HARD_SPELL },
    { "FAST_DIGEST", TR_FAST_DIGEST },
    { "SLOW_REGEN", TR_SLOW_REGEN },
    { "MIGHTY_THROW", TR_MIGHTY_THROW },
    { "EASY2_WEAPON", TR_EASY2_WEAPON },
    { "DOWN_SAVING", TR_DOWN_SAVING },
    { "NO_AC", TR_NO_AC },
    { "HEAVY_SPELL", TR_HEAVY_SPELL },
    { "RES_TIME", TR_RES_TIME },
    { "RES_WATER", TR_RES_WATER },
    { "INVULN_ARROW", TR_INVULN_ARROW },
    { "DARK_SOURCE", TR_DARK_SOURCE },
    { "SUPPORTIVE", TR_SUPPORTIVE },
    { "RES_CURSE", TR_RES_CURSE },
    { "BERS_RAGE", TR_BERS_RAGE },
    { "BRAND_MAGIC", TR_BRAND_MAGIC },
    { "IMPACT", TR_IMPACT },
    { "NASTY", TR_NASTY },
    { "INDESTRUCTIBLE", TR_INDESTRUCTIBLE },
    { "NEVER_MOVE", TR_NEVER_MOVE },
    { "VUL_ACID", TR_VUL_ACID },
    { "VUL_COLD", TR_VUL_COLD },
    { "VUL_ELEC", TR_VUL_ELEC },
    { "VUL_FIRE", TR_VUL_FIRE },
    { "VUL_LITE", TR_VUL_LITE },
    { "IM_DARK", TR_IM_DARK },
    { "INVEN_ACTIVATE", TR_INVEN_ACTIVATE },
    { "SELF_FIRE", TR_SELF_FIRE },
    { "SELF_COLD", TR_SELF_COLD },
    { "SELF_ELEC", TR_SELF_ELEC },
    { "WORLD_END", TR_WORLD_END },
    { "PERSISTENT_CURSE", TR_PERSISTENT_CURSE },
    { "VUL_CURSE", TR_VUL_CURSE },
};

/*!
 * オブジェクト生成特性トークンの定義 /
 * Object flags
 */
const std::unordered_map<std::string_view, ItemGenerationTraitType> k_info_gen_flags = {
    { "INSTA_ART", ItemGenerationTraitType::INSTA_ART },
    { "QUESTITEM", ItemGenerationTraitType::QUESTITEM },
    { "XTRA_POWER", ItemGenerationTraitType::XTRA_POWER },
    { "ONE_SUSTAIN", ItemGenerationTraitType::ONE_SUSTAIN },
    { "XTRA_RES_OR_POWER", ItemGenerationTraitType::XTRA_RES_OR_POWER },
    { "XTRA_H_RES", ItemGenerationTraitType::XTRA_H_RES },
    { "XTRA_E_RES", ItemGenerationTraitType::XTRA_E_RES },
    { "XTRA_L_RES", ItemGenerationTraitType::XTRA_L_RES },
    { "XTRA_D_RES", ItemGenerationTraitType::XTRA_D_RES },
    { "XTRA_RES", ItemGenerationTraitType::XTRA_RES },
    { "CURSED", ItemGenerationTraitType::CURSED },
    { "HEAVY_CURSE", ItemGenerationTraitType::HEAVY_CURSE },
    { "PERMA_CURSE", ItemGenerationTraitType::PERMA_CURSE },
    { "RANDOM_CURSE0", ItemGenerationTraitType::RANDOM_CURSE0 },
    { "RANDOM_CURSE1", ItemGenerationTraitType::RANDOM_CURSE1 },
    { "RANDOM_CURSE2", ItemGenerationTraitType::RANDOM_CURSE2 },
    { "XTRA_DICE", ItemGenerationTraitType::XTRA_DICE },
    { "POWERFUL", ItemGenerationTraitType::POWERFUL },
    { "LIGHT_WEIGHT", ItemGenerationTraitType::LIGHT_WEIGHT },
    { "HEAVY_WEIGHT", ItemGenerationTraitType::HEAVY_WEIGHT },
    { "XTRA_AC", ItemGenerationTraitType::XTRA_AC },
    { "HIGH_TELEPATHY", ItemGenerationTraitType::HIGH_TELEPATHY },
    { "LOW_TELEPATHY", ItemGenerationTraitType::LOW_TELEPATHY },
    { "XTRA_L_ESP", ItemGenerationTraitType::XTRA_L_ESP },
    { "MOD_ACCURACY", ItemGenerationTraitType::MOD_ACCURACY },
    { "MOD_VELOCITY", ItemGenerationTraitType::MOD_VELOCITY },
    { "XTRA_DICE_SIDE", ItemGenerationTraitType::XTRA_DICE_SIDE },
    { "ADD_DICE", ItemGenerationTraitType::ADD_DICE },
    { "DOUBLED_DICE", ItemGenerationTraitType::DOUBLED_DICE },
};
// clang-format on
