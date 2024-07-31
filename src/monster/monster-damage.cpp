/*
 * @brief モンスターがダメージを受けた時の処理と経験値の加算処理
 * @date 2021/08/04
 * @author Hourier
 */

#include "monster/monster-damage.h"
#include "avatar/avatar-changer.h"
#include "core/speed-table.h"
#include "core/stuff-handler.h"
#include "game-option/birth-options.h"
#include "game-option/play-record-options.h"
#include "io/files-util.h"
#include "io/report.h"
#include "io/write-diary.h"
#include "main/sound-definitions-table.h"
#include "main/sound-of-music.h"
#include "mind/mind-ninja.h"
#include "monster-floor/monster-death.h"
#include "monster-floor/monster-remover.h"
#include "monster-floor/monster-summon.h"
#include "monster-floor/place-monster-types.h"
#include "monster-race/monster-race-hook.h"
#include "monster/monster-describer.h"
#include "monster/monster-description-types.h"
#include "monster/monster-info.h"
#include "monster/monster-status-setter.h"
#include "monster/monster-status.h"
#include "object-enchant/object-curse.h"
#include "player/player-status.h"
#include "player/special-defense-types.h"
#include "spell-kind/spells-random.h"
#include "status/experience.h"
#include "system/angband-system.h"
#include "system/floor-type-definition.h"
#include "system/monster-entity.h"
#include "system/monster-race-info.h"
#include "system/player-type-definition.h"
#include "system/redrawing-flags-updater.h"
#include "timed-effect/timed-effects.h"
#include "tracking/health-bar-tracker.h"
#include "tracking/lore-tracker.h"
#include "util/bit-flags-calculator.h"
#include "view/display-messages.h"
#include "world/world.h"
#include <optional>
#include <sstream>
#include <string>

/*
 * @brief コンストラクタ
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param m_idx ダメージを与えたモンスターのID
 * @param dam 与えたダメージ量
 * @param fear ダメージによってモンスターが恐慌状態に陥ったならばtrue
 * @param attribute 与えたダメージの種類 (単一属性)
 * @param note モンスターが倒された際の特別なメッセージ述語
 */
MonsterDamageProcessor::MonsterDamageProcessor(PlayerType *player_ptr, MONSTER_IDX m_idx, int dam, bool *fear, AttributeType attribute)
    : player_ptr(player_ptr)
    , m_idx(m_idx)
    , dam(dam)
    , fear(fear)
{
    this->attribute_flags.clear();
    this->attribute_flags.set((AttributeType)attribute);
}

/*
 * @brief コンストラクタ
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param m_idx ダメージを与えたモンスターのID
 * @param dam 与えたダメージ量
 * @param fear ダメージによってモンスターが恐慌状態に陥ったならばtrue
 * @param attribute_flags 与えたダメージの種類 (複数属性)
 * @param note モンスターが倒された際の特別なメッセージ述語
 */
MonsterDamageProcessor::MonsterDamageProcessor(PlayerType *player_ptr, MONSTER_IDX m_idx, int dam, bool *fear, AttributeFlags attribute_flags)
    : player_ptr(player_ptr)
    , m_idx(m_idx)
    , dam(dam)
    , fear(fear)
    , attribute_flags(attribute_flags)
{
}

/*!
 * @brief モンスターのHPをダメージに応じて減算する
 * @return モンスターが生きていればfalse、死んだらtrue
 * @details exp_mon をコピーしているのは、撃破時の経験値を算出する頃にはmonsterが無効モンスターになっているため.
 */
bool MonsterDamageProcessor::mon_take_hit(std::string_view note)
{
    auto &monster = this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    const MonsterEntity exp_mon = monster;
    auto exp_dam = (monster.hp > this->dam) ? this->dam : monster.hp;
    this->get_exp_from_mon(exp_mon, exp_dam);
    if (this->genocide_chaos_patron()) {
        return true;
    }

    monster.hp -= this->dam;
    monster.dealt_damage += this->dam;
    if (monster.dealt_damage > monster.max_maxhp * 100) {
        monster.dealt_damage = monster.max_maxhp * 100;
    }

    if (AngbandWorld::get_instance().wizard) {
        msg_format(_("合計%d/%dのダメージを与えた。", "You do %d (out of %d) damage."), monster.dealt_damage, monster.maxhp);
    }

    if (this->process_dead_exp_virtue(note, exp_mon)) {
        return true;
    }

    this->add_monster_fear();
    return false;
}

