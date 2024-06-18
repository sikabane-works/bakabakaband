/*!
 * @brief ベースアイテム情報の構造体 / Information about object "kinds", including player knowledge.
 * @date 2019/05/01
 * @author deskull
 * @details
 * ゲーム進行用のセーブファイル上では aware と tried のみ保存対象とすること。と英文ではあるが実際はもっとある様子である。 /
 * Only "aware" and "tried" are saved in the savefile
 */

#include "system/baseitem-info.h"
#include "object/tval-types.h"
#include "sv-definition/sv-armor-types.h"
#include "sv-definition/sv-bow-types.h"
#include "sv-definition/sv-food-types.h"
#include "sv-definition/sv-lite-types.h"
#include "sv-definition/sv-other-types.h"
#include "sv-definition/sv-protector-types.h"
#include "sv-definition/sv-rod-types.h"
#include "sv-definition/sv-weapon-types.h"
#include "system/angband-exceptions.h"
#include <set>
#include <unordered_map>

namespace {
constexpr auto ITEM_NOT_BOW = "This item is not a bow!";
constexpr auto ITEM_NOT_ROD = "This item is not a rod!";
constexpr auto ITEM_NOT_LITE = "This item is not a lite!";
constexpr auto INVALID_BI_ID_FORMAT = "Invalid Baseitem ID is specified! %d";
}

bool BaseitemKey::operator==(const BaseitemKey &other) const
{
    return (this->type_value == other.type_value) && (this->subtype_value == other.subtype_value);
}

// @details type_valueに大小があればそれを判定し、同一ならばsubtype_valueの大小を判定する.
bool BaseitemKey::operator<(const BaseitemKey &other) const
{
    if (this->type_value < other.type_value) {
        return true;
    }

    if (this->type_value > other.type_value) {
        return false;
    }

    return this->subtype_value < other.subtype_value;
}

ItemKindType BaseitemKey::tval() const
{
    return this->type_value;
}

std::optional<int> BaseitemKey::sval() const
{
    return this->subtype_value;
}

bool BaseitemKey::is(ItemKindType tval) const
{
    return this->type_value == tval;
}

/*!
 * @brief 射撃武器に対応する矢/弾薬のベースアイテムIDを返す
 * @return 対応する矢/弾薬のベースアイテムID
 */
ItemKindType BaseitemKey::get_arrow_kind() const
{
    if ((this->type_value != ItemKindType::BOW) || !this->subtype_value.has_value()) {
        THROW_EXCEPTION(std::logic_error, ITEM_NOT_BOW);
    }

    switch (this->subtype_value.value()) {
    case SV_SLING:
        return ItemKindType::SHOT;
    case SV_SHORT_BOW:
    case SV_LONG_BOW:
    case SV_NAMAKE_BOW:
        return ItemKindType::ARROW;
    case SV_LIGHT_XBOW:
    case SV_HEAVY_XBOW:
        return ItemKindType::BOLT;
    case SV_CRIMSON:
    case SV_HARP:
        return ItemKindType::NO_AMMO;
    default:
        return ItemKindType::NONE;
    }
}

