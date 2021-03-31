#include "store/purchase-order.h"
#include "autopick/autopick.h"
#include "core/asking-player.h"
#include "core/stuff-handler.h"
#include "flavor/flavor-describer.h"
#include "flavor/object-flavor-types.h"
#include "game-option/birth-options.h"
#include "game-option/play-record-options.h"
#include "grid/feature.h"
#include "inventory/inventory-object.h"
#include "io/write-diary.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "object-enchant/item-feeling.h"
#include "object-enchant/special-object-flags.h"
#include "object/object-generator.h"
#include "object/object-info.h"
#include "object/object-stack.h"
#include "object/object-value.h"
#include "perception/object-perception.h"
#include "player-info/avatar.h"
#include "player/race-info-table.h"
#include "store/home.h"
#include "store/owner-insults.h"
#include "store/pricing.h"
#include "store/say-comments.h"
#include "store/store-util.h"
#include "store/store.h"
#include "term/screen-processor.h"
#include "util/int-char-converter.h"
#include "view/display-messages.h"
#include "view/display-store.h"
#include "world/world.h"
#include "io/input-key-acceptor.h"

typedef struct haggle_type {
    object_type *o_ptr;
    s32b *price;
    s32b cur_ask;
    s32b final_ask;
    int noneed;
    bool final;
    concptr pmt;
    s32b min_per;
    s32b max_per;
    s32b last_offer;
    s32b offer;
    bool flag;
    int annoyed;
    bool cancel;
} haggle_type;

static bool show_store_select_item(COMMAND_CODE *item, const int i)
{
    char out_val[160];
#ifdef JP
    /* ブラックマーケットの時は別のメッセージ */
    switch (cur_store_num) {
    case 7:
        sprintf(out_val, "どのアイテムを取りますか? ");
        break;
    case 6:
        sprintf(out_val, "どれ? ");
        break;
    default:
        sprintf(out_val, "どの品物が欲しいんだい? ");
        break;
    }
#else
    if (cur_store_num == STORE_HOME) {
        sprintf(out_val, "Which item do you want to take? ");
    } else {
        sprintf(out_val, "Which item are you interested in? ");
    }
#endif

    return get_stock(item, out_val, 0, i - 1) != 0;
}

static bool process_purchase_result(player_type *player_ptr, object_type *o_ptr, object_type *j_ptr, COMMAND_CODE *item_new,const int amt, int *i, const COMMAND_CODE item)
{
    if (cur_store_num != STORE_HOME)
        return FALSE;

    bool combined_or_reordered;
    distribute_charges(o_ptr, j_ptr, amt);
    *item_new = store_item_to_inventory(player_ptr, j_ptr);
    GAME_TEXT o_name[MAX_NLEN];
    describe_flavor(player_ptr, o_name, &player_ptr->inventory_list[*item_new], 0);
    handle_stuff(player_ptr);
    msg_format(_("%s(%c)を取った。", "You have %s (%c)."), o_name, index_to_label(*item_new));
    *i = st_ptr->stock_num;
    store_item_increase(item, -amt);
    store_item_optimize(item);
    combined_or_reordered = combine_and_reorder_home(player_ptr, STORE_HOME);
    if (*i == st_ptr->stock_num) {
        if (combined_or_reordered)
            display_store_inventory(player_ptr);
        else
            display_entry(player_ptr, item);

        return TRUE;
    }

    if (st_ptr->stock_num == 0)
        store_top = 0;
    else if (store_top >= st_ptr->stock_num)
        store_top -= store_bottom;

    display_store_inventory(player_ptr);
    chg_virtue(player_ptr, V_SACRIFICE, 1);
    return TRUE;
}

static void shuffle_store(player_type *player_ptr)
{
    if (!one_in_(STORE_SHUFFLE)) {
        msg_print(_("店主は新たな在庫を取り出した。", "The shopkeeper brings out some new stock."));
        return;
    }

    char buf[80];
    msg_print(_("店主は引退した。", "The shopkeeper retires."));
    store_shuffle(player_ptr, cur_store_num);
    prt("", 3, 0);
    sprintf(buf, "%s (%s)", ot_ptr->owner_name, race_info[ot_ptr->owner_race].title);
    put_str(buf, 3, 10);
    sprintf(buf, "%s (%ld)", f_info[cur_store_feat].name.c_str(), (long)(ot_ptr->max_cost));
    prt(buf, 3, 50);
}

static void switch_store_stock(player_type *player_ptr, const int i, const COMMAND_CODE item)
{
    if (st_ptr->stock_num == 0) {
        shuffle_store(player_ptr);
        store_maintenance(player_ptr, player_ptr->town_num, cur_store_num, 10);

        store_top = 0;
        display_store_inventory(player_ptr);
        return;
    }
    
    if (st_ptr->stock_num != i) {
        if (store_top >= st_ptr->stock_num)
            store_top -= store_bottom;

        display_store_inventory(player_ptr);
        return;
    }

    display_entry(player_ptr, item);
}

/*!
 * @brief 店からの購入処理のメインルーチン /
 * Buy an item from a store 			-RAK-
 * @param player_ptr プレーヤーへの参照ポインタ
 * @return なし
 */
