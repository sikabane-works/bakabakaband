﻿/*!
 * @brief モンスター死亡時の特殊処理switch (一般的な処理もdefaultで実施)
 * @date 2020/08/21
 * @author Hourier
 */

#include "monster-floor/special-death-switcher.h"
#include "artifact/fixed-art-generator.h"
#include "artifact/fixed-art-types.h"
#include "artifact/random-art-generator.h"
#include "effect/attribute-types.h"
#include "effect/effect-characteristics.h"
#include "effect/effect-processor.h"
#include "floor/cave.h"
#include "floor/floor-object.h"
#include "floor/floor-util.h"
#include "floor/geometry.h"
#include "game-option/birth-options.h"
#include "grid/grid.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "monster-floor/monster-death-util.h"
#include "monster-floor/monster-death.h"
#include "monster-floor/monster-summon.h"
#include "monster-floor/place-monster-types.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags8.h"
#include "monster-race/race-indice-types.h"
#include "monster/monster-describer.h"
#include "monster/monster-description-types.h"
#include "monster/monster-info.h"
#include "object-enchant/item-apply-magic.h"
#include "object-enchant/item-magic-applier.h"
#include "object/object-kind-hook.h"
#include "spell/summon-types.h"
#include "sv-definition/sv-food-types.h"
#include "sv-definition/sv-other-types.h"
#include "sv-definition/sv-potion-types.h"
#include "sv-definition/sv-protector-types.h"
#include "sv-definition/sv-weapon-types.h"
#include "system/artifact-type-definition.h"
#include "system/baseitem-info-definition.h"
#include "system/floor-type-definition.h"
#include "system/item-entity.h"
#include "system/monster-race-info.h"
#include "system/monster-type-definition.h"
#include "system/player-type-definition.h"
#include "view/display-messages.h"
#include "world/world.h"

/*!
 * @brief 死亡召喚に使用するモード選択
 * @param md_ptr モンスター撃破構造体への参照ポインタ
 * @return 撃破モンスターがPETであればPM_FORCE_PETを、CLONEであればPM_CLONEを立てる
 */
static BIT_FLAGS dead_mode(monster_death_type *md_ptr)
{
    bool pet = md_ptr->m_ptr->is_pet();
    bool clone = md_ptr->m_ptr->mflag2.has(MonsterConstantFlagType::CLONED);
    BIT_FLAGS mode = pet ? PM_FORCE_PET : PM_NONE;
    if (clone) {
        mode = mode | PM_CLONE;
    }

    return mode;
}

/*!
 * @brief 死亡時召喚処理 (今のところ自分自身のみ)
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param md_ptr モンスター撃破構造体への参照ポインタ
 * @param type 召喚タイプ
 * @param probability 召喚確率 (計算式：1 - 1/probability)
 * @param radius 召喚半径 (モンスターが死亡した座標から半径何マス以内に召喚させるか)
 * @param message 召喚時のメッセージ
 */
static void summon_self(PlayerType *player_ptr, monster_death_type *md_ptr, summon_type type, int probability, POSITION radius, concptr message)
{
    auto *floor_ptr = player_ptr->current_floor_ptr;
    if (floor_ptr->inside_arena || player_ptr->phase_out || one_in_(probability)) {
        return;
    }

    POSITION wy = md_ptr->md_y;
    POSITION wx = md_ptr->md_x;
    int attempts = 100;
    bool pet = md_ptr->m_ptr->is_pet();
    do {
        scatter(player_ptr, &wy, &wx, md_ptr->md_y, md_ptr->md_x, radius, PROJECT_NONE);
    } while (!(in_bounds(floor_ptr, wy, wx) && is_cave_empty_bold2(player_ptr, wy, wx)) && --attempts);

    if (attempts <= 0) {
        return;
    }

    BIT_FLAGS mode = dead_mode(md_ptr);
    if (summon_specific(player_ptr, (pet ? -1 : md_ptr->m_idx), wy, wx, 100, type, mode) && player_can_see_bold(player_ptr, wy, wx)) {
        msg_print(message);
    }
}