bool BaseitemKey::is_spell_book() const
{
    switch (this->type_value) {
    case ItemKindType::LIFE_BOOK:
    case ItemKindType::SORCERY_BOOK:
    case ItemKindType::NATURE_BOOK:
    case ItemKindType::CHAOS_BOOK:
    case ItemKindType::DEATH_BOOK:
    case ItemKindType::TRUMP_BOOK:
    case ItemKindType::ARCANE_BOOK:
    case ItemKindType::CRAFT_BOOK:
    case ItemKindType::DEMON_BOOK:
    case ItemKindType::CRUSADE_BOOK:
    case ItemKindType::MUSIC_BOOK:
    case ItemKindType::HISSATSU_BOOK:
    case ItemKindType::HEX_BOOK:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_high_level_book() const
{
    if (!this->is_spell_book()) {
        return false;
    }

    if (this->type_value == ItemKindType::ARCANE_BOOK) {
        return false;
    }

    return this->subtype_value >= 2;
}

bool BaseitemKey::is_melee_weapon() const
{
    switch (this->type_value) {
    case ItemKindType::POLEARM:
    case ItemKindType::SWORD:
    case ItemKindType::DIGGING:
    case ItemKindType::HAFTED:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_ammo() const
{
    switch (this->type_value) {
    case ItemKindType::SHOT:
    case ItemKindType::ARROW:
    case ItemKindType::BOLT:
        return true;
    default:
        return false;
    }
}

/*
 * @brief 未鑑定名を持つか否かの判定
 * @details FOODはキノコが該当する
 */
bool BaseitemKey::has_unidentified_name() const
{
    switch (this->type_value) {
    case ItemKindType::AMULET:
    case ItemKindType::RING:
    case ItemKindType::STAFF:
    case ItemKindType::WAND:
    case ItemKindType::ROD:
    case ItemKindType::SCROLL:
    case ItemKindType::POTION:
        return true;
    case ItemKindType::FOOD:
        return this->is_mushrooms();
    default:
        return false;
    }
}

bool BaseitemKey::can_recharge() const
{
    switch (this->type_value) {
    case ItemKindType::STAFF:
    case ItemKindType::WAND:
    case ItemKindType::ROD:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_wand_rod() const
{
    switch (this->type_value) {
    case ItemKindType::WAND:
    case ItemKindType::ROD:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_wand_staff() const
{
    switch (this->type_value) {
    case ItemKindType::WAND:
    case ItemKindType::STAFF:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_protector() const
{
    switch (this->type_value) {
    case ItemKindType::BOOTS:
    case ItemKindType::GLOVES:
    case ItemKindType::HELM:
    case ItemKindType::CROWN:
    case ItemKindType::SHIELD:
    case ItemKindType::CLOAK:
    case ItemKindType::SOFT_ARMOR:
    case ItemKindType::HARD_ARMOR:
    case ItemKindType::DRAG_ARMOR:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::can_be_aura_protector() const
{
    switch (this->type_value) {
    case ItemKindType::CLOAK:
    case ItemKindType::SOFT_ARMOR:
    case ItemKindType::HARD_ARMOR:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_wearable() const
{
    switch (this->type_value) {
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
    case ItemKindType::CARD:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_weapon() const
{
    switch (this->type_value) {
    case ItemKindType::BOW:
    case ItemKindType::DIGGING:
    case ItemKindType::HAFTED:
    case ItemKindType::POLEARM:
    case ItemKindType::SWORD:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_equipement() const
{
    return this->is_wearable() || this->is_ammo();
}

bool BaseitemKey::is_melee_ammo() const
{
    switch (this->type_value) {
    case ItemKindType::HAFTED:
    case ItemKindType::POLEARM:
    case ItemKindType::DIGGING:
    case ItemKindType::BOLT:
    case ItemKindType::ARROW:
    case ItemKindType::SHOT:
        return true;
    case ItemKindType::SWORD:
        return this->subtype_value != SV_POISON_NEEDLE;
    default:
        return false;
    }
}

bool BaseitemKey::is_orthodox_melee_weapon() const
{
    switch (this->type_value) {
    case ItemKindType::HAFTED:
    case ItemKindType::POLEARM:
    case ItemKindType::DIGGING:
        return true;
    case ItemKindType::SWORD:
        return this->subtype_value != SV_POISON_NEEDLE;
    default:
        return false;
    }
}

bool BaseitemKey::is_broken_weapon() const
{
    if (this->type_value != ItemKindType::SWORD) {
        return false;
    }

    if (!this->subtype_value.has_value()) {
        return false;
    }

    switch (this->subtype_value.value()) {
    case SV_BROKEN_DAGGER:
    case SV_BROKEN_SWORD:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_throwable() const
{
    switch (this->type_value) {
    case ItemKindType::DIGGING:
    case ItemKindType::HAFTED:
    case ItemKindType::POLEARM:
    case ItemKindType::SWORD:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_wieldable_in_etheir_hand() const
{
    switch (this->type_value) {
    case ItemKindType::DIGGING:
    case ItemKindType::HAFTED:
    case ItemKindType::POLEARM:
    case ItemKindType::SWORD:
    case ItemKindType::SHIELD:
    case ItemKindType::CAPTURE:
    case ItemKindType::CARD:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_rare() const
{
    static const std::unordered_map<ItemKindType, const std::set<int>> rare_table = {
        { ItemKindType::HAFTED, { SV_MACE_OF_DISRUPTION, SV_WIZSTAFF } },
        { ItemKindType::POLEARM, { SV_SCYTHE_OF_SLICING, SV_DEATH_SCYTHE } },
        { ItemKindType::SWORD, { SV_BLADE_OF_CHAOS, SV_DIAMOND_EDGE, SV_POISON_NEEDLE, SV_HAYABUSA } },
        { ItemKindType::SHIELD, { SV_DRAGON_SHIELD, SV_MIRROR_SHIELD } },
        { ItemKindType::HELM, { SV_DRAGON_HELM } },
        { ItemKindType::BOOTS, { SV_PAIR_OF_DRAGON_GREAVE } },
        { ItemKindType::CLOAK, { SV_ELVEN_CLOAK, SV_ETHEREAL_CLOAK, SV_SHADOW_CLOAK, SV_MAGIC_RESISTANCE_CLOAK } },
        { ItemKindType::GLOVES, { SV_SET_OF_DRAGON_GLOVES } },
        { ItemKindType::SOFT_ARMOR, { SV_KUROSHOUZOKU, SV_ABUNAI_MIZUGI } },
        { ItemKindType::HARD_ARMOR, { SV_MITHRIL_CHAIN_MAIL, SV_MITHRIL_PLATE_MAIL, SV_ADAMANTITE_PLATE_MAIL } },
        { ItemKindType::DRAG_ARMOR, { /* Any */ } },
    };

    if (!this->subtype_value.has_value()) {
        return false;
    }

    if (auto it = rare_table.find(this->type_value); it != rare_table.end()) {
        const auto &svals = it->second;
        return svals.empty() || (svals.find(this->subtype_value.value()) != svals.end());
    }

    return false;
}

short BaseitemKey::get_bow_energy() const
{
    if ((this->type_value != ItemKindType::BOW) || !this->subtype_value.has_value()) {
        THROW_EXCEPTION(std::logic_error, ITEM_NOT_BOW);
    }

    switch (this->subtype_value.value()) {
    case SV_SLING:
        return 8000;
    case SV_NAMAKE_BOW:
        return 7777;
    case SV_LIGHT_XBOW:
        return 12000;
    case SV_HEAVY_XBOW:
        return 13333;
    default:
        return 10000;
    }
}

int BaseitemKey::get_arrow_magnification() const
{
    if ((this->type_value != ItemKindType::BOW) || !this->subtype_value.has_value()) {
        THROW_EXCEPTION(std::logic_error, ITEM_NOT_BOW);
    }

    switch (this->subtype_value.value()) {
    case SV_SLING:
    case SV_SHORT_BOW:
        return 2;
    case SV_LONG_BOW:
    case SV_NAMAKE_BOW:
    case SV_LIGHT_XBOW:
        return 3;
    case SV_HEAVY_XBOW:
        return 4;
    default:
        return 0;
    }
}

bool BaseitemKey::is_aiming_rod() const
{
    if ((this->type_value != ItemKindType::ROD) || !this->subtype_value.has_value()) {
        THROW_EXCEPTION(std::logic_error, ITEM_NOT_ROD);
    }

    switch (this->subtype_value.value()) {
    case SV_ROD_TELEPORT_AWAY:
    case SV_ROD_DISARMING:
    case SV_ROD_LITE:
    case SV_ROD_SLEEP_MONSTER:
    case SV_ROD_SLOW_MONSTER:
    case SV_ROD_HYPODYNAMIA:
    case SV_ROD_POLYMORPH:
    case SV_ROD_ACID_BOLT:
    case SV_ROD_ELEC_BOLT:
    case SV_ROD_FIRE_BOLT:
    case SV_ROD_COLD_BOLT:
    case SV_ROD_ACID_BALL:
    case SV_ROD_ELEC_BALL:
    case SV_ROD_FIRE_BALL:
    case SV_ROD_COLD_BALL:
    case SV_ROD_STONE_TO_MUD:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_lite_requiring_fuel() const
{
    if ((this->type_value != ItemKindType::LITE) || !this->subtype_value.has_value()) {
        THROW_EXCEPTION(std::logic_error, ITEM_NOT_LITE);
    }

    switch (this->subtype_value.value()) {
    case SV_LITE_TORCH:
    case SV_LITE_LANTERN:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_junk() const
{
    switch (this->type_value) {
    case ItemKindType::SKELETON:
    case ItemKindType::BOTTLE:
    case ItemKindType::JUNK:
    case ItemKindType::STATUE:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_armour() const
{
    switch (this->type_value) {
    case ItemKindType::SOFT_ARMOR:
    case ItemKindType::HARD_ARMOR:
    case ItemKindType::DRAG_ARMOR:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::is_cross_bow() const
{
    if ((this->type_value != ItemKindType::BOW) || !this->subtype_value.has_value()) {
        return false;
    }

    switch (this->subtype_value.value()) {
    case SV_LIGHT_XBOW:
    case SV_HEAVY_XBOW:
        return true;
    default:
        return false;
    }
}

bool BaseitemKey::should_refuse_enchant() const
{
    return *this == BaseitemKey(ItemKindType::SWORD, SV_POISON_NEEDLE);
}

/*!
 * @brief ベースアイテムが発動効果を持つ時、その記述を生成する
 * @return 発動効果
 */
std::string BaseitemKey::explain_activation() const
{
    switch (this->type_value) {
    case ItemKindType::WHISTLE:
        return _("ペット呼び寄せ : 100+d100ターン毎", "call pet every 100+d100 turns");
    case ItemKindType::CAPTURE:
        return _("モンスターを捕える、又は解放する。", "captures or releases a monster.");
    default:
        return _("何も起きない", "Nothing");
    }
}

bool BaseitemKey::is_convertible() const
{
    auto is_convertible = this->is(ItemKindType::JUNK) || this->is(ItemKindType::SKELETON);
    is_convertible |= *this == BaseitemKey(ItemKindType::CORPSE, SV_SKELETON);
    return is_convertible;
}

bool BaseitemKey::is_fuel() const
{
    auto is_fuel = *this == BaseitemKey(ItemKindType::LITE, SV_LITE_TORCH);
    is_fuel |= *this == BaseitemKey(ItemKindType::LITE, SV_LITE_LANTERN);
    is_fuel |= *this == BaseitemKey(ItemKindType::FLASK, SV_FLASK_OIL);
    return is_fuel;
}

bool BaseitemKey::is_lance() const
{
    auto is_lance = *this == BaseitemKey(ItemKindType::POLEARM, SV_LANCE);
    is_lance |= *this == BaseitemKey(ItemKindType::POLEARM, SV_HEAVY_LANCE);
    return is_lance;
}

bool BaseitemKey::is_readable() const
{
    auto can_read = this->is(ItemKindType::SCROLL);
    can_read |= this->is(ItemKindType::READING_MATTER);
    return can_read;
}

bool BaseitemKey::is_corpse() const
{
    return *this == BaseitemKey(ItemKindType::CORPSE, SV_CORPSE);
}

bool BaseitemKey::is_mushrooms() const
{
    if (!this->subtype_value.has_value()) {
        return false;
    }

    switch (this->subtype_value.value()) {
    case SV_FOOD_POISON:
    case SV_FOOD_BLINDNESS:
    case SV_FOOD_PARANOIA:
    case SV_FOOD_CONFUSION:
    case SV_FOOD_HALLUCINATION:
    case SV_FOOD_PARALYSIS:
    case SV_FOOD_WEAKNESS:
    case SV_FOOD_SICKNESS:
    case SV_FOOD_STUPIDITY:
    case SV_FOOD_NAIVETY:
    case SV_FOOD_UNHEALTH:
    case SV_FOOD_DISEASE:
    case SV_FOOD_CURE_POISON:
    case SV_FOOD_CURE_BLINDNESS:
    case SV_FOOD_CURE_PARANOIA:
    case SV_FOOD_CURE_CONFUSION:
    case SV_FOOD_CURE_SERIOUS:
    case SV_FOOD_RESTORE_STR:
    case SV_FOOD_RESTORE_CON:
    case SV_FOOD_RESTORING:
        return true;
    default:
        return false;
    }
}

BaseitemInfo::BaseitemInfo()
    : bi_key(ItemKindType::NONE)
{
}

/*!
 * @brief 正常なベースアイテムかを判定する
 * @return 正常なベースアイテムか否か
 * @details ID 0は「何か」という異常アイテム
 * その他、ベースアイテムIDは歴史的事情により歯抜けが多数あり、それらは名前が空欄になるようにオブジェクトを生成している
 * @todo v3.1以降で歯抜けを埋めるようにベースアイテムを追加していきたい (詳細未定)
 */
bool BaseitemInfo::is_valid() const
{
    return (this->idx > 0) && !this->name.empty();
}

/*!
 * @brief オブジェクトを試行済にする
 */
void BaseitemInfo::mark_as_tried()
{
    this->tried = true;
}

/*!
 * @brief 最初から簡易な名称が明らかなベースアイテムにその旨のフラグを立てる
 */
void BaseitemInfo::decide_easy_know()
{
    switch (this->bi_key.tval()) {
    case ItemKindType::LIFE_BOOK:
    case ItemKindType::SORCERY_BOOK:
    case ItemKindType::NATURE_BOOK:
    case ItemKindType::CHAOS_BOOK:
    case ItemKindType::DEATH_BOOK:
    case ItemKindType::TRUMP_BOOK:
    case ItemKindType::ARCANE_BOOK:
    case ItemKindType::CRAFT_BOOK:
    case ItemKindType::DEMON_BOOK:
    case ItemKindType::CRUSADE_BOOK:
    case ItemKindType::MUSIC_BOOK:
    case ItemKindType::HISSATSU_BOOK:
    case ItemKindType::HEX_BOOK:
    case ItemKindType::FLASK:
    case ItemKindType::JUNK:
    case ItemKindType::BOTTLE:
    case ItemKindType::SKELETON:
    case ItemKindType::SPIKE:
    case ItemKindType::WHISTLE:
    case ItemKindType::FOOD:
    case ItemKindType::POTION:
    case ItemKindType::SCROLL:
    case ItemKindType::ROD:
    case ItemKindType::STATUE:
    case ItemKindType::READING_MATTER:
        this->easy_know = true;
        return;
    default:
        this->easy_know = false;
        return;
    }
}

/*!
 * @brief オブジェクトを試行済にする
 */
void BaseitemInfo::mark_as_tried()
{
    this->tried = true;
}

void BaseitemInfo::mark_as_aware()
{
    this->aware = true;
}

std::vector<BaseitemInfo> baseitems_info;

BaseitemList BaseitemList::instance{};

BaseitemList &BaseitemList::get_instance()
{
    return instance;
}

BaseitemInfo &BaseitemList::get_baseitem(const short bi_id)
{
    if ((bi_id < 0) || (bi_id >= static_cast<short>(this->baseitems.size()))) {
        THROW_EXCEPTION(std::logic_error, format(INVALID_BI_ID_FORMAT, bi_id));
    }

    return this->baseitems[bi_id];
}

const BaseitemInfo &BaseitemList::get_baseitem(const short bi_id) const
{
    if ((bi_id < 0) || (bi_id >= static_cast<short>(this->baseitems.size()))) {
        THROW_EXCEPTION(std::logic_error, format(INVALID_BI_ID_FORMAT, bi_id));
    }

    return this->baseitems[bi_id];
}

std::vector<BaseitemInfo> &BaseitemList::get_raw_vector()
{
    return this->baseitems;
}

std::vector<BaseitemInfo>::iterator BaseitemList::begin()
{
    return this->baseitems.begin();
}

std::vector<BaseitemInfo>::const_iterator BaseitemList::begin() const
{
    return this->baseitems.begin();
}

std::vector<BaseitemInfo>::iterator BaseitemList::end()
{
    return this->baseitems.end();
}

std::vector<BaseitemInfo>::const_iterator BaseitemList::end() const
{
    return this->baseitems.end();
}

std::vector<BaseitemInfo>::reverse_iterator BaseitemList::rbegin()
{
    return this->baseitems.rbegin();
}

std::vector<BaseitemInfo>::const_reverse_iterator BaseitemList::rbegin() const
{
    return this->baseitems.rbegin();
}

std::vector<BaseitemInfo>::reverse_iterator BaseitemList::rend()
{
    return this->baseitems.rend();
}

std::vector<BaseitemInfo>::const_reverse_iterator BaseitemList::rend() const
{
    return this->baseitems.rend();
}

size_t BaseitemList::size() const
{
    return this->baseitems.size();
}

bool BaseitemList::empty() const
{
    return this->baseitems.empty();
}

void BaseitemList::resize(size_t new_size)
{
    this->baseitems.resize(new_size);
}
