﻿#include "info-reader/vault-reader.h"
#include "main/angband-headers.h"
#include "info-reader/info-reader-util.h"
#include "info-reader/parse-error-types.h"
#include "room/rooms-vault.h"
#include "util/string-processor.h"

/*!
 * @brief Vault情報(v_info)のパース関数 /
 * Initialize the "v_info" array, by parsing an ascii "template" file
 * @param buf テキスト列
 * @param head ヘッダ構造体
 * @return エラーコード
 */
errr parse_v_info(std::string_view buf, angband_header *head)
{
    static vault_type *v_ptr = NULL;
    const auto &tokens = str_split(buf, ':', false, 5);

    if (tokens[0] == "N") {
        // N:index:name
        if (tokens.size() < 3)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        if (tokens[1].size() == 0 || tokens[2].size() == 0)
            return PARSE_ERROR_GENERIC;

        auto i = std::stoi(tokens[1]);
        if (i < error_idx)
            return PARSE_ERROR_NON_SEQUENTIAL_RECORDS;
        if (i >= head->info_num)
            return PARSE_ERROR_OUT_OF_BOUNDS;

        error_idx = i;
        v_ptr = &v_info[i];
        v_ptr->name = std::string(tokens[2]);
    } else if (!v_ptr)
        return PARSE_ERROR_MISSING_RECORD_HEADER;
    else if (tokens[0] == "D") {
        // D:MapText
        if (tokens.size() < 2 || tokens[1].size() == 0)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        v_ptr->text.append(buf.substr(2));
    } else if (tokens[0] == "X") {
        // X:type:rate:height:width
        if (tokens.size() < 5)
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;

        info_set_value(v_ptr->typ, tokens[1]);
        info_set_value(v_ptr->rat, tokens[2]);
        info_set_value(v_ptr->hgt, tokens[3]);
        info_set_value(v_ptr->wid, tokens[4]);
    } else if (tokens[0] == "F") {
        if (tokens.size() < 3) {
            return PARSE_ERROR_TOO_FEW_ARGUMENTS;
        }
        char c;
        FEAT_IDX feat_idx;
        c = tokens[1].c_str()[0];
        info_set_value(feat_idx, tokens[2]);
        v_ptr->feature_list[c] = feat_idx;
    } else
        return PARSE_ERROR_UNDEFINED_DIRECTIVE;

    return PARSE_ERROR_NONE;
}
