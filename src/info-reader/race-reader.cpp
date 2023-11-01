#include "info-reader/race-reader.h"
#include "alliance/alliance.h"
#include "artifact/fixed-art-types.h"
#include "info-reader/info-reader-util.h"
#include "info-reader/json-reader-util.h"
#include "info-reader/parse-error-types.h"
#include "info-reader/race-info-tokens-table.h"
#include "locale/japanese.h"
#include "main/angband-headers.h"
#include "player-ability/player-ability-types.h"
#include "system/monster-race-info.h"
#include "term/gameterm.h"
#include "util/enum-converter.h"
#include "util/string-processor.h"
#include "view/display-messages.h"
#include <string>

/*!
 * @brief テキストトークンを走査してフラグを一つ得る(モンスター用1) /
 * Grab one (basic) flag in a MonsterRaceInfo from a textual string
 * @param monrace 保管先のモンスター種族構造体
 * @param what 参照元の文字列ポインタ
 * @return 見つけたらtrue
 */
static bool grab_one_basic_flag(MonsterRaceInfo &monrace, std::string_view what)
{
    if (EnumClassFlagGroup<MonsterFeedType>::grab_one_flag(monrace.meat_feed_flags, r_info_meat_feed, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterResistanceType>::grab_one_flag(monrace.resistance_flags, r_info_flagsr, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterAuraType>::grab_one_flag(monrace.aura_flags, r_info_aura_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterBehaviorType>::grab_one_flag(monrace.behavior_flags, r_info_behavior_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterVisualType>::grab_one_flag(monrace.visual_flags, r_info_visual_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterKindType>::grab_one_flag(monrace.kind_flags, r_info_kind_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterDropType>::grab_one_flag(monrace.drop_flags, r_info_drop_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterWildernessType>::grab_one_flag(monrace.wilderness_flags, r_info_wilderness_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterFeatureType>::grab_one_flag(monrace.feature_flags, r_info_feature_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterPopulationType>::grab_one_flag(monrace.population_flags, r_info_population_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterSpeakType>::grab_one_flag(monrace.speak_flags, r_info_speak_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterBrightnessType>::grab_one_flag(monrace.brightness_flags, r_info_brightness_flags, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterSpecialType>::grab_one_flag(monrace.special_flags, r_info_flags1, what)) {
        return true;
    }

    if (EnumClassFlagGroup<MonsterMiscType>::grab_one_flag(monrace.misc_flags, r_info_flags2, what)) {
        return true;
    }

    return false;
}

/*!
 * @brief テキストトークンを走査してフラグを一つ得る(モンスター用2) /
 * Grab one (spell) flag in a MonsterRaceInfo from a textual string
 * @param monrace 保管先のモンスター種族構造体
 * @param what 参照元の文字列ポインタ
 * @return 見つけたらtrue
 */
static bool grab_one_spell_flag(MonsterRaceInfo &monrace, std::string_view what)
{
    if (EnumClassFlagGroup<MonsterAbilityType>::grab_one_flag(monrace.ability_flags, r_info_ability_flags, what)) {
        return true;
    }

    msg_format(_("未知のモンスター・フラグ '%s'。", "Unknown monster flag '%s'."), what.data());
    return false;
}

/*!
 * @brief JSON Objectからモンスター名をセットする
 * @param name_data 名前情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_name(const nlohmann::json &name_data, MonsterRaceInfo &monrace)
{
    if (name_data.is_null()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }
    if (!name_data["ja"].is_string() || !name_data["en"].is_string()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto ja_name = name_data["ja"].get<std::string>();
    const auto en_name = name_data["en"].get<std::string>();

#ifdef JP
    auto ja_name_sys = utf8_to_sys(ja_name);
    if (!ja_name_sys) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.name = std::move(*ja_name_sys);
    monrace.E_name = en_name;
#else
    monrace.name = en_name;
#endif
    /*
        } else if (tokens[0] == "T") {
            if (tokens.size() < 2 || tokens[1].size() == 0) {
                return PARSE_ERROR_TOO_FEW_ARGUMENTS;
            }
            monrace.tag = tokens[1];
        } else if (tokens[0] == "D") {
            // D:text_ja
            // D:$text_en
            if (tokens.size() < 2 || buf.length() < 3) {
    */
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターシンボルをセットする
 * @param symbol_data シンボル情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_symbol(const nlohmann::json &symbol_data, MonsterRaceInfo &monrace)
{
    if (symbol_data.is_null()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto &character_obj = symbol_data["character"];
    if (!character_obj.is_string()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto &color_obj = symbol_data["color"];
    if (!color_obj.is_string()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto color = color_list.find(color_obj.get<std::string>());
    if (color == color_list.end()) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    if (color->second > 127) {
        return PARSE_ERROR_GENERIC;
    }

    monrace.symbol_definition = { color->second, character_obj.get<std::string>().front() };
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスター速度をセットする
 * @param speed_data 速度情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_speed(const nlohmann::json &speed_data, MonsterRaceInfo &monrace)
{
    int speed;
    if (auto err = info_set_integer(speed_data, speed, true, Range(-50, 99))) {
        return err;
    }
    monrace.speed = speed + STANDARD_SPEED;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの進化をセットする
 * @param evolve_data 進化情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_evolve(nlohmann::json &evolve_data, MonsterRaceInfo &monrace)
{
    if (evolve_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    /*
    const auto &flags = str_split(tokens[1], '|', true, 10);
    for (const auto &f : flags) {

        if (f.size() == 0) {
            continue;
        }

        const auto &s_tokens = str_split(f, '_', false);

        if (s_tokens.size() == 2 && s_tokens[0] == "PERHP") {
            info_set_value(monrace.cur_hp_per, s_tokens[1]);
            continue;
        }

        if (s_tokens.size() == 2 && s_tokens[0] == "ALLIANCE") {
            for (auto a : alliance_list) {
                if (a.second->tag == s_tokens[1]) {
                    monrace.alliance_idx = static_cast<AllianceType>(a.second->id);
                }
            }
            continue;
        }

        if (s_tokens.size() == 2 && s_tokens[0] == "MOB") {
            info_set_value(monrace.max_num, s_tokens[1]);
            monrace.mob_num = monrace.max_num;
            continue;
        }

        if (s_tokens.size() == 2 && s_tokens[0] == "FATHER") {
            info_set_value(monrace.father_r_idx, s_tokens[1]);
            continue;
        }

        if (s_tokens.size() == 2 && s_tokens[0] == "MOTHER") {
            info_set_value(monrace.mother_r_idx, s_tokens[1]);
            continue;
        }

        if (s_tokens.size() == 2 && s_tokens[0] == "COLLAPSE-OVER") {
            const auto &p_tokens = str_split(s_tokens[1], '.', false);
            if (p_tokens.size() != 2) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            if (p_tokens[1].size() > 6) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            int mul = 6 - static_cast<int>(p_tokens[1].size());
            int deci, fraq;
            info_set_value(deci, p_tokens[0]);
            info_set_value(fraq, p_tokens[1]);
            for (int i = 0; i < mul; i++) {
                fraq *= 10;
            }
            monrace.collapse_over = deci * 1000000 + fraq;
            continue;
        }

        if (s_tokens.size() == 2 && s_tokens[0] == "COLLAPSE") {
            const auto &p_tokens = str_split(s_tokens[1], '.', false);
            if (p_tokens.size() != 2) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            if (p_tokens[1].size() > 6) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            int mul = 6 - static_cast<int>(p_tokens[1].size());
            int deci, fraq;
            bool is_plus = p_tokens[0][0] != '-';
            info_set_value(deci, p_tokens[0]);
            info_set_value(fraq, p_tokens[1]);
            for (int i = 0; i < mul; i++) {
                fraq *= 10;
            }
            monrace.plus_collapse = is_plus ? deci * 1000000 + fraq : deci * 1000000 - fraq;
            continue;
        }

        if (s_tokens.size() == 2 && s_tokens[0] == "SUICIDE") {
            // ターン後自滅
            int num, side;
            const auto &dices = str_split(s_tokens[1], 'd', true, 10);
            if (dices.size() != 2) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            info_set_value(num, dices[0]);
            info_set_value(side, dices[1]);
            monrace.suicide_dice_num = num;
            monrace.suicide_dice_side = side;
            continue;
        }

        if (s_tokens.size() == 6 && s_tokens[0] == "SPAWN") {
            // 落とし子自動生成率
            if (s_tokens[1] == "CREATURE" && s_tokens[3] == "IN") {
                int num;
                int deno;
                MonsterRaceId mon_idx;
                info_set_value(num, s_tokens[2]);
                info_set_value(deno, s_tokens[4]);
                info_set_value(mon_idx, s_tokens[5]);
                monrace.spawn_monsters.push_back({ num, deno, mon_idx });
                continue;
            }

            // 地形変化率
            if (s_tokens[1] == "FEATURE" && s_tokens[3] == "IN") {
                int num;
                int deno;
                FEAT_IDX feat_idx;
                info_set_value(num, s_tokens[2]);
                info_set_value(deno, s_tokens[4]);
                info_set_value(feat_idx, s_tokens[5]);
                monrace.change_feats.push_back({ num, deno, feat_idx });
                continue;
            }

            // アイテム自然ドロップ率
            if (s_tokens[1] == "ITEM" && s_tokens[3] == "IN") {
                int num;
                int deno;
                short kind_idx;
                info_set_value(num, s_tokens[2]);
                info_set_value(deno, s_tokens[4]);
                info_set_value(kind_idx, s_tokens[5]);
                monrace.spawn_items.push_back({ num, deno, kind_idx });
                continue;
            }
        }

        if (s_tokens.size() == 8 && s_tokens[0] == "DROP" && s_tokens[1] == "KIND" && s_tokens[3] == "IN") {
            int num;
            int deno;
            int dn;
            int ds;
            int grade;
            short kind_idx;
            info_set_value(num, s_tokens[2]);
            info_set_value(deno, s_tokens[4]);
            info_set_value(kind_idx, s_tokens[5]);
            info_set_value(grade, s_tokens[6]);
            const auto &dices = str_split(s_tokens[7], 'd', true, 10);
            if (dices.size() != 2) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            info_set_value(dn, dices[0]);
            info_set_value(ds, dices[1]);
            monrace.drop_kinds.push_back({ num, deno, kind_idx, grade, ds, dn });
            continue;
        }

        if (s_tokens.size() == 8 && s_tokens[0] == "DROP" && s_tokens[1] == "TVAL" && s_tokens[3] == "IN") {
            int num;
            int deno;
            int dn;
            int ds;
            int grade;
            short kind_idx;
            info_set_value(num, s_tokens[2]);
            info_set_value(deno, s_tokens[4]);
            info_set_value(kind_idx, s_tokens[5]);
            info_set_value(grade, s_tokens[6]);
            const auto &dices = str_split(s_tokens[7], 'd', true, 10);
            if (dices.size() != 2) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            info_set_value(dn, dices[0]);
            info_set_value(ds, dices[1]);
            monrace.drop_tvals.push_back({ num, deno, kind_idx, grade, ds, dn });
            continue;
        }
        if (s_tokens.size() == 7 && s_tokens[0] == "DEAD" && s_tokens[1] == "SPAWN" && s_tokens[3] == "IN") {
            int num;
            int deno;
            int dn;
            int ds;
            MonsterRaceId r_idx;
            info_set_value(num, s_tokens[2]);
            info_set_value(deno, s_tokens[4]);
            info_set_value(r_idx, s_tokens[5]);
            const auto &dices = str_split(s_tokens[6], 'd', true, 10);
            if (dices.size() != 2) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            info_set_value(dn, dices[0]);
            info_set_value(ds, dices[1]);
            monrace.dead_spawns.push_back({ num, deno, r_idx, ds, dn });
            continue;
        }
        if (!grab_one_basic_flag(r_ptr, f)) {
            return PARSE_ERROR_INVALID_FLAG;
        }
    }
}
    if (tokens[0] == "S") {
        // S:flags
        if (tokens.size() < 2 || tokens[1].size() == 0) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }

        const auto &flags = str_split(tokens[1], '|', true, 10);
        for (const auto &f : flags) {
            if (f.size() == 0) {
                continue;
            }

            const auto &s_tokens = str_split(f, '_', false, 3);

            // 特殊行動確率
            if (s_tokens.size() == 3 && s_tokens[1] == "IN") {
                if (s_tokens[0] != "1") {
                    return PARSE_ERROR_GENERIC;
                }
                RARITY i;
                info_set_value(i, s_tokens[2]);
                monrace.freq_spell = 100 / i;
                continue;
            }

            if (!grab_one_spell_flag(r_ptr, f)) {
                return PARSE_ERROR_INVALID_FLAG;
            }
        }
    } else if (tokens[0] == "A") {
        // A:artifact_idx:chance
        if (tokens.size() < 3) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }

        FixedArtifactId a_idx;
        PERCENTAGE chance;
        info_set_value(a_idx, tokens[1]);
        info_set_value(chance, tokens[2]);
        monrace.drop_artifacts.emplace_back(a_idx, chance);
    } else if (tokens[0] == "X") {
        if (tokens.size() < 2) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }
        uint32_t sex;
        if (!info_grab_one_const(sex, r_info_sex, tokens[1])) {
            return PARSE_ERROR_INVALID_FLAG;
        }
        monrace.sex = static_cast<MonsterSex>(sex);
    } else if (tokens[0] == "V") {
        // V:arena_odds
        if (tokens.size() < 2) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }

        info_set_value(monrace.arena_ratio, tokens[1]);
    } else {
        return PARSE_ERROR_UNDEFINED_DIRECTIVE;
    }
    */

    return PARSE_ERROR_NONE;
}