bool MonsterDamageProcessor::genocide_chaos_patron()
{
    auto *m_ptr = &this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    if (!m_ptr->is_valid()) {
        this->m_idx = 0;
    }

    this->set_redraw();
    (void)set_monster_csleep(this->player_ptr, this->m_idx, 0);
    set_superstealth(this->player_ptr, false);

    return this->m_idx == 0;
}

bool MonsterDamageProcessor::process_dead_exp_virtue(std::string_view note, const MonsterEntity &exp_mon)
{
    auto &monster = this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    auto &monrace = monster.get_real_monrace();
    if (monster.hp >= 0) {
        return false;
    }

    this->death_special_flag_monster();
    if (monrace.r_akills < MAX_SHORT) {
        monrace.r_akills++;
    }

    this->increase_kill_numbers();
    const auto m_name = monster_desc(this->player_ptr, &monster, MD_TRUE_NAME);
    this->death_amberites(m_name);
    this->dying_scream(m_name);
    AvatarChanger ac(this->player_ptr, &monster);
    ac.change_virtue();
    if (monrace.kind_flags.has(MonsterKindType::UNIQUE) && record_destroy_uniq) {
        std::stringstream ss;
        ss << monrace.name << (monster.mflag2.has(MonsterConstantFlagType::CLONED) ? _("(クローン)", "(Clone)") : "");
        exe_write_diary(*this->player_ptr->current_floor_ptr, DiaryKind::UNIQUE, 0, ss.str());
    }

    sound(SOUND_KILL);
    this->show_kill_message(note, m_name);
    this->show_bounty_message(m_name);
    monster_death(this->player_ptr, this->m_idx, true, this->attribute_flags);
    this->summon_special_unique();
    this->get_exp_from_mon(exp_mon, exp_mon.max_maxhp * 2);
    *this->fear = false;
    return true;
}

/*
 * @brief たぬき、カメレオン、ナズグル、ユニークの死亡時処理
 * @param m_ptr ダメージを与えたモンスターの構造体参照ポインタ
 */
void MonsterDamageProcessor::death_special_flag_monster()
{
    auto *m_ptr = &this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    auto r_idx = m_ptr->r_idx;
    auto *r_ptr = &monraces_info[r_idx];
    if (monraces_info[r_idx].misc_flags.has(MonsterMiscType::TANUKI)) {
        r_ptr = &monraces_info[r_idx];
        m_ptr->ap_r_idx = r_idx;
        if (r_ptr->r_sights < MAX_SHORT) {
            r_ptr->r_sights++;
        }
    }

    if (m_ptr->mflag2.has(MonsterConstantFlagType::CHAMELEON)) {
        auto &real_r_ref = m_ptr->get_real_monrace();
        r_idx = m_ptr->get_real_monrace_id();
        if (real_r_ref.r_sights < MAX_SHORT) {
            real_r_ref.r_sights++;
        }
    }

    if (m_ptr->mflag2.has(MonsterConstantFlagType::CLONED)) {
        return;
    }

    if (r_ptr->population_flags.has(MonsterPopulationType::NAZGUL)) {
        r_ptr->mob_num--;
        return;
    }

    if (r_ptr->kind_flags.has_not(MonsterKindType::UNIQUE)) {
        return;
    }

    this->death_unique_monster(r_idx);
}

/*
 * @brief ユニークの死亡処理
 * @param r_idx 死亡したユニークの種族番号
 */
void MonsterDamageProcessor::death_unique_monster(MonsterRaceId r_idx)
{
    monraces_info[r_idx].max_num = 0;
    auto &monraces = MonraceList::get_instance();
    if (monraces.can_unify_separate(r_idx)) {
        monraces.kill_unified_unique(r_idx);
    }
}

