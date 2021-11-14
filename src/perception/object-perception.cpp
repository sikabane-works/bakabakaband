﻿#include "perception/object-perception.h"
#include "flavor/flavor-describer.h"
#include "flavor/object-flavor-types.h"
#include "game-option/play-record-options.h"
#include "io/write-diary.h"
#include "object-enchant/item-feeling.h"
#include "object-enchant/special-object-flags.h"
#include "object-enchant/trg-types.h"
#include "object/item-tester-hooker.h" // 暫定、このファイルへ引っ越す.
#include "object/object-kind.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"

/*!
 * @brief オブジェクトを鑑定済にする /
 * Known is true when the "attributes" of an object are "known".
 * @param o_ptr 鑑定済にするオブジェクトの構造体参照ポインタ
 * These include tohit, todam, toac, cost, and pval (charges).\n
 *\n
 * Note that "knowing" an object gives you everything that an "awareness"\n
 * gives you, and much more.  In fact, the player is always "aware" of any\n
 * item of which he has full "knowledge".\n
 *\n
 * But having full knowledge of, say, one "wand of wonder", does not, by\n
 * itself, give you knowledge, or even awareness, of other "wands of wonder".\n
 * It happens that most "identify" routines (including "buying from a shop")\n
 * will make the player "aware" of the object as well as fully "know" it.\n
 *\n
 * This routine also removes any inscriptions generated by "feelings".\n
 */
void object_known(object_type *o_ptr)
{
    o_ptr->feeling = FEEL_NONE;
    o_ptr->ident &= ~(IDENT_SENSE);
    o_ptr->ident &= ~(IDENT_EMPTY);
    o_ptr->ident |= (IDENT_KNOWN);
}

/*!
 * @brief オブジェクトを＊鑑定＊済にする /
 * The player is now aware of the effects of the given object.
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param o_ptr ＊鑑定＊済にするオブジェクトの構造体参照ポインタ
 */
void object_aware(PlayerType *player_ptr, object_type *o_ptr)
{
    const bool is_already_awared = o_ptr->is_aware();

    k_info[o_ptr->k_idx].aware = true;

    // 以下、playrecordに記録しない場合はreturnする
    if (!record_ident)
        return;

    if (is_already_awared || player_ptr->is_dead)
        return;

    // アーティファクト専用ベースアイテムは記録しない
    if (k_info[o_ptr->k_idx].gen_flags.has(TRG::INSTA_ART))
        return;

    // 未鑑定名の無いアイテムは記録しない
    if (!((o_ptr->tval >= ItemKindType::AMULET && o_ptr->tval <= ItemKindType::POTION) || o_ptr->tval == ItemKindType::FOOD))
        return;

    // playrecordに識別したアイテムを記録
    object_type forge;
    object_type *q_ptr;
    GAME_TEXT o_name[MAX_NLEN];

    q_ptr = &forge;
    q_ptr->copy_from(o_ptr);

    q_ptr->number = 1;
    describe_flavor(player_ptr, o_name, q_ptr, OD_NAME_ONLY);

    exe_write_diary(player_ptr, DIARY_FOUND, 0, o_name);
}

/*!
 * @brief オブジェクトを試行済にする /
 * Something has been "sampled"
 * @param o_ptr 試行済にするオブジェクトの構造体参照ポインタ
 */
void object_tried(object_type *o_ptr) { k_info[o_ptr->k_idx].tried = true; }
