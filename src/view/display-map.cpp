#include "view/display-map.h"
#include "autopick/autopick-finder.h"
#include "autopick/autopick-methods-table.h"
#include "autopick/autopick-util.h"
#include "floor/cave.h"
#include "floor/geometry.h"
#include "game-option/map-screen-options.h"
#include "game-option/special-options.h"
#include "grid/feature.h"
#include "grid/grid.h"
#include "monster-race/monster-race.h"
#include "object/object-info.h"
#include "object/object-mark-types.h"
#include "system/baseitem-info.h"
#include "system/floor-type-definition.h"
#include "system/grid-type-definition.h"
#include "system/item-entity.h"
#include "system/monster-entity.h"
#include "system/monster-race-info.h"
#include "system/player-type-definition.h"
#include "system/terrain-type-definition.h"
#include "term/term-color-types.h"
#include "timed-effect/player-blindness.h"
#include "timed-effect/player-hallucination.h"
#include "timed-effect/timed-effects.h"
#include "util/bit-flags-calculator.h"
#include "view/colored-char.h"
#include "window/main-window-util.h"
#include "world/world.h"
#include <algorithm>
#include <span>

uint8_t display_autopick; /*!< 自動拾い状態の設定フラグ */

namespace {
/* 一般的にオブジェクトシンボルとして扱われる記号を定義する(幻覚処理向け) /  Hack -- Legal object codes */
const std::string image_objects = R"(?/|\"!$()_-=[]{},~)";

/* 一般的にモンスターシンボルとして扱われる記号を定義する(幻覚処理向け) / Hack -- Legal monster codes */
const std::string image_monsters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*!
 * @brief オブジェクトの表示を幻覚状態に差し替える
 * @return 差し替えたシンボルと色
 */
ColoredChar image_object()
{
    if (use_graphics) {
        auto &baseitems = BaseitemList::get_instance();
        const std::span<BaseitemInfo> candidates(baseitems.begin() + 1, baseitems.end());
        const auto &baseitem = rand_choice(candidates);
        return baseitem.cc_config;
    }

    return { randnum1<uint8_t>(15), rand_choice(image_objects) };
}

/*!
 * @brief モンスターの表示を幻覚状態に差し替える
 * @return 差し替えたシンボルと色
 */
ColoredChar image_monster()
{
    if (use_graphics) {
        const auto monrace_id = MonsterRace::pick_one_at_random();
        const auto &monrace = monraces_info[monrace_id];
        return monrace.cc_config;
    }

    const auto color = randnum1<uint8_t>(15);
    const auto character = one_in_(25) ? rand_choice(image_objects) : rand_choice(image_monsters);
    return { color, character };
}

/*!
 * @brief オブジェクト＆モンスターの表示を幻覚状態に差し替える
 * @return 差し替えたシンボルと色
 */
ColoredChar image_random()
{
    if (randint0(100) < 75) {
        return image_monster();
    } else {
        return image_object();
    }
}
}

/*!
 * @brief マップに表示されるべき地形(壁)かどうかを判定する
 * @param floor 階の情報への参照
 * @param pos グリッド座標
 * @return 表示されるべきならtrue、そうでないならfalse
 * @details
 * 周り全てが壁に囲まれている壁についてはオプション状態による。
 * 1か所でも空きがあるか、壁ではない地形、金を含む地形、永久岩は表示。
 */
static bool is_revealed_wall(const FloorType &floor, const Pos2D &pos)
{
    const auto &grid = floor.get_grid(pos);
    if (view_hidden_walls) {
        if (view_unsafe_walls) {
            return true;
        }
        if (none_bits(grid.info, CAVE_UNSAFE)) {
            return true;
        }
    }

    const auto &terrain = grid.get_terrain_mimic();
    if (terrain.flags.has_not(TerrainCharacteristics::WALL) || terrain.flags.has(TerrainCharacteristics::HAS_GOLD)) {
        return true;
    }

    if (in_bounds(&floor, pos.y, pos.x) && terrain.flags.has(TerrainCharacteristics::PERMANENT)) {
        return true;
    }

    const auto num_of_walls = std::count_if(CCW_DD.begin(), CCW_DD.end(),
        [&floor, &pos](const auto &dd) {
            const auto pos_neighbor = pos + dd;
            if (!in_bounds(&floor, pos_neighbor.y, pos_neighbor.x)) {
                return true;
            }

            const auto &terrain_neighbor = floor.get_grid(pos_neighbor).get_terrain();
            return terrain_neighbor.flags.has(TerrainCharacteristics::WALL);
        });

    return num_of_walls != std::ssize(CCW_DD);
}

/*!
 * @brief 指定した座標の地形の表示属性を取得する
 * @param player_ptr プレイヤー情報への参照ポインタ
 * @param pos 階の中の座標
 * @return シンボル表記
 * @todo 強力発動コピペの嵐…ポインタ引数の嵐……Fuuu^h^hck!!
 */
