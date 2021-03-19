#include "store/sell-order.h"
#include "action/weapon-shield.h"
#include "autopick/autopick.h"
#include "core/asking-player.h"
#include "core/player-update-types.h"
#include "core/stuff-handler.h"
#include "core/window-redrawer.h"
#include "flavor/flavor-describer.h"
#include "flavor/object-flavor-types.h"
#include "floor/floor-object.h"
#include "game-option/birth-options.h"
#include "game-option/play-record-options.h"
#include "inventory/inventory-object.h"
#include "inventory/inventory-slot-types.h"
#include "io/write-diary.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "object-enchant/item-feeling.h"
#include "object-enchant/special-object-flags.h"
#include "object-hook/hook-checker.h"
#include "object/item-tester-hooker.h"
#include "object/item-use-flags.h"
#include "object/object-generator.h"
#include "object/object-info.h"
#include "object/object-stack.h"
#include "object/object-value.h"
#include "player-info/avatar.h"
#include "racial/racial-android.h"
#include "spell-kind/spells-perception.h"
#include "store/home.h"
#include "store/owner-insults.h"
#include "store/pricing.h"
#include "store/say-comments.h"
#include "store/service-checker.h"
#include "store/store-util.h"
#include "store/store.h"
#include "system/object-type-definition.h"
#include "term/screen-processor.h"
#include "view/display-messages.h"
#include "view/display-store.h"
#include "view/object-describer.h"
#include "util/bit-flags-calculator.h"
#include "world/world.h"
#include "io/input-key-acceptor.h"
#include "util/int-char-converter.h"


/*!
 * @brief 店からの売却処理のメインルーチン /
 * Sell an item to the store (or home)
 * @param owner_ptr プレーヤーへの参照ポインタ
 * @return なし
 */