void MonsterDamageProcessor::increase_kill_numbers()
{
    auto &monster = this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    auto &monrace = monster.get_real_monrace();
    auto is_hallucinated = this->player_ptr->effects()->hallucination().is_hallucinated();
    if (((monster.ml == 0) || is_hallucinated) && monrace.kind_flags.has_not(MonsterKindType::UNIQUE)) {
        return;
    }

    if (monster.mflag2.has(MonsterConstantFlagType::KAGE) && (monraces_info[MonsterRaceId::KAGE].r_pkills < MAX_SHORT)) {
        monraces_info[MonsterRaceId::KAGE].r_pkills++;
    } else if (monrace.r_pkills < MAX_SHORT) {
        monrace.r_pkills++;
    }

    if (monster.mflag2.has(MonsterConstantFlagType::KAGE) && (monraces_info[MonsterRaceId::KAGE].r_tkills < MAX_SHORT)) {
        monraces_info[MonsterRaceId::KAGE].r_tkills++;
    } else if (monrace.r_tkills < MAX_SHORT) {
        monrace.r_tkills++;
    }

    LoreTracker::get_instance().set_trackee(monster.ap_r_idx);
}

void MonsterDamageProcessor::death_amberites(std::string_view m_name)
{
    auto *m_ptr = &this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    const auto &r_ref = m_ptr->get_real_monrace();
    if (r_ref.kind_flags.has_not(MonsterKindType::AMBERITE) || one_in_(2)) {
        return;
    }

    auto curses = 1 + randint1(3);
    auto stop_ty = false;
    auto count = 0;
    msg_format(_("%s^は恐ろしい血の呪いをあなたにかけた！", "%s^ puts a terrible blood curse on you!"), m_name.data());
    curse_equipment(this->player_ptr, 100, 50);
    do {
        stop_ty = activate_ty_curse(this->player_ptr, stop_ty, &count);
    } while (--curses);
}

void MonsterDamageProcessor::dying_scream(std::string_view m_name)
{
    auto *m_ptr = &this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    const auto &r_ref = m_ptr->get_real_monrace();
    if (r_ref.speak_flags.has_none_of({ MonsterSpeakType::SPEAK_ALL, MonsterSpeakType::SPEAK_DEATH })) {
        return;
    }

    const auto death_mes = get_random_line(_("mondeath_j.txt", "mondeath.txt"), enum2i(m_ptr->r_idx));
    if (death_mes.has_value()) {
        msg_format("%s^ %s", m_name.data(), death_mes->data());
    }

#ifdef WORLD_SCORE
    if (m_ptr->r_idx == MonsterRaceId::SERPENT) {
        screen_dump = make_screen_dump(this->player_ptr);
    }
#endif
}

void MonsterDamageProcessor::show_kill_message(std::string_view note, std::string_view m_name)
{
    auto *floor_ptr = this->player_ptr->current_floor_ptr;
    auto *m_ptr = &floor_ptr->m_list[this->m_idx];
    if (!note.empty()) {
        msg_format("%s^%s", m_name.data(), note.data());
        return;
    }

    if (!m_ptr->ml) {
        auto mes = is_echizen(this->player_ptr) ? _("せっかくだから%sを殺した。", "Because it's time, you have killed %s.")
                                                : _("%sを殺した。", "You have killed %s.");
        msg_format(mes, m_name.data());
        return;
    }

    const auto is_explodable = m_ptr->is_explodable();
    const auto died_mes = m_ptr->get_died_message();
    if (m_ptr->has_living_flag()) {
        if (is_explodable) {
            this->show_explosion_message(died_mes, m_name);
            return;
        }

        auto mes = is_echizen(this->player_ptr) ? _("せっかくだから%sを葬り去った。", "Because it's time, you have slain %s.")
                                                : _("%sを葬り去った。", "You have slain %s.");
        msg_format(mes, m_name.data());
        return;
    }

    if (is_explodable) {
        this->show_explosion_message(died_mes, m_name);
        return;
    }

    auto mes = is_echizen(this->player_ptr) ? _("せっかくだから%sを倒した。", "Because it's time, you have destroyed %s.")
                                            : _("%sを倒した。", "You have destroyed %s.");
    msg_format(mes, m_name.data());
}

