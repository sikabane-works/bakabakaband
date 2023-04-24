/*!
 * @brief 馬鹿馬鹿蛮怒 v1.5.0以前の旧いセーブデータを読み込む処理
 * @date 2020/07/04
 * @author Hourier
 * @details 互換性を最大限に確保するため、基本的に関数分割は行わないものとする.
 */

#include "load/old/load-v1-5-0.h"
#include "dungeon/dungeon.h"
#include "floor/floor-object.h"
#include "game-option/birth-options.h"
#include "grid/feature.h"
#include "grid/grid.h"
#include "grid/trap.h"
#include "load/angband-version-comparer.h"
#include "load/item/item-loader-factory.h"
#include "load/item/item-loader-version-types.h"
#include "load/load-util.h"
#include "load/monster/monster-loader-factory.h"
#include "load/old-feature-types.h"
#include "load/old/item-loader-savefile50.h"
#include "load/old/monster-loader-savefile50.h"
#include "mind/mind-weaponsmith.h"
#include "monster-floor/monster-move.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags-resistance.h"
#include "monster-race/race-flags1.h"
#include "monster-race/race-flags3.h"
#include "monster-race/race-indice-types.h"
#include "monster/monster-flag-types.h"
#include "monster/monster-info.h"
#include "monster/monster-list.h"
#include "object-enchant/object-ego.h"
#include "object-enchant/old-ego-extra-values.h"
#include "object-enchant/tr-types.h"
#include "object-enchant/trc-types.h"
#include "object-enchant/trg-types.h"
#include "object/object-kind-hook.h"
#include "sv-definition/sv-armor-types.h"
#include "sv-definition/sv-lite-types.h"
#include "system/angband-exceptions.h"
#include "system/artifact-type-definition.h"
#include "system/floor-type-definition.h"
#include "system/grid-type-definition.h"
#include "system/monster-race-definition.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "util/bit-flags-calculator.h"
#include "util/enum-converter.h"
#include "util/quarks.h"
#include "world/world-object.h"
#include "world/world.h"

/* Old hidden trap flag */
static const BIT_FLAGS CAVE_TRAP = 0x8000;

const int OLD_QUEST_WATER_CAVE = 18; // 湖の洞窟.
const int QUEST_OLD_CASTLE = 27; // 古い城.
const int QUEST_ROYAL_CRYPT = 28; // 王家の墓.

static void move_RF3_to_RFR(monster_race *r_ptr, const BIT_FLAGS rf3, const MonsterResistanceType rfr)
{
    if (r_ptr->r_flags3 & rf3) {
        r_ptr->r_flags3 &= ~rf3;
        r_ptr->resistance_flags.set(rfr);
    }
}

static void move_RF4_BR_to_RFR(monster_race *r_ptr, BIT_FLAGS f4, const BIT_FLAGS rf4_br, const MonsterResistanceType rfr)
{
    if (f4 & rf4_br) {
        r_ptr->resistance_flags.set(rfr);
    }
}

/*!
 * @brief モンスターの思い出を読み込む
 * @param r_ptr モンスター種族情報への参照ポインタ
 * @param r_idx モンスター種族ID
 * @details 本来はr_idxからr_ptrを決定可能だが、互換性を優先するため元コードのままとする
 */
void set_old_lore(monster_race *r_ptr, BIT_FLAGS f4, const MonsterRaceId r_idx)
{
    r_ptr->r_resistance_flags.clear();
    move_RF3_to_RFR(r_ptr, RF3_IM_ACID, MonsterResistanceType::IMMUNE_ACID);
    move_RF3_to_RFR(r_ptr, RF3_IM_ELEC, MonsterResistanceType::IMMUNE_ELEC);
    move_RF3_to_RFR(r_ptr, RF3_IM_FIRE, MonsterResistanceType::IMMUNE_FIRE);
    move_RF3_to_RFR(r_ptr, RF3_IM_COLD, MonsterResistanceType::IMMUNE_COLD);
    move_RF3_to_RFR(r_ptr, RF3_IM_POIS, MonsterResistanceType::IMMUNE_POISON);
    move_RF3_to_RFR(r_ptr, RF3_RES_TELE, MonsterResistanceType::RESIST_TELEPORT);
    move_RF3_to_RFR(r_ptr, RF3_RES_NETH, MonsterResistanceType::RESIST_NETHER);
    move_RF3_to_RFR(r_ptr, RF3_RES_WATE, MonsterResistanceType::RESIST_WATER);
    move_RF3_to_RFR(r_ptr, RF3_RES_PLAS, MonsterResistanceType::RESIST_PLASMA);
    move_RF3_to_RFR(r_ptr, RF3_RES_NEXU, MonsterResistanceType::RESIST_NEXUS);
    move_RF3_to_RFR(r_ptr, RF3_RES_DISE, MonsterResistanceType::RESIST_DISENCHANT);
    move_RF3_to_RFR(r_ptr, RF3_RES_ALL, MonsterResistanceType::RESIST_ALL);

    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_LITE, MonsterResistanceType::RESIST_LITE);
    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_DARK, MonsterResistanceType::RESIST_DARK);
    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_SOUN, MonsterResistanceType::RESIST_SOUND);
    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_CHAO, MonsterResistanceType::RESIST_CHAOS);
    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_TIME, MonsterResistanceType::RESIST_TIME);
    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_INER, MonsterResistanceType::RESIST_INERTIA);
    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_GRAV, MonsterResistanceType::RESIST_GRAVITY);
    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_SHAR, MonsterResistanceType::RESIST_SHARDS);
    move_RF4_BR_to_RFR(r_ptr, f4, RF4_BR_WALL, MonsterResistanceType::RESIST_FORCE);

    if (f4 & RF4_BR_CONF) {
        r_ptr->r_flags3 |= RF3_NO_CONF;
    }

    if (r_idx == MonsterRaceId::STORMBRINGER) {
        r_ptr->r_resistance_flags.set(MonsterResistanceType::RESIST_CHAOS);
    }

    if (r_ptr->r_kind_flags.has(MonsterKindType::ORC)) {
        r_ptr->r_resistance_flags.set(MonsterResistanceType::RESIST_DARK);
    }
}