void store_sell(player_type *owner_ptr)
{
    concptr q;
    if (cur_store_num == STORE_HOME)
        q = _("どのアイテムを置きますか? ", "Drop which item? ");
    else if (cur_store_num == STORE_MUSEUM)
        q = _("どのアイテムを寄贈しますか? ", "Give which item? ");
    else
        q = _("どのアイテムを売りますか? ", "Sell which item? ");

    item_tester_hook = store_will_buy;

    /* 我が家でおかしなメッセージが出るオリジナルのバグを修正 */
    concptr s;
    if (cur_store_num == STORE_HOME) {
        s = _("置けるアイテムを持っていません。", "You don't have any items to drop.");
    } else if (cur_store_num == STORE_MUSEUM) {
        s = _("寄贈できるアイテムを持っていません。", "You don't have any items to give.");
    } else {
        s = _("欲しい物がないですねえ。", "You have nothing that I want.");
    }

    OBJECT_IDX item;
    object_type *o_ptr;
    o_ptr = choose_object(owner_ptr, &item, q, s, USE_EQUIP | USE_INVEN | USE_FLOOR | IGNORE_BOTHHAND_SLOT, TV_NONE);
    if (!o_ptr)
        return;

    if ((item >= INVEN_MAIN_HAND) && object_is_cursed(o_ptr)) {
        msg_print(_("ふーむ、どうやらそれは呪われているようだね。", "Hmmm, it seems to be cursed."));
        return;
    }

    int amt = 1;
    if (o_ptr->number > 1) {
        amt = get_quantity(NULL, o_ptr->number);
        if (amt <= 0)
            return;
    }

    object_type forge;
    object_type *q_ptr;
    q_ptr = &forge;
    object_copy(q_ptr, o_ptr);
    q_ptr->number = amt;
    if ((o_ptr->tval == TV_ROD) || (o_ptr->tval == TV_WAND))
        q_ptr->pval = o_ptr->pval * amt / o_ptr->number;

    GAME_TEXT o_name[MAX_NLEN];
    describe_flavor(owner_ptr, o_name, q_ptr, 0);
    if ((cur_store_num != STORE_HOME) && (cur_store_num != STORE_MUSEUM)) {
        q_ptr->inscription = 0;
        q_ptr->feeling = FEEL_NONE;
    }

    if (!store_check_num(q_ptr)) {
        if (cur_store_num == STORE_HOME)
            msg_print(_("我が家にはもう置く場所がない。", "Your home is full."));

        else if (cur_store_num == STORE_MUSEUM)
            msg_print(_("博物館はもう満杯だ。", "The Museum is full."));

        else
            msg_print(_("すいませんが、店にはもう置く場所がありません。", "I have not the room in my store to keep it."));

        return;
    }

    int choice;
    PRICE price, value, dummy;
    if ((cur_store_num != STORE_HOME) && (cur_store_num != STORE_MUSEUM)) {
        char out_val[80];
        msg_format(_("%s(%c)を売却する。", "Selling %s (%c)."), o_name, index_to_label(item));
        msg_print(NULL);

        price = price_item(owner_ptr, o_ptr, ot_ptr->min_inflate, TRUE);
        choice = 0;

        if (st_ptr->store_open >= current_world_ptr->game_turn)
            return;

        sprintf(out_val, _("売却価格: $%d [Enter/Escape]", "Selling price: $%d [Enter/Escape]"), price);
        put_str(out_val, 0, 0);
        while (TRUE) {
            char k = inkey();
            if (k == '\r') {
                break;
            }
            if (k == ESCAPE) {
                put_str("                                                     ", 0, 0);
                return;
            }
        }

        if (choice == 0) {
            say_comment_1(owner_ptr);
            sound(SOUND_SELL);
            if (cur_store_num == STORE_BLACK)
                chg_virtue(owner_ptr, V_JUSTICE, -1);

            if ((o_ptr->tval == TV_BOTTLE) && (cur_store_num != STORE_HOME))
                chg_virtue(owner_ptr, V_NATURE, 1);
            decrease_insults();

            owner_ptr->au += price;
            store_prt_gold(owner_ptr);
            dummy = object_value(owner_ptr, q_ptr) * q_ptr->number;

            identify_item(owner_ptr, o_ptr);
            q_ptr = &forge;
            object_copy(q_ptr, o_ptr);
            q_ptr->number = amt;
            q_ptr->ident |= IDENT_STORE;
            if ((o_ptr->tval == TV_ROD) || (o_ptr->tval == TV_WAND))
                q_ptr->pval = o_ptr->pval * amt / o_ptr->number;

            value = object_value(owner_ptr, q_ptr) * q_ptr->number;
            describe_flavor(owner_ptr, o_name, q_ptr, 0);
            msg_format(_("%sを $%ldで売却しました。", "You sold %s for %ld gold."), o_name, (long)price);

            if (record_sell)
                exe_write_diary(owner_ptr, DIARY_SELL, 0, o_name);

            if (!((o_ptr->tval == TV_FIGURINE) && (value > 0)))
                purchase_analyze(owner_ptr, price, value, dummy);

            distribute_charges(o_ptr, q_ptr, amt);
            q_ptr->timeout = 0;
            inven_item_increase(owner_ptr, item, -amt);
            inven_item_describe(owner_ptr, item);
            if (o_ptr->number > 0)
                autopick_alter_item(owner_ptr, item, FALSE);

            inven_item_optimize(owner_ptr, item);
            int item_pos = store_carry(owner_ptr, q_ptr);
            if (item_pos >= 0) {
                store_top = (item_pos / store_bottom) * store_bottom;
                display_store_inventory(owner_ptr);
            }
        }
    } else if (cur_store_num == STORE_MUSEUM) {
        char o2_name[MAX_NLEN];
        describe_flavor(owner_ptr, o2_name, q_ptr, OD_NAME_ONLY);

        if (-1 == store_check_num(q_ptr))
            msg_print(_("それと同じ品物は既に博物館にあるようです。", "The Museum already has one of those items."));
        else
            msg_print(_("博物館に寄贈したものは取り出すことができません！！", "You cannot take back items which have been donated to the Museum!!"));
        
        if (!get_check(format(_("本当に%sを寄贈しますか？", "Really give %s to the Museum? "), o2_name)))
            return;

        identify_item(owner_ptr, q_ptr);
        q_ptr->ident |= IDENT_FULL_KNOWN;

        distribute_charges(o_ptr, q_ptr, amt);
        msg_format(_("%sを置いた。(%c)", "You drop %s (%c)."), o_name, index_to_label(item));
        choice = 0;

        vary_item(owner_ptr, item, -amt);

        int item_pos = home_carry(owner_ptr, q_ptr);
        if (item_pos >= 0) {
            store_top = (item_pos / store_bottom) * store_bottom;
            display_store_inventory(owner_ptr);
        }
    } else {
        distribute_charges(o_ptr, q_ptr, amt);
        msg_format(_("%sを置いた。(%c)", "You drop %s (%c)."), o_name, index_to_label(item));
        choice = 0;
        vary_item(owner_ptr, item, -amt);
        int item_pos = home_carry(owner_ptr, q_ptr);
        if (item_pos >= 0) {
            store_top = (item_pos / store_bottom) * store_bottom;
            display_store_inventory(owner_ptr);
        }
    }

    set_bits(owner_ptr->update, PU_BONUS);
    set_bits(owner_ptr->window_flags, PW_PLAYER);
    handle_stuff(owner_ptr);

    if ((choice == 0) && (item >= INVEN_MAIN_HAND)) {
        calc_android_exp(owner_ptr);
        verify_equip_slot(owner_ptr, item);
    }
}
