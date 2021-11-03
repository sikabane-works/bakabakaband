#include "alliance/alliance.h"
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
 * @brief モンスターパーティ情報(party_info)のパース関数 /
 * Initialize the "party_info" array, by parsing an ascii "template" file
 * @param buf テキスト列
 * @param head ヘッダ構造体
 * @return エラーコード
 */
errr parse_party_info(std::string_view buf, angband_header *)
{
    static monster_race *r_ptr = nullptr;
    const auto &tokens = str_split(buf, ':', true, 10);
    return PARSE_ERROR_NONE;
}
