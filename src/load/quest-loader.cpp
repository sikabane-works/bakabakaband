#include "load/quest-loader.h"
#include "artifact/fixed-art-types.h"
#include "dungeon/quest.h"
#include "floor/floor-town.h"
#include "load/load-util.h"
#include "load/load-zangband.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags1.h"
#include "monster-race/race-flags7.h"
#include "object-enchant/trg-types.h"
#include "system/artifact-type-definition.h"
#include "system/floor-type-definition.h"
#include "system/monster-race-info.h"
#include "system/player-type-definition.h"
#include "util/enum-converter.h"

errr load_town(void)
{
    auto max_towns_load = rd_u16b();
    if (max_towns_load <= towns_info.size()) {
        return 0;
    }

    load_note(format(_("町が多すぎる(%u)！", "Too many (%u) towns!"), max_towns_load));
    return 23;
}

std::tuple<uint16_t, byte> load_quest_info()
{
    auto max_quests_load = rd_u16b();
    byte max_rquests_load;
    max_rquests_load = rd_byte();

    return std::make_tuple(max_quests_load, max_rquests_load);
}

static void load_quest_completion(QuestType *q_ptr)
{
    q_ptr->status = i2enum<QuestStatusType>(rd_s16b());
    q_ptr->level = rd_s16b();
    q_ptr->complev = rd_byte();
    q_ptr->comptime = rd_u32b();
}

static void load_quest_details(PlayerType *player_ptr, QuestType *q_ptr, const QuestId loading_quest_index)
{
    q_ptr->cur_num = rd_s16b();
    q_ptr->max_num = rd_s16b();
    q_ptr->type = i2enum<QuestKindType>(rd_s16b());

    q_ptr->r_idx = i2enum<MonsterRaceId>(rd_s16b());
    if ((q_ptr->type == QuestKindType::RANDOM) && !MonsterRace(q_ptr->r_idx).is_valid()) {
        auto &quest_list = QuestList::get_instance();
        determine_random_questor(player_ptr, &quest_list[loading_quest_index]);
    }
    q_ptr->reward_artifact_idx = i2enum<FixedArtifactId>(rd_s16b());
    if (q_ptr->has_reward()) {
        q_ptr->get_reward().gen_flags.set(ItemGenerationTraitType::QUESTITEM);
    }

    q_ptr->flags = rd_byte();
}

static bool is_loadable_quest(const QuestId q_idx, const byte max_rquests_load)
{
    const auto &quest_list = QuestList::get_instance();
    if (quest_list.find(q_idx) != quest_list.end()) {
        return true;
    }

    auto status = i2enum<QuestStatusType>(rd_s16b());

    strip_bytes(7);

    auto is_quest_running = (status == QuestStatusType::TAKEN);
    is_quest_running |= (status == QuestStatusType::COMPLETED);
    is_quest_running |= ((enum2i(q_idx) >= MIN_RANDOM_QUEST) && (enum2i(q_idx) <= (MIN_RANDOM_QUEST + max_rquests_load)));
    if (!is_quest_running) {
        return false;
    }

    strip_bytes(2);
    strip_bytes(2);
    strip_bytes(2);
    strip_bytes(2);
    strip_bytes(2);
    strip_bytes(1);
    return false;
}

void analyze_quests(PlayerType *player_ptr, const uint16_t max_quests_load, const byte max_rquests_load)
{
    auto &quest_list = QuestList::get_instance();
    for (auto i = 0; i < max_quests_load; i++) {
        QuestId q_idx;
        if (loading_savefile_version_is_older_than(19)) {
            q_idx = i2enum<QuestId>(i);
        } else {
            q_idx = i2enum<QuestId>(rd_s16b());
        }
        if (!is_loadable_quest(q_idx, max_rquests_load)) {
            continue;
        }

        auto *const q_ptr = &quest_list[q_idx];
        load_quest_completion(q_ptr);
        auto is_quest_running = (q_ptr->status == QuestStatusType::TAKEN);
        is_quest_running |= (q_ptr->status == QuestStatusType::COMPLETED);
        is_quest_running |= ((enum2i(q_idx) >= MIN_RANDOM_QUEST) && (enum2i(q_idx) <= (MIN_RANDOM_QUEST + max_rquests_load)));
        if (!is_quest_running) {
            continue;
        }

        load_quest_details(player_ptr, q_ptr, q_idx);
        q_ptr->dungeon = rd_byte();

        if (q_ptr->status == QuestStatusType::TAKEN || q_ptr->status == QuestStatusType::UNTAKEN) {
            if (monraces_info[q_ptr->r_idx].kind_flags.has(MonsterKindType::UNIQUE)) {
                monraces_info[q_ptr->r_idx].flags1 |= RF1_QUESTOR;
            }
        }
    }
}