void MonsterDamageProcessor::show_explosion_message(std::string_view died_mes, std::string_view m_name)
{
    std::stringstream ss;
    ss << _(m_name, format("%s^", m_name.data()));
    ss << died_mes;
    msg_print(ss.str());
    return;
}

void MonsterDamageProcessor::show_bounty_message(std::string_view m_name)
{
    auto &floor = *this->player_ptr->current_floor_ptr;
    auto &monster = floor.m_list[this->m_idx];
    const auto &monrace = monster.get_real_monrace();
    if (monrace.kind_flags.has_not(MonsterKindType::UNIQUE) || monster.mflag2.has(MonsterConstantFlagType::CLONED) || vanilla_town) {
        return;
    }

    if (monster.mflag2.has(MonsterConstantFlagType::CHAMELEON)) {
        return;
    }

    if (monrace.is_bounty(true)) {
        msg_format(_("%sの首には賞金がかかっている。", "There is a price on %s's head."), m_name.data());
    }
}

/*!
 * @brief モンスターに与えたダメージを元に経験値を加算する /
 * Calculate experience point to be get
 * @param m_ptr ダメージを与えたモンスターの構造体参照ポインタ
 * @details
 * <pre>
 * Even the 64 bit operation is not big enough to avoid overflaw
 * unless we carefully choose orders of ENERGY_MULTIPLICATION and ENERGY_DIVISION.
 * Get the coefficient first, and multiply (potentially huge) base
 * experience point of a monster later.
 * </pre>
 */
void MonsterDamageProcessor::get_exp_from_mon(const MonsterEntity &monster, int exp_dam)
{
    const auto &monrace = monster.get_monrace();
    if (!monster.is_valid() || monster.is_pet() || AngbandSystem::get_instance().is_phase_out()) {
        return;
    }

    /*
     * - Ratio of monster's level to player's level effects
     * - Varying speed effects
     * - Get a fraction in proportion of damage point
     */
    auto new_exp = monrace.level * speed_to_energy(monster.mspeed) * exp_dam;
    auto new_exp_frac = 0U;
    auto div_h = 0;
    auto div_l = (uint)((this->player_ptr->max_plv + 2) * speed_to_energy(monrace.speed));

    /* Use (average maxhp * 2) as a denominator */
    int compensation = monrace.misc_flags.has(MonsterMiscType::FORCE_MAXHP) ? monrace.hit_dice.maxroll() * 2 : monrace.hit_dice.floored_expected_value_multiplied_by(2);
    s64b_mul(&div_h, &div_l, 0, (ironman_nightmare ? 2 : 1) * compensation);

    /* Special penalty in the wilderness */
    if (!this->player_ptr->current_floor_ptr->is_in_underground()) {
        auto is_dungeon_monster = monrace.wilderness_flags.has_not(MonsterWildernessType::WILD_ONLY);
        is_dungeon_monster |= monrace.kind_flags.has_not(MonsterKindType::UNIQUE);
        if (is_dungeon_monster) {
            s64b_mul(&div_h, &div_l, 0, 5);
        }
    }

    /* Do ENERGY_DIVISION first to prevent overflaw */
    s64b_div(&new_exp, &new_exp_frac, div_h, div_l);

    /* Special penalty for mutiply-monster */
    if (monrace.misc_flags.has(MonsterMiscType::MULTIPLY) || (monster.r_idx == MonsterRaceId::DAWN)) {
        int monnum_penarty = monrace.r_akills / 400;
        if (monnum_penarty > 8) {
            monnum_penarty = 8;
        }

        while (monnum_penarty--) {
            /* Divide by 4 */
            s64b_rshift(&new_exp, &new_exp_frac, 2);
        }
    }

    /* Special penalty for rest_and_shoot exp scum */
    if ((monster.dealt_damage > monster.max_maxhp) && (monster.hp >= 0)) {
        int over_damage = monster.dealt_damage / monster.max_maxhp;
        if (over_damage > 32) {
            over_damage = 32;
        }

        while (over_damage--) {
            /* 9/10 for once */
            s64b_mul(&new_exp, &new_exp_frac, 0, 9);
            s64b_div(&new_exp, &new_exp_frac, 0, 10);
        }
    }

    s64b_mul(&new_exp, &new_exp_frac, 0, monrace.mexp);
    gain_exp_64(this->player_ptr, new_exp, new_exp_frac);
}