static void on_dead_pink_horror(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (player_ptr->current_floor_ptr->inside_arena || player_ptr->phase_out) {
        return;
    }

    bool notice = false;
    const int blue_horrors = 2;
    for (int i = 0; i < blue_horrors; i++) {
        POSITION wy = md_ptr->md_y;
        POSITION wx = md_ptr->md_x;
        bool pet = md_ptr->m_ptr->is_pet();
        BIT_FLAGS mode = dead_mode(md_ptr);
        if (summon_specific(player_ptr, (pet ? -1 : md_ptr->m_idx), wy, wx, 100, SUMMON_BLUE_HORROR, mode) && player_can_see_bold(player_ptr, wy, wx)) {
            notice = true;
        }
    }

    if (notice) {
        sound(SOUND_SUMMON);
        msg_print(_("ピンク・ホラーは分裂した！", "The Pink horror divides!"));
    }
}

static void on_dead_spawn_monsters(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    bool notice = false;

    for (auto race : md_ptr->r_ptr->dead_spawns) {
        int num = std::get<0>(race);
        int deno = std::get<1>(race);
        if (randint1(deno) > num) {
            continue;
        }
        auto r_idx = std::get<2>(race);
        int dn = std::get<3>(race);
        int ds = std::get<4>(race);
        int spawn_nums = damroll(dn, ds);
        POSITION wy = md_ptr->md_y;
        POSITION wx = md_ptr->md_x;
        bool pet = md_ptr->m_ptr->is_pet();
        BIT_FLAGS mode = pet ? PM_FORCE_PET : PM_NONE;
        for (int i = 0; i < spawn_nums; i++) {
            if (summon_named_creature(player_ptr, 0, wy, wx, r_idx, mode) && player_can_see_bold(player_ptr, wy, wx)) {
                notice = true;
            }
        }
    }

    if (notice) {
        sound(SOUND_SUMMON);
    }
}

static void on_dead_drop_kind_item(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    for (auto kind : md_ptr->r_ptr->drop_kinds) {
        ItemEntity forge;
        ItemEntity *q_ptr = &forge;
        int num = std::get<0>(kind);
        int deno = std::get<1>(kind);
        if (randint1(deno) > num) {
            continue;
        }
        int kind_idx = std::get<2>(kind);
        int grade = std::get<3>(kind);
        int dn = std::get<4>(kind);
        int ds = std::get<5>(kind);
        int drop_nums = damroll(dn, ds);

        for (int i = 0; i < drop_nums; i++) {
            q_ptr->prep(kind_idx);
            switch (grade) {
            /* Apply bad magic, but first clear object */
            case -2:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | AM_GOOD | AM_GREAT | AM_CURSED).execute();
                break;
            /* Apply bad magic, but first clear object */
            case -1:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | AM_GOOD | AM_CURSED).execute();
                break;
            /* Apply normal magic, but first clear object */
            case 0:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART).execute();
                break;
            /* Apply good magic, but first clear object */
            case 1:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | AM_GOOD).execute();
                break;
            /* Apply great magic, but first clear object */
            case 2:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | AM_GOOD | AM_GREAT).execute();
                break;
            /* Apply special magic, but first clear object */
            case 3:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_GOOD | AM_GREAT | AM_SPECIAL).execute();
                if (!q_ptr->is_fixed_artifact()) {
                    become_random_artifact(player_ptr, q_ptr, false);
                }
                break;
            default:
                break;
            }
            (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
        }
    }
}

static void on_dead_drop_tval_item(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    for (auto kind : md_ptr->r_ptr->drop_tvals) {
        ItemEntity forge;
        ItemEntity *q_ptr = &forge;
        int num = std::get<0>(kind);
        int deno = std::get<1>(kind);
        if (randint1(deno) > num) {
            continue;
        }
        int tval = std::get<2>(kind);
        int grade = std::get<3>(kind);
        int dn = std::get<4>(kind);
        int ds = std::get<5>(kind);
        int drop_nums = damroll(dn, ds);

        for (int i = 0; i < drop_nums; i++) {
            q_ptr->prep(lookup_baseitem_id({ i2enum<ItemKindType>(tval), 0 }));
            switch (grade) {
            /* Apply bad magic, but first clear object */
            case -2:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | AM_GOOD | AM_GREAT | AM_CURSED).execute();
                break;
            /* Apply bad magic, but first clear object */
            case -1:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | AM_GOOD | AM_CURSED).execute();
                break;
            /* Apply normal magic, but first clear object */
            case 0:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART).execute();
                break;
            /* Apply good magic, but first clear object */
            case 1:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | AM_GOOD).execute();
                break;
            /* Apply great magic, but first clear object */
            case 2:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | AM_GOOD | AM_GREAT).execute();
                break;
            /* Apply special magic, but first clear object */
            case 3:
                ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_GOOD | AM_GREAT | AM_SPECIAL).execute();
                if (!q_ptr->is_fixed_artifact()) {
                    become_random_artifact(player_ptr, q_ptr, false);
                }
                break;
            default:
                break;
            }
            (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
        }
    }
}