/*!
 * @brief ダンジョン情報を読み込む / Read the dungeon (old method)
 * @param player_ptr プレイヤーへの参照ポインタ
 * @details
 * The monsters/objects must be loaded in the same order
 * that they were stored, since the actual indexes matter.
 */
errr rd_dungeon_old(PlayerType *player_ptr)
{
    auto *floor_ptr = player_ptr->current_floor_ptr;
    floor_ptr->dun_level = rd_s16b();
    if (h_older_than(0, 3, 8)) {
        player_ptr->dungeon_idx = DUNGEON_ANGBAND;
    } else {
        player_ptr->dungeon_idx = rd_byte();
    }

    floor_ptr->base_level = floor_ptr->dun_level;
    floor_ptr->base_level = rd_s16b();

    floor_ptr->num_repro = rd_s16b();
    player_ptr->y = rd_s16b();
    player_ptr->x = rd_s16b();
    if (h_older_than(0, 3, 13) && !floor_ptr->dun_level && !floor_ptr->inside_arena) {
        player_ptr->y = 33;
        player_ptr->x = 131;
    }
    floor_ptr->height = rd_s16b();
    floor_ptr->width = rd_s16b();
    strip_bytes(2); /* max_panel_rows */
    strip_bytes(2); /* max_panel_cols */

    int ymax = floor_ptr->height;
    int xmax = floor_ptr->width;

    for (int x = 0, y = 0; y < ymax;) {
        uint16_t info;
        auto count = rd_byte();
        if (h_older_than(0, 3, 6)) {
            info = rd_byte();
        } else {
            info = rd_u16b();
            info &= ~(CAVE_LITE | CAVE_VIEW | CAVE_MNLT | CAVE_MNDK);
        }

        for (int i = count; i > 0; i--) {
            grid_type *g_ptr;
            g_ptr = &floor_ptr->grid_array[y][x];
            g_ptr->info = info;
            if (++x >= xmax) {
                x = 0;
                if (++y >= ymax) {
                    break;
                }
            }
        }
    }

    for (int x = 0, y = 0; y < ymax;) {
        auto count = rd_byte();
        auto tmp8u = rd_byte();
        for (int i = count; i > 0; i--) {
            grid_type *g_ptr;
            g_ptr = &floor_ptr->grid_array[y][x];
            g_ptr->feat = (int16_t)tmp8u;
            if (++x >= xmax) {
                x = 0;
                if (++y >= ymax) {
                    break;
                }
            }
        }
    }

    for (int x = 0, y = 0; y < ymax;) {
        auto count = rd_byte();
        auto tmp8u = rd_byte();
        for (int i = count; i > 0; i--) {
            grid_type *g_ptr;
            g_ptr = &floor_ptr->grid_array[y][x];
            g_ptr->mimic = (int16_t)tmp8u;
            if (++x >= xmax) {
                x = 0;
                if (++y >= ymax) {
                    break;
                }
            }
        }
    }

    for (int x = 0, y = 0; y < ymax;) {
        auto count = rd_byte();
        auto tmp16s = rd_s16b();
        for (int i = count; i > 0; i--) {
            grid_type *g_ptr;
            g_ptr = &floor_ptr->grid_array[y][x];
            g_ptr->special = tmp16s;
            if (++x >= xmax) {
                x = 0;
                if (++y >= ymax) {
                    break;
                }
            }
        }
    }

    if (h_older_than(1, 0, 99)) {
        for (int y = 0; y < ymax; y++) {
            for (int x = 0; x < xmax; x++) {
                floor_ptr->grid_array[y][x].info &= ~(CAVE_MASK);
            }
        }
    }

    if (h_older_than(1, 1, 1, 0)) {
        for (int y = 0; y < ymax; y++) {
            for (int x = 0; x < xmax; x++) {
                grid_type *g_ptr;
                g_ptr = &floor_ptr->grid_array[y][x];

                /* Very old */
                if (g_ptr->feat == OLD_FEAT_INVIS) {
                    g_ptr->feat = feat_floor;
                    g_ptr->info |= CAVE_TRAP;
                }

                /* Older than 1.1.1 */
                if (g_ptr->feat == OLD_FEAT_MIRROR) {
                    g_ptr->feat = feat_floor;
                    g_ptr->info |= CAVE_OBJECT;
                }
            }
        }
    }

    if (h_older_than(1, 3, 1, 0)) {
        for (int y = 0; y < ymax; y++) {
            for (int x = 0; x < xmax; x++) {
                grid_type *g_ptr;
                g_ptr = &floor_ptr->grid_array[y][x];

                /* Old CAVE_IN_MIRROR flag */
                if (g_ptr->info & CAVE_OBJECT) {
                    g_ptr->mimic = feat_mirror;
                } else if ((g_ptr->feat == OLD_FEAT_RUNE_EXPLOSION) || (g_ptr->feat == OLD_FEAT_RUNE_PROTECTION)) {
                    g_ptr->info |= CAVE_OBJECT;
                    g_ptr->mimic = g_ptr->feat;
                    g_ptr->feat = feat_floor;
                } else if (g_ptr->info & CAVE_TRAP) {
                    g_ptr->info &= ~CAVE_TRAP;
                    g_ptr->mimic = g_ptr->feat;
                    g_ptr->feat = choose_random_trap(player_ptr);
                } else if (g_ptr->feat == OLD_FEAT_INVIS) {
                    g_ptr->mimic = feat_floor;
                    g_ptr->feat = feat_trap_open;
                }
            }
        }
    }

    /* Quest 18 was removed */
    if (!vanilla_town) {
        for (int y = 0; y < ymax; y++) {
            for (int x = 0; x < xmax; x++) {
                grid_type *g_ptr;
                g_ptr = &floor_ptr->grid_array[y][x];

                if ((g_ptr->special == OLD_QUEST_WATER_CAVE) && !floor_ptr->dun_level) {
                    if (g_ptr->feat == OLD_FEAT_QUEST_ENTER) {
                        g_ptr->feat = feat_tree;
                        g_ptr->special = 0;
                    } else if (g_ptr->feat == OLD_FEAT_BLDG_1) {
                        g_ptr->special = lite_town ? QUEST_OLD_CASTLE : QUEST_ROYAL_CRYPT;
                    }
                } else if ((g_ptr->feat == OLD_FEAT_QUEST_EXIT) && (floor_ptr->quest_number == i2enum<QuestId>(OLD_QUEST_WATER_CAVE))) {
                    g_ptr->feat = feat_up_stair;
                    g_ptr->special = 0;
                }
            }
        }
    }

    uint16_t limit;
    limit = rd_u16b();
    if (limit > w_ptr->max_o_idx) {
        load_note(format(_("アイテムの配列が大きすぎる(%d)！", "Too many (%d) object entries!"), limit));
        return 151;
    }

    auto item_loader = ItemLoaderFactory::create_loader();
    for (int i = 1; i < limit; i++) {
        OBJECT_IDX o_idx = o_pop(floor_ptr);
        if (i != o_idx) {
            load_note(format(_("アイテム配置エラー (%d <> %d)", "Object allocation error (%d <> %d)"), i, o_idx));
            return 152;
        }

        auto &item = floor_ptr->o_list[o_idx];
        item_loader->rd_item(&item);
        auto &list = get_o_idx_list_contains(floor_ptr, o_idx);
        list.add(floor_ptr, o_idx);
    }

    limit = rd_u16b();
    if (limit > w_ptr->max_m_idx) {
        load_note(format(_("モンスターの配列が大きすぎる(%d)！", "Too many (%d) monster entries!"), limit));
        return 161;
    }

    auto monster_loader = MonsterLoaderFactory::create_loader();
    for (int i = 1; i < limit; i++) {
        auto m_idx = m_pop(floor_ptr);
        if (i != m_idx) {
            load_note(format(_("モンスター配置エラー (%d <> %d)", "Monster allocation error (%d <> %d)"), i, m_idx));
            return 162;
        }

        auto m_ptr = &floor_ptr->m_list[m_idx];
        monster_loader->rd_monster(m_ptr);
        auto *g_ptr = &floor_ptr->grid_array[m_ptr->fy][m_ptr->fx];
        g_ptr->m_idx = m_idx;
        real_r_ptr(m_ptr)->cur_num++;
    }

    if (h_older_than(0, 3, 13) && !floor_ptr->dun_level && !floor_ptr->inside_arena) {
        w_ptr->character_dungeon = false;
    } else {
        w_ptr->character_dungeon = true;
    }

    return 0;
}
