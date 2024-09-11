/*!
 * @brief ダンジョンの生成 / Dungeon generation
 * @date 2014/01/04
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke\n
 * This software may be copied and distributed for educational, research,\n
 * and not for profit purposes provided that this copyright and statement\n
 * are included in all such copies.  Other copyrights may also apply.\n
 * 2014 Deskull rearranged comment for Doxygen. \n
 */

#include "floor/floor-generator.h"
#include "dungeon/dungeon-flag-types.h"
#include "dungeon/quest.h"
#include "floor/cave-generator.h"
#include "floor/floor-base-definitions.h"
#include "floor/floor-events.h"
#include "floor/floor-generator.h"
#include "floor/floor-save.h" //!< @todo precalc_cur_num_of_pet() が依存している、違和感.
#include "floor/floor-util.h"
#include "floor/wild.h"
#include "game-option/birth-options.h"
#include "game-option/cheat-types.h"
#include "game-option/game-play-options.h"
#include "game-option/play-record-options.h"
#include "grid/feature.h"
#include "grid/grid.h"
#include "info-reader/fixed-map-parser.h"
#include "io/write-diary.h"
#include "market/arena-entry.h"
#include "monster-floor/monster-generator.h"
#include "monster-floor/monster-remover.h"
#include "monster-floor/place-monster-types.h"
#include "monster/monster-flag-types.h"
#include "monster/monster-status-setter.h"
#include "monster/monster-status.h"
#include "monster/monster-update.h"
#include "monster/monster-util.h"
#include "player/player-status.h"
#include "system/angband-system.h"
#include "system/building-type-definition.h"
#include "system/dungeon-info.h"
#include "system/floor-type-definition.h"
#include "system/grid-type-definition.h"
#include "system/item-entity.h"
#include "system/monster-entity.h"
#include "system/monster-race-info.h"
#include "system/player-type-definition.h"
#include "system/terrain-type-definition.h"
#include "util/bit-flags-calculator.h"
#include "view/display-messages.h"
#include "window/main-window-util.h"
#include "wizard/wizard-messages.h"
#include "world/world.h"
#include <algorithm>
#include <array>
#include <stack>

/*!
 * @brief 闘技場用のアリーナ地形を作成する / Builds the on_defeat_arena_monster after it is entered -KMW-
 * @param player_ptr プレイヤーへの参照ポインタ
 */
static void build_arena(PlayerType *player_ptr, POSITION *start_y, POSITION *start_x)
{
    POSITION yval = ARENA_WID / 2;
    POSITION xval = ARENA_HGT / 2;
    POSITION y_height = yval - 15;
    POSITION y_depth = yval + 15;
    POSITION x_left = xval - 15;
    POSITION x_right = xval + 15;
    auto *floor_ptr = player_ptr->current_floor_ptr;

    for (POSITION i = y_height; i <= y_height + 5; i++) {
        for (POSITION j = x_left; j <= x_right; j++) {
            place_bold(player_ptr, i, j, GB_EXTRA_PERM);
            floor_ptr->grid_array[i][j].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }

    for (POSITION i = y_depth; i >= y_depth - 5; i--) {
        for (POSITION j = x_left; j <= x_right; j++) {
            place_bold(player_ptr, i, j, GB_EXTRA_PERM);
            floor_ptr->grid_array[i][j].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }

    for (POSITION j = x_left; j <= x_left + 5; j++) {
        for (POSITION i = y_height; i <= y_depth; i++) {
            place_bold(player_ptr, i, j, GB_EXTRA_PERM);
            floor_ptr->grid_array[i][j].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }

    for (POSITION j = x_right; j >= x_right - 5; j--) {
        for (POSITION i = y_height; i <= y_depth; i++) {
            place_bold(player_ptr, i, j, GB_EXTRA_PERM);
            floor_ptr->grid_array[i][j].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }

    // 柱っぽいもの
    place_bold(player_ptr, y_height + 6, x_left + 18, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_height + 6][x_left + 18].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_depth - 6, x_left + 18, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_depth - 6][x_left + 18].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_height + 6, x_right - 18, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_height + 6][x_right - 18].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_depth - 6, x_right - 18, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_depth - 6][x_right - 18].info |= CAVE_GLOW | CAVE_MARK;

    place_bold(player_ptr, y_height + 9, x_left + 21, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_height + 9][x_left + 21].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_depth - 9, x_left + 21, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_depth - 9][x_left + 21].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_height + 9, x_right - 21, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_height + 9][x_right - 21].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_depth - 9, x_right - 21, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_depth - 9][x_right - 21].info |= CAVE_GLOW | CAVE_MARK;

    *start_y = y_height + 10;
    *start_x = xval;
    floor_ptr->grid_array[*start_y][*start_x].feat = TerrainList::get_instance().get_terrain_id_by_tag("ARENA_GATE");
    floor_ptr->grid_array[*start_y][*start_x].info |= CAVE_GLOW | CAVE_MARK;
}

