﻿#include "alliance/alliance.h"
#include "info-reader/race-reader.h"
#include "info-reader/info-reader-util.h"
#include "info-reader/parse-error-types.h"
#include "info-reader/race-info-tokens-table.h"
#include "main/angband-headers.h"
#include "monster-race/monster-race.h"
#include "player-ability/player-ability-types.h"
#include "system/monster-race-definition.h"
#include "term/gameterm.h"
#include "util/string-processor.h"
#include "view/display-messages.h"

/*!
 * @brief テキストトークンを走査してフラグを一つ得る(モンスター用1) /
 * Grab one (basic) flag in a monster_race from a textual string
 * @param r_ptr 保管先のモンスター種族構造体参照ポインタ
 * @param what 参照元の文字列ポインタ
 * @return 見つけたらtrue
 */
static bool grab_one_basic_flag(monster_race *r_ptr, std::string_view what)
{
    if (info_grab_one_flag(r_ptr->flags1, r_info_flags1, what))
        return true;

    if (info_grab_one_flag(r_ptr->flags2, r_info_flags2, what))
        return true;

    if (info_grab_one_flag(r_ptr->flags3, r_info_flags3, what))
        return true;

    if (info_grab_one_flag(r_ptr->flags7, r_info_flags7, what))
        return true;

    if (info_grab_one_flag(r_ptr->flags8, r_info_flags8, what))
        return true;

    if (info_grab_one_flag(r_ptr->flags9, r_info_flags9, what))
        return true;

    if (info_grab_one_flag(r_ptr->flagsr, r_info_flagsr, what))
        return true;

    if (EnumClassFlagGroup<MonsterAuraType>::grab_one_flag(r_ptr->aura_flags, r_info_aura_flags, what)) {
        return true;
    }

    msg_format(_("未知のモンスター・フラグ '%s'。", "Unknown monster flag '%s'."), what.data());
    return false;
}

/*!
 * @brief テキストトークンを走査してフラグを一つ得る(モンスター用2) /
 * Grab one (spell) flag in a monster_race from a textual string
 * @param r_ptr 保管先のモンスター種族構造体参照ポインタ
 * @param what 参照元の文字列ポインタ
 * @return 見つけたらtrue
 */
static bool grab_one_spell_flag(monster_race *r_ptr, std::string_view what)
{
    if (EnumClassFlagGroup<MonsterAbilityType>::grab_one_flag(r_ptr->ability_flags, r_info_ability_flags, what)) {
        return true;
    }

    msg_format(_("未知のモンスター・フラグ '%s'。", "Unknown monster flag '%s'."), what.data());
    return false;
}

/*!
 * @brief モンスター種族情報(r_info)のパース関数 /
 * Initialize the "r_info" array, by parsing an ascii "template" file
 * @param buf テキスト列
 * @param head ヘッダ構造体
 * @return エラーコード
 */