static void on_dead_bottle_gnome(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    ItemEntity forge;
    ItemEntity *q_ptr = &forge;
    q_ptr->prep(lookup_baseitem_id({ ItemKindType::POTION, SV_POTION_CURE_CRITICAL }));
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

static void on_dead_bloodletter(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (!md_ptr->drop_chosen_item || (randint1(100) >= 15)) {
        return;
    }

    ItemEntity forge;
    auto *q_ptr = &forge;
    q_ptr->prep(lookup_baseitem_id({ ItemKindType::SWORD, SV_BLADE_OF_CHAOS }));
    ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->object_level, AM_NO_FIXED_ART | md_ptr->mo_mode).execute();
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

static void on_dead_inariman1_2(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    ItemEntity forge;
    ItemEntity *q_ptr = &forge;
    q_ptr->prep(lookup_baseitem_id({ ItemKindType::FOOD, SV_FOOD_SUSHI2 }));
    ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | md_ptr->mo_mode).execute();
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

static void on_dead_inariman3(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    ItemEntity forge;
    ItemEntity *q_ptr = &forge;
    q_ptr->prep(lookup_baseitem_id({ ItemKindType::FOOD, SV_FOOD_SUSHI3 }));
    ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART | md_ptr->mo_mode).execute();
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

static void on_dead_raal(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    auto *floor_ptr = player_ptr->current_floor_ptr;
    if (!md_ptr->drop_chosen_item || (floor_ptr->dun_level <= 9)) {
        return;
    }

    ItemEntity forge;
    auto *q_ptr = &forge;
    q_ptr->wipe();
    if ((floor_ptr->dun_level > 49) && one_in_(5)) {
        get_obj_index_hook = kind_is_good_book;
    } else {
        get_obj_index_hook = kind_is_book;
    }

    (void)make_object(player_ptr, q_ptr, md_ptr->mo_mode);
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

/*!
 * @brief 6/7の確率で、20マス以内に暁の戦士自身を召喚する
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param md_ptr モンスター撃破構造体への参照ポインタ
 */
static void on_dead_dawn(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    summon_self(player_ptr, md_ptr, SUMMON_DAWN, 7, 20, _("新たな戦士が現れた！", "A new warrior steps forth!"));
}

static void on_dead_ninja(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (is_seen(player_ptr, md_ptr->m_ptr)) {
        GAME_TEXT m_name[MAX_NLEN];
        msg_print(_("「サヨナラ！」", "Sayonara!"));
        monster_desc(player_ptr, m_name, md_ptr->m_ptr, MD_NONE);
        msg_format(_("%sは哀れ爆発四散した！ショッギョ・ムッジョ！", "%s explodes pitifully! Shogyomujo!"), m_name);
    }

    (void)project(player_ptr, md_ptr->m_idx, 6, md_ptr->md_y, md_ptr->md_x, 20, AttributeType::MISSILE, PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL);
}

static void on_dead_earth_destroyer(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    msg_print(_("ワーオ！22世紀の文明の叡知が今炸裂した！", "Wow! The wisdom of 22nd century civilization has now exploded!"));
    (void)project(player_ptr, md_ptr->m_idx, 10, md_ptr->md_y, md_ptr->md_x, 10000, AttributeType::DISINTEGRATE, PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL);
}

static void on_dead_sacred_treasures(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if ((player_ptr->ppersonality != PERSONALITY_LAZY) || !md_ptr->drop_chosen_item) {
        return;
    }

    FixedArtifactId a_idx = FixedArtifactId::NONE;
    ArtifactType *a_ptr = nullptr;
    do {
        switch (randint0(3)) {
        case 0:
            a_idx = FixedArtifactId::NAMAKE_HAMMER;
            break;
        case 1:
            a_idx = FixedArtifactId::NAMAKE_BOW;
            break;
        case 2:
            a_idx = FixedArtifactId::NAMAKE_ARMOR;
            break;
        }

        a_ptr = &artifacts_info.at(a_idx);
    } while (a_ptr->is_generated);

    if (!create_named_art(player_ptr, a_idx, md_ptr->md_y, md_ptr->md_x)) {
        return;
    }

    if (w_ptr->character_dungeon) {
        a_ptr->floor_id = player_ptr->floor_id;
    }
}

static void on_dead_serpent(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (!md_ptr->drop_chosen_item) {
        return;
    }

    ItemEntity forge;
    auto *q_ptr = &forge;
    q_ptr->prep(lookup_baseitem_id({ ItemKindType::HAFTED, SV_GROND }));
    q_ptr->fixed_artifact_idx = FixedArtifactId::GROND;
    ItemMagicApplier(player_ptr, q_ptr, -1, AM_GOOD | AM_GREAT).execute();
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
    q_ptr = &forge;
    q_ptr->prep(lookup_baseitem_id({ ItemKindType::CROWN, SV_CHAOS }));
    q_ptr->fixed_artifact_idx = FixedArtifactId::CHAOS;
    ItemMagicApplier(player_ptr, q_ptr, -1, AM_GOOD | AM_GREAT).execute();
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

static void on_dead_death_sword(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (!md_ptr->drop_chosen_item) {
        return;
    }

    ItemEntity forge;
    auto *q_ptr = &forge;
    q_ptr->prep(lookup_baseitem_id({ ItemKindType::SWORD, randint1(2) }));
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

static void on_dead_can_angel(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    bool is_drop_can = md_ptr->drop_chosen_item;
    bool is_silver = md_ptr->m_ptr->r_idx == MonsterRaceId::A_SILVER;
    is_silver &= md_ptr->r_ptr->r_akills % 5 == 0;
    is_drop_can &= (md_ptr->m_ptr->r_idx == MonsterRaceId::A_GOLD) || is_silver;
    if (!is_drop_can) {
        return;
    }

    ItemEntity forge;
    auto *q_ptr = &forge;
    q_ptr->prep(lookup_baseitem_id({ ItemKindType::CHEST, SV_CHEST_KANDUME }));
    ItemMagicApplier(player_ptr, q_ptr, player_ptr->current_floor_ptr->object_level, AM_NO_FIXED_ART).execute();
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

static void on_dead_aqua_illusion(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (player_ptr->current_floor_ptr->inside_arena || player_ptr->phase_out) {
        return;
    }

    bool notice = false;
    const int popped_bubbles = 4;
    for (int i = 0; i < popped_bubbles; i++) {
        POSITION wy = md_ptr->md_y;
        POSITION wx = md_ptr->md_x;
        bool pet = md_ptr->m_ptr->is_pet();
        BIT_FLAGS mode = dead_mode(md_ptr);
        auto smaller_bubble = md_ptr->m_ptr->r_idx - 1;
        if (summon_named_creature(player_ptr, (pet ? -1 : md_ptr->m_idx), wy, wx, smaller_bubble, mode) && player_can_see_bold(player_ptr, wy, wx)) {
            notice = true;
        }
    }

    if (notice) {
        msg_print(_("泡が弾けた！", "The bubble pops!"));
        sound(SOUND_SUMMON);
    }
}

/*!
 * @brief 7/8の確率で、5マス以内にトーテムモアイ自身を召喚する
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param md_ptr モンスター撃破構造体への参照ポインタ
 */
static void on_dead_totem_moai(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    summon_self(player_ptr, md_ptr, SUMMON_TOTEM_MOAI, 8, 5, _("新たなモアイが現れた！", "A new moai steps forth!"));
}

static void on_dead_dragon_centipede(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (player_ptr->current_floor_ptr->inside_arena || player_ptr->phase_out) {
        return;
    }

    bool notice = false;
    const int reproduced_centipede = 2;
    for (int i = 0; i < reproduced_centipede; i++) {
        POSITION wy = md_ptr->md_y;
        POSITION wx = md_ptr->md_x;
        bool pet = md_ptr->m_ptr->is_pet();
        BIT_FLAGS mode = dead_mode(md_ptr);

        auto smaller_centipede = md_ptr->m_ptr->r_idx - 1;
        if (summon_named_creature(player_ptr, (pet ? -1 : md_ptr->m_idx), wy, wx, smaller_centipede, mode) && player_can_see_bold(player_ptr, wy, wx)) {
            notice = true;
        }
    }

    GAME_TEXT m_name[MAX_NLEN];
    monster_desc(player_ptr, m_name, md_ptr->m_ptr, MD_NONE);
    if (notice) {
        msg_format(_("%sが再生した！", "The %s reproduced!"), m_name);
        sound(SOUND_SUMMON);
    }
}

/*
 * @brief 装備品の生成を試みる
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param q_ptr 生成中アイテムへの参照ポインタ
 * @param drop_mode ドロップ品の質
 * @param is_object_hook_null アイテム種別が何でもありならtrue、指定されていればfalse
 * @return 生成したアイテムが装備品ならtrue、それ以外ならfalse
 * @todo 汎用的に使えそうだがどこかにいいファイルはないか？
 */
static bool make_equipment(PlayerType *player_ptr, ItemEntity *q_ptr, const BIT_FLAGS drop_mode, const bool is_object_hook_null)
{
    q_ptr->wipe();
    (void)make_object(player_ptr, q_ptr, drop_mode);
    if (!is_object_hook_null) {
        return true;
    }

    auto tval = q_ptr->tval;
    switch (tval) {
    case ItemKindType::BOW:
    case ItemKindType::DIGGING:
    case ItemKindType::HAFTED:
    case ItemKindType::POLEARM:
    case ItemKindType::SWORD:
    case ItemKindType::BOOTS:
    case ItemKindType::GLOVES:
    case ItemKindType::HELM:
    case ItemKindType::CROWN:
    case ItemKindType::SHIELD:
    case ItemKindType::CLOAK:
    case ItemKindType::SOFT_ARMOR:
    case ItemKindType::HARD_ARMOR:
    case ItemKindType::DRAG_ARMOR:
    case ItemKindType::LITE:
    case ItemKindType::AMULET:
    case ItemKindType::RING:
        return true;
    default:
        return false;
    }
}

/*
 * @brief 死亡時ドロップとしてランダムアーティファクトのみを生成する
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param md_ptr モンスター撃破構造体への参照ポインタ
 * @param object_hook_pf アイテム種別指定、特になければnullptrで良い
 * @return なし
 * @details
 * 最初のアイテム生成でいきなり☆が生成された場合を除き、中途半端な☆ (例：呪われている)は生成しない.
 * このルーチンで★は生成されないので、★生成フラグのキャンセルも不要
 */
static void on_dead_random_artifact(PlayerType *player_ptr, monster_death_type *md_ptr, bool (*object_hook_pf)(KIND_OBJECT_IDX k_idx))
{
    ItemEntity forge;
    auto *q_ptr = &forge;
    auto is_object_hook_null = object_hook_pf == nullptr;
    auto drop_mode = md_ptr->mo_mode | AM_NO_FIXED_ART;
    while (true) {
        // make_object() の中でアイテム種別をキャンセルしている
        // よってこのwhileループ中へ入れないと、引数で指定していない種別のアイテムが選ばれる可能性がある
        get_obj_index_hook = object_hook_pf;
        if (!make_equipment(player_ptr, q_ptr, drop_mode, is_object_hook_null)) {
            continue;
        }

        if (q_ptr->art_name > 0) {
            break;
        }

        if (q_ptr->is_ego()) {
            continue;
        }

        (void)become_random_artifact(player_ptr, q_ptr, false);
        auto is_good_random_art = !q_ptr->is_cursed();
        is_good_random_art &= q_ptr->to_h > 0;
        is_good_random_art &= q_ptr->to_d > 0;
        is_good_random_art &= q_ptr->to_a > 0;
        is_good_random_art &= q_ptr->pval > 0;
        if (is_good_random_art) {
            break;
        }
    }

    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

/*!
 * @brief マニマニのあくま撃破時メッセージ
 * @todo 死亡時の特殊メッセージを表示するだけの処理を複数作るなら、switch/case文に分けられるように汎用化すること
 */
static void on_dead_manimani(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (!is_seen(player_ptr, md_ptr->m_ptr)) {
        return;
    }

    msg_print(_("どこからか声が聞こえる…「ハロー！　そして…グッドバイ！」", "Heard a voice from somewhere... 'Hello! And... good bye!'"));
}

static void drop_specific_item_on_dead(PlayerType *player_ptr, monster_death_type *md_ptr, bool (*object_hook_pf)(KIND_OBJECT_IDX k_idx))
{
    ItemEntity forge;
    auto *q_ptr = &forge;
    q_ptr->wipe();
    get_obj_index_hook = object_hook_pf;
    (void)make_object(player_ptr, q_ptr, md_ptr->mo_mode);
    (void)drop_near(player_ptr, q_ptr, -1, md_ptr->md_y, md_ptr->md_x);
}

static void on_dead_chest_mimic(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (player_ptr->current_floor_ptr->inside_arena || player_ptr->phase_out) {
        return;
    }

    bool notice = false;
    auto mimic_inside = MonsterRace::empty_id();
    auto num_summons = 0;
    auto r_idx = md_ptr->m_ptr->r_idx;
    switch (r_idx) {
    case MonsterRaceId::CHEST_MIMIC_03:
        mimic_inside = MonsterRaceId::CHEST_MIMIC_02;
        num_summons = 1;
        break;
    case MonsterRaceId::CHEST_MIMIC_04:
        mimic_inside = MonsterRaceId::CHEST_MIMIC_03;
        num_summons = 1;
        break;
    case MonsterRaceId::CHEST_MIMIC_11:
        mimic_inside = MonsterRaceId::CHEST_MIMIC_04;
        num_summons = one_in_(2) ? 3 : 2;
        break;
    default:
        return;
    }

    for (auto i = 0; i < num_summons; i++) {
        auto wy = md_ptr->md_y;
        auto wx = md_ptr->md_x;
        auto pet = md_ptr->m_ptr->is_pet();
        BIT_FLAGS mode = dead_mode(md_ptr);
        if (summon_named_creature(player_ptr, (pet ? -1 : md_ptr->m_idx), wy, wx, mimic_inside, (BIT_FLAGS)mode) && player_can_see_bold(player_ptr, wy, wx)) {
            notice = true;
        }
    }

    if (notice) {
        msg_print(_("箱の中から新たなミミックが現れた！", "A new mimic appears in the dead mimic!"));
        sound(SOUND_SUMMON);
    }
}

static void on_dead_mimics(PlayerType *player_ptr, monster_death_type *md_ptr)
{
    if (!md_ptr->drop_chosen_item) {
        return;
    }

    switch (md_ptr->r_ptr->d_char) {
    case '(':
        if (player_ptr->current_floor_ptr->dun_level <= 0) {
            return;
        }

        drop_specific_item_on_dead(player_ptr, md_ptr, kind_is_cloak);
        return;
    case '/':
        if (player_ptr->current_floor_ptr->dun_level <= 4) {
            return;
        }

        drop_specific_item_on_dead(player_ptr, md_ptr, kind_is_polearm);
        return;
    case '[':
        if (player_ptr->current_floor_ptr->dun_level <= 19) {
            return;
        }

        drop_specific_item_on_dead(player_ptr, md_ptr, kind_is_armor);
        return;
    case '\\':
        if (player_ptr->current_floor_ptr->dun_level <= 4) {
            return;
        }

        drop_specific_item_on_dead(player_ptr, md_ptr, kind_is_hafted);
        return;
    case '|':
        if (md_ptr->m_ptr->r_idx == MonsterRaceId::STORMBRINGER) {
            return;
        }

        drop_specific_item_on_dead(player_ptr, md_ptr, kind_is_sword);
        return;
    case ']':
        if (player_ptr->current_floor_ptr->dun_level <= 19) {
            return;
        }

        drop_specific_item_on_dead(player_ptr, md_ptr, kind_is_boots);
        return;
    default:
        return;
    }
}

static void on_dead_swordfish(PlayerType *player_ptr, monster_death_type *md_ptr, AttributeFlags attribute_flags)
{
    if (attribute_flags.has_not(AttributeType::COLD) || !md_ptr->drop_chosen_item || (randint1(100) >= 10)) {
        return;
    }

    drop_single_artifact(player_ptr, md_ptr, FixedArtifactId::FROZEN_SWORDFISH);
}

void switch_special_death(PlayerType *player_ptr, monster_death_type *md_ptr, AttributeFlags attribute_flags)
{
    if (md_ptr->r_ptr->kind_flags.has(MonsterKindType::NINJA)) {
        on_dead_ninja(player_ptr, md_ptr);
        return;
    }
    on_dead_drop_kind_item(player_ptr, md_ptr);
    on_dead_drop_tval_item(player_ptr, md_ptr);
    on_dead_spawn_monsters(player_ptr, md_ptr);

    switch (md_ptr->m_ptr->r_idx) {
    case MonsterRaceId::EARTH_DESTROYER:
        on_dead_earth_destroyer(player_ptr, md_ptr);
        break;
    case MonsterRaceId::BOTTLE_GNOME:
        on_dead_bottle_gnome(player_ptr, md_ptr);
        return;
    case MonsterRaceId::PINK_HORROR:
        on_dead_pink_horror(player_ptr, md_ptr);
        return;
    case MonsterRaceId::BLOODLETTER:
        on_dead_bloodletter(player_ptr, md_ptr);
        return;
    case MonsterRaceId::RAAL:
        on_dead_raal(player_ptr, md_ptr);
        return;
    case MonsterRaceId::DAWN:
        on_dead_dawn(player_ptr, md_ptr);
        return;
    case MonsterRaceId::UNMAKER:
        (void)project(player_ptr, md_ptr->m_idx, 6, md_ptr->md_y, md_ptr->md_x, 100, AttributeType::CHAOS, PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL);
        break;
    case MonsterRaceId::UNICORN_ORD:
    case MonsterRaceId::MORGOTH:
    case MonsterRaceId::ONE_RING:
        on_dead_sacred_treasures(player_ptr, md_ptr);
        return;
    case MonsterRaceId::SERPENT:
        on_dead_serpent(player_ptr, md_ptr);
        return;
    case MonsterRaceId::B_DEATH_SWORD:
        on_dead_death_sword(player_ptr, md_ptr);
        return;
    case MonsterRaceId::A_GOLD:
    case MonsterRaceId::A_SILVER:
        on_dead_can_angel(player_ptr, md_ptr);
        return;
    case MonsterRaceId::ROLENTO:
        (void)project(player_ptr, md_ptr->m_idx, 3, md_ptr->md_y, md_ptr->md_x, damroll(20, 10), AttributeType::FIRE, PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL);
        return;
    case MonsterRaceId::MIDDLE_AQUA_FIRST:
    case MonsterRaceId::LARGE_AQUA_FIRST:
    case MonsterRaceId::EXTRA_LARGE_AQUA_FIRST:
    case MonsterRaceId::MIDDLE_AQUA_SECOND:
    case MonsterRaceId::LARGE_AQUA_SECOND:
    case MonsterRaceId::EXTRA_LARGE_AQUA_SECOND:
        on_dead_aqua_illusion(player_ptr, md_ptr);
        return;
    case MonsterRaceId::TOTEM_MOAI:
        on_dead_totem_moai(player_ptr, md_ptr);
        return;
    case MonsterRaceId::DRAGON_CENTIPEDE:
    case MonsterRaceId::DRAGON_WORM:
        on_dead_dragon_centipede(player_ptr, md_ptr);
        return;
    case MonsterRaceId::CAIT_SITH:
        drop_specific_item_on_dead(player_ptr, md_ptr, kind_is_boots);
        return;
    case MonsterRaceId::YENDOR_WIZARD_1:
        on_dead_random_artifact(player_ptr, md_ptr, kind_is_amulet);
        return;
    case MonsterRaceId::YENDOR_WIZARD_2:
        drop_specific_item_on_dead(player_ptr, md_ptr, kind_is_amulet);
        return;
    case MonsterRaceId::MANIMANI:
        on_dead_manimani(player_ptr, md_ptr);
        return;
    case MonsterRaceId::LOSTRINGIL:
        on_dead_random_artifact(player_ptr, md_ptr, kind_is_sword);
        return;
    case MonsterRaceId::INARIMAN_2:
        on_dead_inariman1_2(player_ptr, md_ptr);
        return;
    case MonsterRaceId::INARIMAN_3:
        on_dead_inariman3(player_ptr, md_ptr);
        return;
    case MonsterRaceId::SWORDFISH:
        on_dead_swordfish(player_ptr, md_ptr, attribute_flags);
        return;
    case MonsterRaceId::CHEST_MIMIC_03:
    case MonsterRaceId::CHEST_MIMIC_04:
    case MonsterRaceId::CHEST_MIMIC_11:
        on_dead_chest_mimic(player_ptr, md_ptr);
        on_dead_mimics(player_ptr, md_ptr);
        return;
    default:
        return;
    }
}