/*!
 * @brief 挑戦時闘技場への入場処理
 * @param player_ptr プレイヤーへの参照ポインタ
 * @details 互換性のため、『森トロル』など地上と闘技場の両方に出現するユニークを撃破した際の不戦勝処理を残している
 * @todo v3.0正式版リリース以降に上記を削除する
 */
static void generate_challenge_arena(PlayerType *player_ptr)
{
    auto &floor = *player_ptr->current_floor_ptr;
    floor.height = SCREEN_HGT;
    floor.width = SCREEN_WID;
    for (auto y = 0; y < MAX_HGT; y++) {
        for (auto x = 0; x < MAX_WID; x++) {
            place_bold(player_ptr, y, x, GB_SOLID_PERM);
            floor.get_grid({ y, x }).add_info(CAVE_GLOW | CAVE_MARK);
        }
    }

    int y;
    int x;
    for (y = 1; y < SCREEN_HGT - 1; y++) {
        for (x = 1; x < SCREEN_WID - 1; x++) {
            floor.get_grid({ y, x }).feat = feat_floor;
        }
    }

    build_arena(player_ptr, &y, &x);
    player_place(player_ptr, y, x);
    auto &entries = ArenaEntryList::get_instance();
    const auto &monrace = entries.get_monrace();
    if (place_specific_monster(player_ptr, player_ptr->y + 5, player_ptr->x, monrace.idx, PM_NO_KAGE | PM_NO_PET)) {
        return;
    }

    AngbandWorld::get_instance().set_arena(true);
    entries.increment_entry();
    msg_print(_("相手は欠場した。あなたの不戦勝だ。", "The enemy is unable to appear. You won by default."));
}

/*!
 * @brief モンスター闘技場のフロア生成 / Builds the on_defeat_arena_monster after it is entered -KMW-
 * @param player_ptr プレイヤーへの参照ポインタ
 */