errr parse_r_info(std::string_view buf, angband_header *)
{
    static monster_race *r_ptr = nullptr;
    const auto &tokens = str_split(buf, ':', true, 10);

    if (tokens[0] == "N") {
        // N:index:name_ja
        if (tokens.size() < 3 || tokens[1].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        auto i = std::stoi(tokens[1]);
        if (i < error_idx)
            return PARSE_ERROR_NON_SEQUENTIAL_RECORDS;
        if (i >= static_cast<int>(r_info.size())) {
            r_info.resize(i + 1);
        }

        error_idx = i;
        r_ptr = &r_info[i];
        r_ptr->idx = static_cast<MONRACE_IDX>(i);
#ifdef JP
        r_ptr->name = tokens[2];
#endif
    } else if (!r_ptr)
        return PARSE_ERROR_MISSING_RECORD_HEADER;
    else if (tokens[0] == "E") {
        // E:name_en
        if (tokens.size() < 2 || tokens[1].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
#ifdef JP
        r_ptr->E_name = tokens[1];
#else
        r_ptr->name = tokens[1];
#endif
    } else if (tokens[0] == "T") {
        if (tokens.size() < 2 || tokens[1].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        r_ptr->tag = tokens[1];
    } else if (tokens[0] == "D") {
        // D:text_ja
        // D:$text_en
        if (tokens.size() < 2 || tokens[1].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
#ifdef JP
        if (tokens[1][0] == '$')
            return PARSE_ERROR_NONE;
        r_ptr->text.append(buf.substr(2));
#else
        if (tokens[1][0] != '$')
            return PARSE_ERROR_NONE;
        append_english_text(r_ptr->text, buf.substr(3));
#endif
    } else if (tokens[0] == "G") {
        // G:color:symbol
        if (tokens.size() < 3 || tokens[1].size() == 0 || tokens[2].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        auto a = color_char_to_attr(tokens[2][0]);
        if (a > 127)
            return PARSE_ERROR_GENERIC;

        r_ptr->d_attr = a;
        r_ptr->d_char = tokens[1][0];
    } else if (tokens[0] == "I") {
        // G:speed:hp_dice:affect_range:ac:sleep_degree
        if (tokens.size() < 6)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        const auto &dice = str_split(tokens[2], 'd', false, 2);
        if (dice.size() < 2)
            return PARSE_ERROR_GENERIC;

        info_set_value(r_ptr->speed, tokens[1]);
        info_set_value(r_ptr->hdice, dice[0]);
        info_set_value(r_ptr->hside, dice.size() == 1 ? "1" : dice[1]);
        info_set_value(r_ptr->aaf, tokens[3]);
        info_set_value(r_ptr->ac, tokens[4]);
        info_set_value(r_ptr->sleep, tokens[5]);
    } else if (tokens[0] == "W") {
        // W:level:ratity:extra:exp:next_exp:next_id
        if (tokens.size() < 5 || tokens.size() == 6)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        info_set_value(r_ptr->level, tokens[1]);
        info_set_value(r_ptr->rarity, tokens[2]);
        info_set_value(r_ptr->extra, tokens[3]);
        info_set_value(r_ptr->mexp, tokens[4]);

        if (tokens.size() < 6)
            return PARSE_ERROR_NONE;

        info_set_value(r_ptr->next_exp, tokens[5]);
        info_set_value(r_ptr->next_r_idx, tokens[6]);
    } else if (tokens[0] == "R") {
        MONSTER_IDX mon_idx;
        DICE_NUMBER dn;
        DICE_SID ds;

        if (tokens.size() < 3)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        if (tokens[1].size() == 0 || tokens[2].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        info_set_value(mon_idx, tokens[1]);
        const auto &dice = str_split(tokens[2], 'd', false, 2);
        info_set_value(dn, dice[0]);
        info_set_value(ds, dice[1]);
        r_ptr->reinforces.push_back({ mon_idx, dn, ds });
    } else if (tokens[0] == "B") {
        // B:blow_type:blow_effect:dice
        size_t i = 0;
        for (; i < 4; i++) {
            if (r_ptr->blow[i].method == RaceBlowMethodType::NONE)
                break;
        }

        if (i >= 4)
            return PARSE_ERROR_GENERIC;

        if (tokens.size() < 3)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        if (tokens[1].size() == 0 || tokens[2].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        auto rbm = r_info_blow_method.find(tokens[1]);
        if (rbm == r_info_blow_method.end())
            return PARSE_ERROR_INVALID_FLAG;

        auto rbe = r_info_blow_effect.find(tokens[2]);
        if (rbe == r_info_blow_effect.end())
            return PARSE_ERROR_INVALID_FLAG;

        r_ptr->blow[i].method = rbm->second;
        r_ptr->blow[i].effect = rbe->second;

        if (tokens.size() < 4)
            return PARSE_ERROR_NONE;

        const auto &dice = str_split(tokens[3], 'd', false, 2);
        info_set_value(r_ptr->blow[i].d_dice, dice[0]);
        info_set_value(r_ptr->blow[i].d_side, dice[1]);
    } else if (tokens[0] == "F") {
        // F:flags
        if (tokens.size() < 2 || tokens[1].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        const auto &flags = str_split(tokens[1], '|', true, 10);
        for (const auto &f : flags) {

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
                for(int i = 0; i < mul; i++) {
                    fraq *= 10;
                }
                r_ptr->plus_collapse = is_plus ? deci * 1000000 + fraq : deci * 1000000 - fraq;
                continue;
            }

            if (s_tokens.size() == 6 && s_tokens[0] == "SPAWN") {
                // 落とし子自動生成率
                if (s_tokens[1] == "CREATURE" && s_tokens[3] == "IN") {
                    int num;
                    int deno;
                    MONRACE_IDX mon_idx;
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
                    KIND_OBJECT_IDX kind_idx;
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
                KIND_OBJECT_IDX kind_idx;
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

            if (s_tokens.size() == 7 && s_tokens[0] == "DEAD" && s_tokens[1] == "SPAWN" && s_tokens[3] == "IN") {
                int num;
                int deno;
                int dn;
                int ds;
                KIND_OBJECT_IDX kind_idx;
                info_set_value(num, s_tokens[2]);
                info_set_value(deno, s_tokens[4]);
                info_set_value(kind_idx, s_tokens[5]);
                const auto& dices = str_split(s_tokens[6], 'd', true, 10);
                if (dices.size() != 2) {
                    return PARSE_ERROR_INVALID_FLAG;
                }
                info_set_value(dn, dices[0]);
                info_set_value(ds, dices[1]);
                r_ptr->dead_spawns.push_back({ num, deno, kind_idx, ds, dn });
                continue;
            }
            if (!grab_one_basic_flag(r_ptr, f))
                return PARSE_ERROR_INVALID_FLAG;
        }

    } else if (tokens[0] == "S") {
        // S:flags
        if (tokens.size() < 2 || tokens[1].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        const auto &flags = str_split(tokens[1], '|', true, 10);
        for (const auto &f : flags) {
            if (f.size() == 0)
                continue;

            const auto &s_tokens = str_split(f, '_', false, 3);

            // 特殊行動確率
            if (s_tokens.size() == 3 && s_tokens[1] == "IN") {
                if (s_tokens[0] != "1")
                    return PARSE_ERROR_GENERIC;
                RARITY i;
                info_set_value(i, s_tokens[2]);
                r_ptr->freq_spell = 100 / i;
                continue;
            }


            if (!grab_one_spell_flag(r_ptr, f))
                return PARSE_ERROR_INVALID_FLAG;
        }

    } else if (tokens[0] == "A") {
        // A:artifact_idx:rarity:percent
        size_t i = 0;
        for (; i < 4; i++) {
            if (!r_ptr->artifact_id[i])
                break;
        }
        if (i >= 4)
            return PARSE_ERROR_GENERIC;

        if (tokens.size() < 4)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        info_set_value(r_ptr->artifact_id[i], tokens[1]);
        info_set_value(r_ptr->artifact_rarity[i], tokens[2]);
        info_set_value(r_ptr->artifact_percent[i], tokens[3]);
    } else if (tokens[0] == "V") {
        // V:arena_odds
        if (tokens.size() < 2)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        info_set_value(r_ptr->arena_ratio, tokens[1]);
    } else
        return PARSE_ERROR_UNDEFINED_DIRECTIVE;

    return PARSE_ERROR_NONE;
}