void MonsterDamageProcessor::set_redraw()
{
    auto &monster = this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    HealthBarTracker::get_instance().set_flag_if_tracking(this->m_idx);
    if (monster.is_riding()) {
        RedrawingFlagsUpdater::get_instance().set_flag(MainWindowRedrawingFlag::UHEALTH);
    }
}

/*
 * @brief 特定ユニークを倒した時に更にユニークを特殊召喚する処理
 * @param m_ptr ダメージを与えた特定ユニークの構造体参照ポインタ
 */
void MonsterDamageProcessor::summon_special_unique()
{
    auto *m_ptr = &this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    bool is_special_summon = m_ptr->r_idx == MonsterRaceId::IKETA;
    is_special_summon |= m_ptr->r_idx == MonsterRaceId::DOPPIO;
    if (!is_special_summon || this->player_ptr->current_floor_ptr->inside_arena || AngbandSystem::get_instance().is_phase_out()) {
        delete_monster_idx(this->player_ptr, this->m_idx);
        return;
    }

    auto dummy_y = m_ptr->fy;
    auto dummy_x = m_ptr->fx;
    auto mode = (BIT_FLAGS)0;
    if (m_ptr->is_pet()) {
        mode |= PM_FORCE_PET;
    }

    MonsterRaceId new_unique_idx;
    concptr mes;
    switch (m_ptr->r_idx) {
    case MonsterRaceId::IKETA:
        new_unique_idx = MonsterRaceId::BIKETAL;
        mes = _("「ハァッハッハッハ！！私がバイケタルだ！！」", "Uwa-hahaha!  *I* am Biketal!");
        break;
    case MonsterRaceId::DOPPIO:
        new_unique_idx = MonsterRaceId::DIAVOLO;
        mes = _("「これは『試練』だ　過去に打ち勝てという『試練』とオレは受けとった」", "This is a 'trial'. I took it as a 'trial' to overcome in the past.");
        break;
    default: // バグでなければ入らない.
        new_unique_idx = MonraceList::empty_id();
        mes = "";
        break;
    }

    delete_monster_idx(this->player_ptr, this->m_idx);
    if (summon_named_creature(this->player_ptr, 0, dummy_y, dummy_x, new_unique_idx, mode)) {
        msg_print(mes);
    }
}

void MonsterDamageProcessor::add_monster_fear()
{
    auto *m_ptr = &this->player_ptr->current_floor_ptr->m_list[this->m_idx];
    if (m_ptr->is_fearful() && (this->dam > 0)) {
        auto fear_remining = m_ptr->get_remaining_fear() - randint1(this->dam);
        if (set_monster_monfear(this->player_ptr, this->m_idx, fear_remining)) {
            *this->fear = false;
        }
    }

    auto *r_ptr = &m_ptr->get_monrace();
    if (m_ptr->is_fearful() || r_ptr->resistance_flags.has(MonsterResistanceType::NO_FEAR)) {
        return;
    }

    int percentage = (100L * m_ptr->hp) / m_ptr->maxhp;
    if ((randint1(10) < percentage) && ((this->dam < m_ptr->hp) || evaluate_percent(20))) {
        return;
    }

    *this->fear = true;
    auto fear_condition = (this->dam >= m_ptr->hp) && (percentage > 7);
    auto fear_value = randint1(10) + (fear_condition ? 20 : (11 - percentage) * 5);
    (void)set_monster_monfear(this->player_ptr, this->m_idx, fear_value);
}