static void build_battle(PlayerType *player_ptr, POSITION *y, POSITION *x)
{
    POSITION yval = ARENA_WID / 2;
    POSITION xval = ARENA_HGT / 2;
    POSITION y_height = yval - 15;
    POSITION y_depth = yval + 15;
    POSITION x_left = xval - 15;
    POSITION x_right = xval + 15;

    auto *floor_ptr = player_ptr->current_floor_ptr;
    for (int i = y_height; i <= y_height + 5; i++) {
        for (int j = x_left; j <= x_right; j++) {
            place_bold(player_ptr, i, j, GB_EXTRA_PERM);
            floor_ptr->grid_array[i][j].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }

    for (int i = y_depth; i >= y_depth - 5; i--) {
        for (int j = x_left; j <= x_right; j++) {
            place_bold(player_ptr, i, j, GB_EXTRA_PERM);
            floor_ptr->grid_array[i][j].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }

    for (int j = x_left; j <= x_left + 5; j++) {
        for (int i = y_height; i <= y_depth; i++) {
            place_bold(player_ptr, i, j, GB_EXTRA_PERM);
            floor_ptr->grid_array[i][j].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }

    for (POSITION j = x_right; j >= x_right - 5; j--) {
        for (POSITION i = y_height; i <= y_depth; i++) {
            place_bold(player_ptr, i, j, GB_EXTRA_PERM);
            floor_ptr->grid_array[i][j].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }

    place_bold(player_ptr, y_height + 6, x_left + 18, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_height + 6][x_left + 18].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_depth - 4, x_left + 18, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_depth - 4][x_left + 18].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_height + 6, x_right - 18, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_height + 6][x_right - 18].info |= CAVE_GLOW | CAVE_MARK;
    place_bold(player_ptr, y_depth - 4, x_right - 18, GB_EXTRA_PERM);
    floor_ptr->grid_array[y_depth - 4][x_right - 18].info |= CAVE_GLOW | CAVE_MARK;

    for (int i = y_height + 1; i <= y_height + 5; i++) {
        for (int j = x_left + 20 + 2 * (y_height + 5 - i); j <= x_right - 20 - 2 * (y_height + 5 - i); j++) {
            assert(j >= 0 && i >= 0);
            floor_ptr->grid_array[i][j].feat = feat_permanent_glass_wall;
        }
    }

    POSITION last_y = y_height + 1;
    POSITION last_x = xval;
    floor_ptr->grid_array[last_y][last_x].feat = TerrainList::get_instance().get_terrain_id_by_tag("BUILDING_3");
    floor_ptr->grid_array[last_y][last_x].info |= CAVE_GLOW | CAVE_MARK;
    *y = last_y;
    *x = last_x;
}

/*!
 * @brief モンスター闘技場への導入処理 / Town logic flow for generation of on_defeat_arena_monster -KMW-
 */
static void generate_gambling_arena(PlayerType *player_ptr)
{
    POSITION y, x;
    POSITION qy = 0;
    POSITION qx = 0;
    auto *floor_ptr = player_ptr->current_floor_ptr;
    for (y = 0; y < MAX_HGT; y++) {
        for (x = 0; x < MAX_WID; x++) {
            place_bold(player_ptr, y, x, GB_SOLID_PERM);
            floor_ptr->grid_array[y][x].info |= (CAVE_GLOW | CAVE_MARK);
        }
    }
    for (y = qy + 1; y < qy + ARENA_HGT - 1; y++) {
        for (x = qx + 1; x < qx + ARENA_WID - 1; x++) {
            floor_ptr->grid_array[y][x].feat = feat_floor;
        }
    }

    build_battle(player_ptr, &y, &x);
    player_place(player_ptr, y, x);
    const auto &melee_arena = MeleeArena::get_instance();
    for (auto i = 0; i < NUM_GLADIATORS; i++) {
        const auto &gladiator = melee_arena.get_gladiator(i);
        const Pos2D pos(player_ptr->y + 8 + (i / 2) * 4, player_ptr->x - 2 + (i % 2) * 4);
        constexpr auto mode = PM_NO_KAGE | PM_NO_PET;
        const auto m_idx = place_specific_monster(player_ptr, pos.y, pos.x, gladiator.monrace_id, mode);
        if (m_idx > 0) {
            floor_ptr->m_list[*m_idx].set_friendly();
        }
    }

    for (MONSTER_IDX i = 1; i < floor_ptr->m_max; i++) {
        auto *m_ptr = &floor_ptr->m_list[i];
        if (!m_ptr->is_valid()) {
            continue;
        }

        m_ptr->mflag2.set({ MonsterConstantFlagType::MARK, MonsterConstantFlagType::SHOW });
        update_monster(player_ptr, i, false);
    }
}

/*!
 * @brief 固定マップクエストのフロア生成 / Generate a quest level
 * @param player_ptr プレイヤーへの参照ポインタ
 */
static void generate_fixed_floor(PlayerType *player_ptr)
{
    auto &floor = *player_ptr->current_floor_ptr;
    for (auto y = 0; y < floor.height; y++) {
        for (auto x = 0; x < floor.width; x++) {
            place_bold(player_ptr, y, x, GB_SOLID_PERM);
        }
    }

    const auto &quests = QuestList::get_instance();
    floor.base_level = quests.get_quest(floor.quest_number).level;
    floor.dun_level = floor.base_level;
    floor.object_level = floor.base_level;
    floor.monster_level = floor.base_level;
    if (record_stair) {
        exe_write_diary_quest(player_ptr, DiaryKind::TO_QUEST, floor.quest_number);
    }
    get_mon_num_prep(player_ptr, get_monster_hook(player_ptr), nullptr);
    init_flags = INIT_CREATE_DUNGEON;
    parse_fixed_map(player_ptr, QUEST_DEFINITION_LIST, 0, 0, MAX_HGT, MAX_WID);
}

/*!
 * @brief ダンジョン時のランダムフロア生成 / Make a real level
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param concptr
 * @return フロアの生成に成功したらTRUE
 */
static bool level_gen(PlayerType *player_ptr, concptr *why)
{
    constexpr auto chance_small_floor = 10;
    auto *floor_ptr = player_ptr->current_floor_ptr;
    DUNGEON_IDX d_idx = floor_ptr->dungeon_idx;
    int level_height;
    int level_width;
    if (dungeons_info[d_idx].flags.has(DungeonFeatureType::ALWAY_MAX_SIZE)) {
        level_height = MAX_HGT / SCREEN_HGT;
        level_width = MAX_WID / SCREEN_WID;
    } else if (always_small_levels || ironman_small_levels || (small_levels && one_in_(chance_small_floor)) || dungeons_info[d_idx].flags.has(DungeonFeatureType::SMALLEST)) {
        level_height = MIN_HGT_MULTIPLE;
        level_width = MIN_WID_MULTIPLE;
    } else if (dungeons_info[d_idx].flags.has(DungeonFeatureType::BEGINNER)) {
        level_height = MIN_HGT_MULTIPLE * 2;
        level_width = MIN_WID_MULTIPLE * 2;
    } else {
        if (one_in_(HUGE_DUNGEON_RATE)) {
            level_height = randint1(MAX_HGT / SCREEN_HGT);
            level_width = randint1(MAX_WID / SCREEN_WID);
        } else if (one_in_(LARGE_DUNGEON_RATE) || dungeons_info[d_idx].flags.has(DungeonFeatureType::BIG)) {
            level_height = randint1(MAX_HGT / SCREEN_HGT / 2);
            level_width = randint1(MAX_WID / SCREEN_WID / 2);
        } else {
            level_height = randint1(MAX_HGT / SCREEN_HGT / 3);
            level_width = randint1(MAX_WID / SCREEN_WID / 3);
        }
        bool is_first_level_area = true;
        bool is_max_area = (level_height == MAX_HGT / SCREEN_HGT) && (level_width == MAX_WID / SCREEN_WID);
        while (is_first_level_area || is_max_area) {
            level_height = randint1(MAX_HGT / SCREEN_HGT);
            level_width = randint1(MAX_WID / SCREEN_WID);
            is_first_level_area = false;
            is_max_area = (level_height == MAX_HGT / SCREEN_HGT) && (level_width == MAX_WID / SCREEN_WID);
        }
    }

    level_width = std::max(MIN_WID_MULTIPLE, level_width);
    level_height = std::max(MIN_HGT_MULTIPLE, level_height);

    floor_ptr->height = level_height * SCREEN_HGT;
    floor_ptr->width = level_width * SCREEN_WID;
    panel_row_min = floor_ptr->height;
    panel_col_min = floor_ptr->width;

    msg_format_wizard(
        player_ptr, CHEAT_DUNGEON, _("小さなフロア: X:%d, Y:%d", "A 'small' dungeon level: X:%d, Y:%d."), floor_ptr->width, floor_ptr->height);

    return cave_gen(player_ptr, why);
}

/*!
 * @brief フロアに存在する全マスの記憶状態を初期化する / Wipe all unnecessary flags after grid_array generation
 */
void wipe_generate_random_floor_flags(FloorType *floor_ptr)
{
    for (auto y = 0; y < floor_ptr->height; y++) {
        for (auto x = 0; x < floor_ptr->width; x++) {
            floor_ptr->get_grid({ y, x }).info &= ~(CAVE_MASK);
        }
    }

    if (floor_ptr->is_in_underground()) {
        for (auto y = 1; y < floor_ptr->height - 1; y++) {
            for (auto x = 1; x < floor_ptr->width - 1; x++) {
                floor_ptr->get_grid({ y, x }).info |= CAVE_UNSAFE;
            }
        }
    }
}

/*!
 * @brief フロアの全情報を初期化する / Clear and empty floor.
 * @parama player_ptr プレイヤーへの参照ポインタ
 */
void clear_cave(PlayerType *player_ptr)
{
    auto *floor_ptr = player_ptr->current_floor_ptr;
    std::fill_n(floor_ptr->o_list.begin(), floor_ptr->o_max, ItemEntity{});
    floor_ptr->o_max = 1;
    floor_ptr->o_cnt = 0;

    for (auto &[r_idx, r_ref] : monraces_info) {
        r_ref.cur_num = 0;
    }

    std::fill_n(floor_ptr->m_list.begin(), floor_ptr->m_max, MonsterEntity{});
    floor_ptr->m_max = 1;
    floor_ptr->m_cnt = 0;
    for (int i = 0; i < MAX_MTIMED; i++) {
        floor_ptr->mproc_max[i] = 0;
    }

    precalc_cur_num_of_pet();
    for (POSITION y = 0; y < MAX_HGT; y++) {
        for (POSITION x = 0; x < MAX_WID; x++) {
            auto *g_ptr = &floor_ptr->grid_array[y][x];
            g_ptr->info = 0;
            g_ptr->feat = 0;
            g_ptr->o_idx_list.clear();
            g_ptr->m_idx = 0;
            g_ptr->special = 0;
            g_ptr->mimic = 0;
            g_ptr->reset_costs();
            g_ptr->reset_dists();
            g_ptr->when = 0;
        }
    }

    floor_ptr->base_level = floor_ptr->dun_level;
    floor_ptr->monster_level = floor_ptr->base_level;
    floor_ptr->object_level = floor_ptr->base_level;
}

typedef bool (*IsWallFunc)(const FloorType *, int, int);

// (y,x) がプレイヤーが通れない永久地形かどうかを返す。
static bool is_permanent_blocker(const FloorType *const floor_ptr, const int y, const int x)
{
    const auto &flags = floor_ptr->get_grid({ y, x }).get_terrain().flags;
    return flags.has(TerrainCharacteristics::PERMANENT) && flags.has_not(TerrainCharacteristics::MOVE);
}

static void floor_is_connected_dfs(const FloorType *const floor_ptr, const IsWallFunc is_wall, const int y_start, const int x_start, bool *const visited)
{
    // clang-format off
    static const int DY[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };
    static const int DX[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };
    // clang-format on

    const int h = floor_ptr->height;
    const int w = floor_ptr->width;
    const int start = w * y_start + x_start;

    // 深さ優先探索用のスタック。
    // 最大フロアサイズが h=66, w=198 なので、スタックオーバーフロー防止のため再帰は使わない。
    std::stack<int> stk;

    stk.emplace(start);
    visited[start] = true;

    while (!stk.empty()) {
        const int cur = stk.top();
        stk.pop();
        const int y = cur / w;
        const int x = cur % w;

        for (int i = 0; i < 8; ++i) {
            const int y_nxt = y + DY[i];
            const int x_nxt = x + DX[i];
            if (y_nxt < 0 || h <= y_nxt || x_nxt < 0 || w <= x_nxt) {
                continue;
            }
            const int nxt = w * y_nxt + x_nxt;
            if (visited[nxt]) {
                continue;
            }
            if (is_wall(floor_ptr, y_nxt, x_nxt)) {
                continue;
            }

            stk.emplace(nxt);
            visited[nxt] = true;
        }
    }
}

// 現在のフロアが連結かどうかを返す。
// 各セルの8近傍は互いに移動可能とし、is_wall が真を返すセルのみを壁とみなす。
//
// 連結成分数が 0 の場合、偽を返す。
static bool floor_is_connected(const FloorType *const floor_ptr, const IsWallFunc is_wall)
{
    static std::array<bool, MAX_HGT * MAX_WID> visited;

    const int h = floor_ptr->height;
    const int w = floor_ptr->width;

    std::fill(begin(visited), end(visited), false);

    int n_component = 0; // 連結成分数

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const int idx = w * y + x;
            if (visited[idx]) {
                continue;
            }
            if (is_wall(floor_ptr, y, x)) {
                continue;
            }

            if (++n_component >= 2) {
                break;
            }
            floor_is_connected_dfs(floor_ptr, is_wall, y, x, visited.data());
        }
    }

    return n_component == 1;
}

