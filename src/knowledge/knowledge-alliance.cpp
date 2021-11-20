/*!
 * @brief これまでの行いを表示する
 * @date 2021/06/21
 * @author Deskull
 */

#include "core/show-file.h"
#include "io-dump/dump-util.h"
#include "system/player-type-definition.h"
#include "util/angband-files.h"
#include "alliance/alliance.h"
#include "system/monster-race-definition.h"
#include "monster-race/monster-race.h"
#include <string>

void do_cmd_knowledge_alliance(PlayerType *player_ptr)
{
    FILE *fff = NULL;
    GAME_TEXT file_name[FILE_NAME_SIZE];
    if (!open_temporary_file(&fff, file_name))
        return;
    
    for(auto a : alliance_list) {
        if(a.second->id == AllianceType::NONE) continue;
        fprintf(fff, _("%-30s/ あなたへの印象値: %+5d \n", "%-30s/ Implession to you: %+5d \n"), a.second->name.c_str(), a.second->calcImplessionPoint(player_ptr));
        fprintf(fff, _("  勢力指数: %12ld \n", "  Power Value: %12ld \n"), a.second->calcCurrentPower());
        fprintf(fff, _("  (%s) \n", "  (%s) \n"), (a.second->isAnnihilated() ? _("壊滅", "Annihilated") : _("健在", "Alive")));
        fprintf(fff, _("所属戦力--\n", "Affiliation strength--\n"));
        for (auto r : r_info) {
            if (r.alliance_idx == a.second->id) {
                fprintf(fff, _("  %-40s レベル %3d 評価値 %7d\n", "  %-40s LEVEL %3d POW %7d\n"), r.name.c_str(), r.level, calc_monrace_eval(&r));
            }
        }
        fprintf(fff, "\n\n");
    }

    angband_fclose(fff);
    (void)show_file(player_ptr, true, file_name, _("各アライアンス情報", "Information of all alliances"), 0, 0);
    fd_kill(file_name);

}