ColoredCharPair map_info(PlayerType *player_ptr, const Pos2D &pos)
{
    auto &floor = *player_ptr->current_floor_ptr;
    auto &grid = floor.get_grid(pos);
    auto &terrains = TerrainList::get_instance();
    auto *terrain_mimic_ptr = &grid.get_terrain_mimic();
    ColoredChar cc_config;
    if (terrain_mimic_ptr->flags.has_not(TerrainCharacteristics::REMEMBER)) {
        auto is_visible = any_bits(grid.info, (CAVE_MARK | CAVE_LITE | CAVE_MNLT));
        auto is_glowing = match_bits(grid.info, CAVE_GLOW | CAVE_MNDK, CAVE_GLOW);
        auto can_view = grid.is_view() && (is_glowing || player_ptr->see_nocto);
        const auto is_blind = player_ptr->effects()->blindness()->is_blind();
        if (!is_blind && (is_visible || can_view)) {
            cc_config = terrain_mimic_ptr->cc_configs[F_LIT_STANDARD];
            if (player_ptr->wild_mode) {
                if (view_special_lite && !w_ptr->is_daytime()) {
                    cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                }
            } else if (darkened_grid(player_ptr, &grid)) {
                const auto unsafe_terrain_id = (view_unsafe_grids && (grid.info & CAVE_UNSAFE)) ? feat_undetected : feat_none;
                terrain_mimic_ptr = &terrains[unsafe_terrain_id];
                cc_config = terrain_mimic_ptr->cc_configs[F_LIT_STANDARD];
            } else if (view_special_lite) {
                if (grid.info & (CAVE_LITE | CAVE_MNLT)) {
                    if (view_yellow_lite) {
                        cc_config = terrain_mimic_ptr->cc_configs[F_LIT_LITE];
                    }
                } else if ((grid.info & (CAVE_GLOW | CAVE_MNDK)) != CAVE_GLOW) {
                    cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                } else if (!(grid.info & CAVE_VIEW)) {
                    if (view_bright_lite) {
                        cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                    }
                }
            }
        } else {
            const auto unsafe_terrain_id = (view_unsafe_grids && (grid.info & CAVE_UNSAFE)) ? feat_undetected : feat_none;
            terrain_mimic_ptr = &terrains[unsafe_terrain_id];
            cc_config = terrain_mimic_ptr->cc_configs[F_LIT_STANDARD];
        }
    } else {
        if (grid.is_mark() && is_revealed_wall(floor, pos)) {
            cc_config = terrain_mimic_ptr->cc_configs[F_LIT_STANDARD];
            const auto is_blind = player_ptr->effects()->blindness()->is_blind();
            if (player_ptr->wild_mode) {
                if (view_granite_lite && (is_blind || !w_ptr->is_daytime())) {
                    cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                }
            } else if (darkened_grid(player_ptr, &grid) && !is_blind) {
                if (terrain_mimic_ptr->flags.has_all_of({ TerrainCharacteristics::LOS, TerrainCharacteristics::PROJECT })) {
                    const auto unsafe_terrain_id = (view_unsafe_grids && (grid.info & CAVE_UNSAFE)) ? feat_undetected : feat_none;
                    terrain_mimic_ptr = &terrains[unsafe_terrain_id];
                    cc_config = terrain_mimic_ptr->cc_configs[F_LIT_STANDARD];
                } else if (view_granite_lite && view_bright_lite) {
                    cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                }
            } else if (view_granite_lite) {
                if (is_blind) {
                    cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                } else if (grid.info & (CAVE_LITE | CAVE_MNLT)) {
                    if (view_yellow_lite) {
                        cc_config = terrain_mimic_ptr->cc_configs[F_LIT_LITE];
                    }
                } else if (view_bright_lite) {
                    if (!(grid.info & CAVE_VIEW)) {
                        cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                    } else if ((grid.info & (CAVE_GLOW | CAVE_MNDK)) != CAVE_GLOW) {
                        cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                    } else if (terrain_mimic_ptr->flags.has_not(TerrainCharacteristics::LOS) && !check_local_illumination(player_ptr, pos.y, pos.x)) {
                        cc_config = terrain_mimic_ptr->cc_configs[F_LIT_DARK];
                    }
                }
            }
        } else {
            const auto unsafe_terrain_id = (view_unsafe_grids && (grid.info & CAVE_UNSAFE)) ? feat_undetected : feat_none;
            terrain_mimic_ptr = &terrains[unsafe_terrain_id];
            cc_config = terrain_mimic_ptr->cc_configs[F_LIT_STANDARD];
        }
    }

    if (feat_priority == -1) {
        feat_priority = terrain_mimic_ptr->priority;
    }

    ColoredCharPair ccp = { cc_config, cc_config };
    const auto is_hallucinated = player_ptr->effects()->hallucination()->is_hallucinated();
    if (is_hallucinated && one_in_(256)) {
        ccp.cc_foreground = image_random();
    }

    for (const auto this_o_idx : grid.o_idx_list) {
        ItemEntity *o_ptr;
        o_ptr = &floor.o_list[this_o_idx];
        if (o_ptr->marked.has_not(OmType::FOUND)) {
            continue;
        }

        if (display_autopick) {
            byte act;

            match_autopick = find_autopick_list(player_ptr, o_ptr);
            if (match_autopick == -1) {
                continue;
            }

            act = autopick_list[match_autopick].action;

            if ((act & DO_DISPLAY) && (act & display_autopick)) {
                autopick_obj = o_ptr;
            } else {
                match_autopick = -1;
                continue;
            }
        }

        ccp.cc_foreground = { o_ptr->get_color(), o_ptr->get_symbol() };
        feat_priority = 20;
        if (is_hallucinated) {
            ccp.cc_foreground = image_object();
        }

        break;
    }

    if (grid.has_monster() && display_autopick != 0) {
        ccp.cc_foreground = set_term_color(player_ptr, pos, ccp.cc_foreground);
        return ccp;
    }

    auto *m_ptr = &floor.m_list[grid.m_idx];
    if (!m_ptr->ml) {
        ccp.cc_foreground = set_term_color(player_ptr, pos, ccp.cc_foreground);
        return ccp;
    }

    const auto &monrace_ap = m_ptr->get_appearance_monrace();
    feat_priority = 30;
    if (is_hallucinated) {
        if (!monrace_ap.visual_flags.has_all_of({ MonsterVisualType::CLEAR, MonsterVisualType::CLEAR_COLOR })) {
            ccp.cc_foreground = image_monster();
        }

        ccp.cc_foreground = set_term_color(player_ptr, pos, ccp.cc_foreground);
        return ccp;
    }

    cc_config = monrace_ap.cc_config;
    if (monrace_ap.visual_flags.has_none_of({ MonsterVisualType::CLEAR, MonsterVisualType::SHAPECHANGER, MonsterVisualType::CLEAR_COLOR, MonsterVisualType::MULTI_COLOR, MonsterVisualType::RANDOM_COLOR })) {
        ccp.cc_foreground = set_term_color(player_ptr, pos, cc_config);
        return ccp;
    }

    if (monrace_ap.visual_flags.has_all_of({ MonsterVisualType::CLEAR, MonsterVisualType::CLEAR_COLOR })) {
        ccp.cc_foreground = set_term_color(player_ptr, pos, ccp.cc_foreground);
        return ccp;
    }

    if (monrace_ap.visual_flags.has(MonsterVisualType::CLEAR_COLOR) && (ccp.cc_foreground.color != TERM_DARK) && !use_graphics) {
        /* Do nothing */
    } else if (monrace_ap.visual_flags.has(MonsterVisualType::MULTI_COLOR) && !use_graphics) {
        if (monrace_ap.visual_flags.has(MonsterVisualType::ANY_COLOR)) {
            ccp.cc_foreground.color = randnum1<uint8_t>(15);
        } else {
            constexpr static auto colors = {
                TERM_RED,
                TERM_L_RED,
                TERM_WHITE,
                TERM_L_GREEN,
                TERM_BLUE,
                TERM_L_DARK,
                TERM_GREEN,
            };

            ccp.cc_foreground.color = rand_choice(colors);
        }
    } else if (monrace_ap.visual_flags.has(MonsterVisualType::RANDOM_COLOR) && !use_graphics) {
        ccp.cc_foreground.color = grid.m_idx % 15 + 1;
    } else {
        ccp.cc_foreground.color = cc_config.color;
    }

    if (monrace_ap.visual_flags.has(MonsterVisualType::CLEAR) && (ccp.cc_foreground.character != ' ') && !use_graphics) {
        ccp.cc_foreground = set_term_color(player_ptr, pos, ccp.cc_foreground);
        return ccp;
    }

    if (monrace_ap.visual_flags.has(MonsterVisualType::SHAPECHANGER)) {
        if (use_graphics) {
            auto r_idx = MonsterRace::pick_one_at_random();
            const auto &monrace = monraces_info[r_idx];
            ccp.cc_foreground = monrace.cc_config;
        } else {
            ccp.cc_foreground.character = one_in_(25) ? rand_choice(image_objects) : rand_choice(image_monsters);
        }

        ccp.cc_foreground = set_term_color(player_ptr, pos, ccp.cc_foreground);
        return ccp;
    }

    ccp.cc_foreground = set_term_color(player_ptr, pos, { ccp.cc_foreground.color, cc_config.character });
    return ccp;
}