void store_purchase(player_type *player_ptr)
{
    if (cur_store_num == STORE_MUSEUM) {
        msg_print(_("博物館から取り出すことはできません。", "Items cannot be taken out of the Museum."));
        return;
    }

    if (st_ptr->stock_num <= 0) {
        if (cur_store_num == STORE_HOME)
            msg_print(_("我が家には何も置いてありません。", "Your home is empty."));
        else
            msg_print(_("現在商品の在庫を切らしています。", "I am currently out of stock."));
        return;
    }

    int i = (st_ptr->stock_num - store_top);
    if (i > store_bottom)
        i = store_bottom;

    COMMAND_CODE item;
    if (!show_store_select_item(&item, i))
        return;

    item = item + store_top;
    object_type *o_ptr;
    o_ptr = &st_ptr->stock[item];
    ITEM_NUMBER amt = 1;
    object_type forge;
    object_type *j_ptr;
    j_ptr = &forge;
    object_copy(j_ptr, o_ptr);

    /*
     * If a rod or wand, allocate total maximum timeouts or charges
     * between those purchased and left on the shelf.
     */
    reduce_charges(j_ptr, o_ptr->number - amt);
    j_ptr->number = amt;
    if (!check_store_item_to_inventory(player_ptr, j_ptr)) {
        msg_print(_("そんなにアイテムを持てない。", "You cannot carry that many different items."));
        return;
    }

    PRICE best = price_item(player_ptr, j_ptr, ot_ptr->min_inflate, FALSE);
    if (o_ptr->number > 1) {
        if ((cur_store_num != STORE_HOME) && (o_ptr->ident & IDENT_FIXED)) {
            msg_format(_("一つにつき $%ldです。", "That costs %ld gold per item."), (long)(best));
        }

        amt = get_quantity(NULL, o_ptr->number);
        if (amt <= 0)
            return;
    }

    j_ptr = &forge;
    object_copy(j_ptr, o_ptr);

    /*
     * If a rod or wand, allocate total maximum timeouts or charges
     * between those purchased and left on the shelf.
     */
    reduce_charges(j_ptr, o_ptr->number - amt);
    j_ptr->number = amt;
    if (!check_store_item_to_inventory(player_ptr, j_ptr)) {
        msg_print(_("ザックにそのアイテムを入れる隙間がない。", "You cannot carry that many items."));
        return;
    }

    COMMAND_CODE item_new;
    if (process_purchase_result(player_ptr, o_ptr, j_ptr, &item_new, amt, &i, item))
        return;

    int choice;
    PRICE price;
    choice = 0;
    char out_val[80];
    price = (best * j_ptr->number);

    if (choice != 0)
        return;

    if (price == (best * j_ptr->number))
        o_ptr->ident |= (IDENT_FIXED);

    if (player_ptr->au < price) {
        msg_print(_("お金が足りません。", "You do not have enough gold."));
        return;
    }

    sprintf(out_val, _("購入価格: $%d [Enter/Escape]", "Purchase price: $%d [Enter/Escape]"), price);
    put_str(out_val, 0, 0);
    while (TRUE) {
        char k = inkey();
        if (k == '\r') {
            break;        
        }
        if (k == ESCAPE) {
            put_str("                                                  ", 0, 0);
            return;
        }
    }

    if (cur_store_num == STORE_BLACK)
        chg_virtue(player_ptr, V_JUSTICE, -1);
    if ((o_ptr->tval == TV_BOTTLE) && (cur_store_num != STORE_HOME))
        chg_virtue(player_ptr, V_NATURE, -1);

    sound(SOUND_BUY);
    decrease_insults();
    player_ptr->au -= price;
    store_prt_gold(player_ptr);
    object_aware(player_ptr, j_ptr);
    j_ptr->ident &= ~(IDENT_FIXED);
    GAME_TEXT o_name[MAX_NLEN];
    describe_flavor(player_ptr, o_name, j_ptr, 0);

    msg_format(_("%sを $%ldで購入しました。", "You bought %s for %ld gold."), o_name, (long)price);

    strcpy(record_o_name, o_name);
    record_turn = current_world_ptr->game_turn;

    if (record_buy)
        exe_write_diary(player_ptr, DIARY_BUY, 0, o_name);
    describe_flavor(player_ptr, o_name, o_ptr, OD_NAME_ONLY);
    if (record_rand_art && o_ptr->art_name)
        exe_write_diary(player_ptr, DIARY_ART, 0, o_name);

    j_ptr->inscription = 0;
    j_ptr->feeling = FEEL_NONE;
    j_ptr->ident &= ~(IDENT_STORE);
    item_new = store_item_to_inventory(player_ptr, j_ptr);
    handle_stuff(player_ptr);

    describe_flavor(player_ptr, o_name, &player_ptr->inventory_list[item_new], 0);
    msg_format(_("%s(%c)を手に入れた。", "You have %s (%c)."), o_name, index_to_label(item_new));

    autopick_alter_item(player_ptr, item_new, FALSE);
    if ((o_ptr->tval == TV_ROD) || (o_ptr->tval == TV_WAND))
        o_ptr->pval -= j_ptr->pval;

    i = st_ptr->stock_num;
    store_item_increase(item, -amt);
    store_item_optimize(item);
    switch_store_stock(player_ptr, i, item);
}