/*!
 * ダンジョンのランダムフロアを生成する / Generates a random dungeon level -RAK-
 * @parama player_ptr プレイヤーへの参照ポインタ
 * @note Hack -- regenerate any "overflow" levels
 */
void generate_floor(PlayerType *player_ptr)
{
    auto &floor = *player_ptr->current_floor_ptr;
    set_floor_and_wall(floor.dungeon_idx);
    const auto is_wild_mode = AngbandWorld::get_instance().is_wild_mode();
    for (int num = 0; true; num++) {
        bool okay = true;
        concptr why = nullptr;
        clear_cave(player_ptr);
        player_ptr->x = player_ptr->y = 0;
        if (floor.inside_arena) {
            generate_challenge_arena(player_ptr);
        } else if (AngbandSystem::get_instance().is_phase_out()) {
            generate_gambling_arena(player_ptr);
        } else if (floor.is_in_quest()) {
            generate_fixed_floor(player_ptr);
        } else if (!floor.is_in_underground()) {
            if (is_wild_mode) {
                wilderness_gen_small(player_ptr);
            } else {
                wilderness_gen(player_ptr);
            }
        } else {
            okay = level_gen(player_ptr, &why);
        }

        if (floor.o_max >= MAX_FLOOR_ITEMS) {
            why = _("アイテムが多すぎる", "too many objects");
            okay = false;
        } else if (floor.m_max >= MAX_FLOOR_MONSTERS) {
            why = _("モンスターが多すぎる", "too many monsters");
            okay = false;
        }

        // ダンジョン内フロアが連結でない(永久壁で区切られた孤立部屋がある)場合、
        // 狂戦士でのプレイに支障をきたしうるので再生成する。
        // 地上、荒野マップ、クエストでは連結性判定は行わない。
        // TODO: 本来はダンジョン生成アルゴリズム自身で連結性を保証するのが理想ではある。
        const bool check_conn = okay && floor.is_in_underground() && !floor.is_in_quest();
        if (check_conn && !floor_is_connected(&floor, is_permanent_blocker)) {
            // 一定回数試しても連結にならないなら諦める。
            if (num >= 1000) {
                plog("cannot generate connected floor. giving up...");
            } else {
                why = _("フロアが連結でない", "floor is not connected");
                okay = false;
            }
        }

        if (okay) {
            break;
        }

        if (why) {
            msg_format(_("生成やり直し(%s)", "Generation restarted (%s)"), why);
        }

        wipe_o_list(&floor);
        wipe_monsters_list(player_ptr);
    }

    glow_deep_lava_and_bldg(player_ptr);
    player_ptr->enter_dungeon = false;
    wipe_generate_random_floor_flags(&floor);
}
