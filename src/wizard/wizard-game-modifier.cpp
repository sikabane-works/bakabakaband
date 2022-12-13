﻿/*!
 * @brief ゲーム属性を変更するデバッグコマンド
 * @date 2021/03/07
 */

#include "wizard/wizard-game-modifier.h"
#include "core/asking-player.h"
#include "dungeon/quest.h"
#include "info-reader/fixed-map-parser.h"
#include "io/input-key-requester.h"
#include "market/arena.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags1.h"
#include "monster-race/race-flags7.h"
#include "player-info/self-info.h"
#include "system/floor-type-definition.h"
#include "system/monster-race-definition.h"
#include "system/player-type-definition.h"
#include "system/system-variables.h"
#include "term/screen-processor.h"
#include "util/bit-flags-calculator.h"
#include "util/int-char-converter.h"
#include "view/display-messages.h"
#include "wizard/wizard-special-process.h"
#include <array>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

void wiz_enter_quest(PlayerType *player_ptr);
void wiz_complete_quest(PlayerType *player_ptr);
void wiz_restore_monster_max_num(MonsterRaceId r_idx);

/*!
 * @brief ゲーム設定コマンド一覧表
 */
constexpr std::array wizard_game_modifier_menu_table = {
    std::make_tuple('t', _("プレイ時間変更", "Modify played time")),
    std::make_tuple('q', _("現在のクエストを完了", "Complete current quest")),
    std::make_tuple('Q', _("クエストに突入", "Enter quest")),
    std::make_tuple('u', _("ユニーク/ナズグルの生存数を復元", "Restore living info of unique/nazgul")),
    std::make_tuple('g', _("モンスター闘技場出場者更新", "Update gambling monster")),
};

/*!
 * @brief ゲーム設定コマンドの一覧を表示する
 */
void display_wizard_game_modifier_menu()
{
    for (auto y = 1U; y <= wizard_game_modifier_menu_table.size(); y++) {
        term_erase(14, y, 64);
    }

    int r = 1;
    int c = 15;
    for (const auto &[symbol, desc] : wizard_game_modifier_menu_table) {
        std::stringstream ss;
        ss << symbol << ") " << desc;
        put_str(ss.str().c_str(), r++, c);
    }
}

/*!
 * @brief ゲーム設定コマンドの入力を受け付ける
 * @param player_ptr プレイヤーの情報へのポインタ
 */
void wizard_game_modifier(PlayerType *player_ptr)
{
    screen_save();
    display_wizard_game_modifier_menu();

    char cmd;
    get_com("Player Command: ", &cmd, false);
    screen_load();

    switch (cmd) {
    case ESCAPE:
    case ' ':
    case '\n':
    case '\r':
        break;
    case 'g':
        update_gambling_monsters(player_ptr);
        break;
    case 'q':
        wiz_complete_quest(player_ptr);
        break;
    case 'Q':
        wiz_enter_quest(player_ptr);
        break;
    case 'u':
        wiz_restore_monster_max_num(i2enum<MonsterRaceId>(command_arg));
        break;
    case 't':
        set_gametime();
        break;
    }
}

/*!
 * @brief 指定したクエストに突入する
 * @param プレイヤーの情報へのポインタ
 */
void wiz_enter_quest(PlayerType *player_ptr)
{
    char ppp[30];
    char tmp_val[5];
    int tmp_int;
    sprintf(ppp, "QuestID (0-%d):", max_q_idx - 1);
    sprintf(tmp_val, "%d", 0);

    if (!get_string(ppp, tmp_val, 3)) {
        return;
    }

    tmp_int = atoi(tmp_val);
    if ((tmp_int < 0) || (tmp_int >= max_q_idx)) {
        return;
    }

    auto q_idx = i2enum<QuestId>(tmp_int);

    init_flags = i2enum<init_flags_type>(INIT_SHOW_TEXT | INIT_ASSIGN);
    player_ptr->current_floor_ptr->quest_number = q_idx;
    parse_fixed_map(player_ptr, "q_info.txt", 0, 0, 0, 0);
    quest_map[q_idx].status = QuestStatusType::TAKEN;
    if (quest_map[q_idx].dungeon == 0) {
        exe_enter_quest(player_ptr, q_idx);
    }
}

/*!
 * @brief 指定したクエストを完了させる
 * @param プレイヤーの情報へのポインタ
 */
void wiz_complete_quest(PlayerType *player_ptr)
{
    if (!inside_quest(player_ptr->current_floor_ptr->quest_number)) {
        msg_print("No current quest");
        msg_print(nullptr);
        return;
    }

    if (quest_map[player_ptr->current_floor_ptr->quest_number].status == QuestStatusType::TAKEN) {
        complete_quest(player_ptr, player_ptr->current_floor_ptr->quest_number);
    }
}

void wiz_restore_monster_max_num(MonsterRaceId r_idx)
{
    if (!is_valid_monster_race(r_idx)) {
        int val;
        if (!get_value("MonsterID", 1, r_info.size() - 1, &val)) {
            return;
        }
        r_idx = static_cast<MonsterRaceId>(val);
    }

    auto *r_ptr = &r_info[r_idx];
    if (r_ptr->name.empty()) {
        msg_print("そのモンスターは存在しません。");
        msg_print(nullptr);
        return;
    }

    auto n = 0;
    if (r_ptr->kind_flags.has(MonsterKindType::UNIQUE)) {
        n = 1;
    } else if (any_bits(r_ptr->flags7, RF7_NAZGUL)) {
        n = MAX_NAZGUL_NUM;
    }

    if (n == 0) {
        msg_print("出現数に制限がないモンスターです。");
        msg_print(nullptr);
        return;
    }

    r_ptr->mob_num = n;
    r_ptr->r_pkills = 0;
    r_ptr->r_akills = 0;

    std::stringstream ss;
    ss << r_ptr->name << _("の出現数を復元しました。", " can appear again now.");
    msg_print(ss.str().c_str());
    msg_print(nullptr);
}
