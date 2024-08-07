#include "info-reader/race-reader.h"
#include "alliance/alliance.h"
#include "artifact/fixed-art-types.h"
#include "info-reader/info-reader-util.h"
#include "info-reader/parse-error-types.h"
#include "info-reader/race-info-tokens-table.h"
#include "locale/japanese.h"
#include "main/angband-headers.h"
#include "monster-race/monster-race.h"
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

    if (EnumClassFlagGroup<MonsterSpecialType>::grab_one_flag(monrace.special_flags, r_info_special_flags, what)) {
        return true;
    }
    if (EnumClassFlagGroup<MonsterMiscType>::grab_one_flag(monrace.misc_flags, r_info_misc_flags, what)) {
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
            r_ptr->tag = tokens[1];
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
    monrace.d_char = character_obj.get<std::string>().front();
    monrace.d_attr = color->second;

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
    if (!speed_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto speed = speed_data.get<int8_t>();
    if (speed < -50 || speed > 99) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.speed = speed + STANDARD_SPEED;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスター体力をセットする
 * @param hp_data 体力情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_hp(const nlohmann::json &hp_data, MonsterRaceInfo &monrace)
{
    if (!hp_data.is_string()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto hit_point = hp_data.get<std::string>();
    const auto &dice = str_split(hit_point, 'd', false, 2);
    if (dice.size() < 2) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }
    monrace.hdice = std::stoi(dice[0]);
    monrace.hside = std::stoi(dice[1]);
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの感知範囲をセットする
 * @param vision_data 感知範囲情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_vision(const nlohmann::json &vision_data, MonsterRaceInfo &monrace)
{
    if (!vision_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto vision = vision_data.get<int>();
    if (vision < 0 || vision > 999) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.aaf = vision;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの物理防御力をセットする
 * @param ac_data 物理防御力情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_ac(const nlohmann::json &ac_data, MonsterRaceInfo &monrace)
{
    if (!ac_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto armour_class = ac_data.get<short>();
    if (armour_class < 0 || armour_class > 10000) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.ac = armour_class;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの警戒度をセットする
 * @param alertness_data 物理防御力情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_alertness(const nlohmann::json &alertness_data, MonsterRaceInfo &monrace)
{
    if (!alertness_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto alertness = alertness_data.get<short>();
    if (alertness < 0 || alertness > 255) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.sleep = alertness;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの出現階層をセットする
 * @param level_data 出現階層情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_level(const nlohmann::json &level_data, MonsterRaceInfo &monrace)
{
    if (!level_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto level = level_data.get<int>();
    if (level < 0 || level > 255) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.level = level;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの出現階層をセットする
 * @param rarity_data 出現階層情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_rarity(const nlohmann::json &rarity_data, MonsterRaceInfo &monrace)
{
    if (!rarity_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto rarity = rarity_data.get<int>();
    if (rarity < 0 || rarity > 255) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.rarity = (RARITY)rarity;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの経験値をセットする
 * @param exp_data 経験値情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_exp(const nlohmann::json &exp_data, MonsterRaceInfo &monrace)
{
    if (!exp_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto exp = exp_data.get<int>();
    if (exp < 0 || exp > 9999999) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.mexp = exp;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの進化をセットする
 * @param evolve_data 進化情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_evolve(const nlohmann::json &evolve_data, MonsterRaceInfo &monrace)
{
    if (evolve_data.is_null()) {
        return PARSE_ERROR_NONE;
    }

    const auto &need_exp_obj = evolve_data["need_exp"];
    const auto &evlove_to_obj = evolve_data["to"];
    if (!need_exp_obj.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }
    if (!evlove_to_obj.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }
    const auto need_exp = need_exp_obj.get<int>();
    const auto evlove_to = evlove_to_obj.get<int>();
    if (need_exp <= 0 || need_exp > 9999999) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    if (evlove_to <= 0 || evlove_to > 9999) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.next_exp = need_exp;
    monrace.next_r_idx = static_cast<MonsterRaceId>(evlove_to);
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの性別をセットする
 * @param sex_data 性別情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_sex(const nlohmann::json &sex_data, MonsterRaceInfo &monrace)
{
    if (sex_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!sex_data.is_string()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    uint32_t sex;
    if (!info_grab_one_const(sex, r_info_sex, sex_data.get<std::string>())) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.sex = static_cast<MonsterSex>(sex);
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの闘技場オッズをセットする
 * @param odds_data オッズ情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_odds_rate(const nlohmann::json &odds_data, MonsterRaceInfo &monrace)
{
    if (odds_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!odds_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto odds = odds_data.get<int>();
    if (odds <= 0 || odds > 9999) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.arena_ratio = odds;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの初期体力をセットする
 * @param start_hp_data 初期体力情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_start_hp_percentage(const nlohmann::json &start_hp_data, MonsterRaceInfo &monrace)
{
    if (start_hp_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!start_hp_data.is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto start_hp = start_hp_data.get<int>();
    if (start_hp < 0 || start_hp > 99) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.cur_hp_per = start_hp;
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの固定アーティファクトドロップ情報をセットする
 * @param artifact_data 固定アーティファクトドロップ情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_artifacts(const nlohmann::json &artifact_data, MonsterRaceInfo &monrace)
{
    if (artifact_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!artifact_data.is_array()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    for (auto &artifact : artifact_data.items()) {
        const auto &artifact_id_obj = artifact.value()["drop_artifact_id"];
        const auto &artifact_chance_obj = artifact.value()["drop_probability"];

        if (!artifact_id_obj.is_number_integer()) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }
        if (!artifact_chance_obj.is_number_integer()) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }
        const auto artifact_id = artifact_id_obj.get<int>();
        if (artifact_id < 0 || artifact_id > 1024) {
            return PARSE_ERROR_INVALID_FLAG;
        }
        const auto a_idx = static_cast<FixedArtifactId>(artifact_id);
        const auto artifact_chance = artifact_chance_obj.get<int>();
        if (artifact_chance <= 0 || artifact_chance > 100) {
            return PARSE_ERROR_INVALID_FLAG;
        }
        monrace.drop_artifacts.emplace_back(a_idx, artifact_chance);
    }
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの護衛情報をセットする
 * @param escort_data 護衛情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_escorts(const nlohmann::json &escort_data, MonsterRaceInfo &monrace)
{
    if (escort_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!escort_data.is_array()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    for (auto &escort : escort_data.items()) {
        const auto &escorts_id_obj = escort.value()["escorts_id"];
        const auto &escort_num_obj = escort.value()["escort_num"];
        if (!escorts_id_obj.is_number_integer()) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }
        if (!escort_num_obj.is_string()) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }

        const auto escorts_id = escorts_id_obj.get<int>();
        if (escorts_id < 0 || escorts_id > 8192) {
            return PARSE_ERROR_INVALID_FLAG;
        }

        const auto monrace_id = static_cast<MonsterRaceId>(escorts_id);
        const auto escort_num = escort_num_obj.get<std::string>();
        const auto &dice = str_split(escort_num, 'd', false, 2);
        DICE_NUMBER dd;
        DICE_SID ds;
        info_set_value(dd, dice[0]);
        info_set_value(ds, dice[1]);
        monrace.reinforces.emplace_back(monrace_id, dd, ds);
    }
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの打撃攻撃をセットする
 * @param blow_data 打撃攻撃情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_blows(const nlohmann::json &blow_data, MonsterRaceInfo &monrace)
{
    if (blow_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!blow_data.is_array()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    auto blow_num = 0;
    for (auto &blow : blow_data.items()) {
        if (blow_num > 5) {
            return PARSE_ERROR_GENERIC;
        }

        const auto &blow_method = blow.value()["method"];
        const auto &blow_effect = blow.value()["effect"];
        if (blow_method.is_null() || blow_effect.is_null()) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }

        const auto rbm = r_info_blow_method.find(blow_method.get<std::string>());
        if (rbm == r_info_blow_method.end()) {
            return PARSE_ERROR_INVALID_FLAG;
        }

        const auto rbe = r_info_blow_effect.find(blow_effect.get<std::string>());
        if (rbe == r_info_blow_effect.end()) {
            return PARSE_ERROR_INVALID_FLAG;
        }
        monrace.blows[blow_num].method = rbm->second;
        monrace.blows[blow_num].effect = rbe->second;

        const auto &blow_dice = blow.value().find("damage_dice");
        if (blow_dice != blow.value().end()) {
            const auto &dice = str_split(blow_dice->get<std::string>(), 'd', false, 2);
            info_set_value(monrace.blows[blow_num].d_dice, dice[0]);
            info_set_value(monrace.blows[blow_num].d_side, dice[1]);
        }

        blow_num++;
    }
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターフラグをセットする
 * @param flag_data モンスターフラグ情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_flags(const nlohmann::json &flag_data, MonsterRaceInfo &monrace)
{
    if (flag_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!flag_data.is_array()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    /*
                if (f.size() == 0) {
                    continue;
                }

                const auto &s_tokens = str_split(f, '_', false);

                if (s_tokens.size() == 2 && s_tokens[0] == "PERHP") {
                    info_set_value(r_ptr->cur_hp_per, s_tokens[1]);
                    continue;
                }

                if (s_tokens.size() == 2 && s_tokens[0] == "ALLIANCE") {
                    for (auto a : alliance_list) {
                        if (a.second->tag == s_tokens[1]) {
                            r_ptr->alliance_idx = static_cast<AllianceType>(a.second->id);
                        }
                    }
                    continue;
                }

                if (s_tokens.size() == 2 && s_tokens[0] == "MOB") {
                    info_set_value(r_ptr->max_num, s_tokens[1]);
                    r_ptr->mob_num = r_ptr->max_num;
                    continue;
                }

                if (s_tokens.size() == 2 && s_tokens[0] == "FATHER") {
                    info_set_value(r_ptr->father_r_idx, s_tokens[1]);
                    continue;
                }

                if (s_tokens.size() == 2 && s_tokens[0] == "MOTHER") {
                    info_set_value(r_ptr->mother_r_idx, s_tokens[1]);
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
                    r_ptr->collapse_over = deci * 1000000 + fraq;
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
                    r_ptr->plus_collapse = is_plus ? deci * 1000000 + fraq : deci * 1000000 - fraq;
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
                    r_ptr->suicide_dice_num = num;
                    r_ptr->suicide_dice_side = side;
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
                        r_ptr->spawn_monsters.push_back({ num, deno, mon_idx });
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
                        r_ptr->change_feats.push_back({ num, deno, feat_idx });
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
                        r_ptr->spawn_items.push_back({ num, deno, kind_idx });
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
                    r_ptr->drop_kinds.push_back({ num, deno, kind_idx, grade, ds, dn });
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
                    r_ptr->drop_tvals.push_back({ num, deno, kind_idx, grade, ds, dn });
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
                    r_ptr->dead_spawns.push_back({ num, deno, r_idx, ds, dn });
                    continue;
                }

                if (grab_one_basic_flag(r_ptr, f)) {
                    continue;
                }

                uint32_t sex;
                if (!info_grab_one_const(sex, r_info_sex, f)) {
                    return PARSE_ERROR_INVALID_FLAG;
                }

                r_ptr->sex = static_cast<MonsterSex>(sex);
            }

        } else if (tokens[0] == "S") {
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
                    r_ptr->freq_spell = 100 / i;
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
            r_ptr->drop_artifacts.emplace_back(a_idx, chance);
        } else if (tokens[0] == "X") {
            if (tokens.size() < 2) {
                return PARSE_ERROR_TOO_FEW_ARGUMENTS;
            }
            uint32_t sex;
            if (!info_grab_one_const(sex, r_info_sex, tokens[1])) {
                return PARSE_ERROR_INVALID_FLAG;
            }
            r_ptr->sex = static_cast<MonsterSex>(sex);
        } else if (tokens[0] == "V") {
            // V:arena_odds
            if (tokens.size() < 2) {
                return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    */
    for (auto &flag : flag_data.items()) {
        if (!grab_one_basic_flag(monrace, flag.value().get<std::string>())) {
            return PARSE_ERROR_INVALID_FLAG;
        }
    }
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの発動能力をセットする
 * @param skill_data 発動能力情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_skills(const nlohmann::json &skill_data, MonsterRaceInfo &monrace)
{
    if (skill_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!skill_data.is_object()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto &prob = skill_data["probability"];
    if (!prob.is_string()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto prob_str = prob.get<std::string>();
    const auto &prob_token = str_split(prob_str, '_', false, 2);
    if (prob_token.size() == 3 && prob_token[1] == "IN") {
        if (prob_token[0] != "1") {
            return PARSE_ERROR_GENERIC;
        }
        byte denominator;
        info_set_value(denominator, prob_token[2]);
        monrace.freq_spell = 100 / denominator;
    }

    const auto &skill_list = skill_data["list"];
    if (!skill_list.is_array()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    for (auto &skill : skill_list.items()) {
        if (!grab_one_spell_flag(monrace, skill.value().get<std::string>())) {
            return PARSE_ERROR_INVALID_FLAG;
        }
    }
    return PARSE_ERROR_NONE;
}

/*!
 * @brief JSON Objectからモンスターの説明文をセットする
 * @param flag_data 説明文の情報の格納されたJSON Object
 * @param monrace 保管先のモンスター種族構造体
 * @return エラーコード
 */
static errr set_mon_flavor(const nlohmann::json &flavor_data, MonsterRaceInfo &monrace)
{
    if (flavor_data.is_null()) {
        return PARSE_ERROR_NONE;
    }
    if (!flavor_data.is_object()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

#ifdef JP
    const auto &flavor_ja = flavor_data.find("ja");
    if (flavor_ja == flavor_data.end()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }
    auto flavor_ja_sys = utf8_to_sys(flavor_ja->get<std::string>());
    if (!flavor_ja_sys) {
        return PARSE_ERROR_INVALID_FLAG;
    }
    monrace.text = std::move(*flavor_ja_sys);
#else
    const auto &flavor_en = flavor_data.find("en");
    if (flavor_en == flavor_data.end()) {
        return PARSE_ERROR_NONE;
    }
    monrace.text = flavor_en->get<std::string>();
#endif
    return PARSE_ERROR_NONE;
}

/*!
 * @brief モンスター種族情報(JSON Object)のパース関数
 * @param mon_data モンスターデータの格納されたJSON Object
 * @param head ヘッダ構造体
 * @return エラーコード
 */
errr parse_monraces_info(nlohmann::json &mon_data, angband_header *)
{
    if (!mon_data["id"].is_number_integer()) {
        return PARSE_ERROR_TOO_FEW_ARGUMENTS;
    }

    const auto monster_idx = mon_data["id"].get<int>();
    if (monster_idx < error_idx) {
        return PARSE_ERROR_NON_SEQUENTIAL_RECORDS;
    }
    error_idx = monster_idx;
    auto &monrace = monraces_info.emplace_hint(monraces_info.end(), i2enum<MonsterRaceId>(monster_idx), MonsterRaceInfo{})->second;
    monrace.idx = i2enum<MonsterRaceId>(monster_idx);

    errr err;
    err = set_mon_name(mon_data["name"], monrace);
    if (err) {
        msg_format(_("モンスター名読込失敗。ID: '%d'。", "Failed to load monster name. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_symbol(mon_data["symbol"], monrace);
    if (err) {
        msg_format(_("モンスターシンボル読込失敗。ID: '%d'。", "Failed to load monster symbol. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_speed(mon_data["speed"], monrace);
    if (err) {
        msg_format(_("モンスター速度読込失敗。ID: '%d'。", "Failed to load monster speed. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_hp(mon_data["hit_point"], monrace);
    if (err) {
        msg_format(_("モンスターHP読込失敗。ID: '%d'。", "Failed to load monster HP. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_vision(mon_data["vision"], monrace);
    if (err) {
        msg_format(_("モンスター感知範囲読込失敗。ID: '%d'。", "Failed to load monster vision. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_ac(mon_data["armor_class"], monrace);
    if (err) {
        msg_format(_("モンスターAC読込失敗。ID: '%d'。", "Failed to load monster AC. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_alertness(mon_data["alertness"], monrace);
    if (err) {
        msg_format(_("モンスター警戒度読込失敗。ID: '%d'。", "Failed to load monster alertness. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_level(mon_data["level"], monrace);
    if (err) {
        msg_format(_("モンスターレベル読込失敗。ID: '%d'。", "Failed to load monster level. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_rarity(mon_data["rarity"], monrace);
    if (err) {
        msg_format(_("モンスター希少度読込失敗。ID: '%d'。", "Failed to load monster rarity. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_exp(mon_data["exp"], monrace);
    if (err) {
        msg_format(_("モンスター経験値読込失敗。ID: '%d'。", "Failed to load monster exp. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_evolve(mon_data["evolve"], monrace);
    if (err) {
        msg_format(_("モンスター進化情報読込失敗。ID: '%d'。", "Failed to load monster evolve data. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_sex(mon_data["sex"], monrace);
    if (err) {
        msg_format(_("モンスター性別読込失敗。ID: '%d'。", "Failed to load monster sex. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_odds_rate(mon_data["odds_correction_ratio"], monrace);
    if (err) {
        msg_format(_("モンスター賭け倍率読込失敗。ID: '%d'。", "Failed to load monster odds for arena. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_start_hp_percentage(mon_data["start_hp_percentage"], monrace);
    if (err) {
        msg_format(_("モンスター初期体力読込失敗。ID: '%d'。", "Failed to load monster starting HP. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_artifacts(mon_data["artifacts"], monrace);
    if (err) {
        msg_format(_("モンスター固定アーティファクトドロップ情報読込失敗。ID: '%d'。", "Failed to load monster artifact drop data. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_escorts(mon_data["escorts"], monrace);
    if (err) {
        msg_format(_("モンスター護衛情報読込失敗。ID: '%d'。", "Failed to load monster escorts. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_blows(mon_data["blows"], monrace);
    if (err) {
        msg_format(_("モンスター打撃情報読込失敗。ID: '%d'。", "Failed to load monster blow data. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_flags(mon_data["flags"], monrace);
    if (err) {
        msg_format(_("モンスターフラグ読込失敗。ID: '%d'。", "Failed to load monster flag data. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_skills(mon_data["skill"], monrace);
    if (err) {
        msg_format(_("モンスター発動能力情報読込失敗。ID: '%d'。", "Failed to load monster skill data. ID: '%d'."), error_idx);
        return err;
    }
    err = set_mon_flavor(mon_data["flavor"], monrace);
    if (err) {
        msg_format(_("モンスター説明文読込失敗。ID: '%d'。", "Failed to load monster flavor text. ID: '%d'."), error_idx);
        return err;
    }

    return PARSE_ERROR_NONE;
}
