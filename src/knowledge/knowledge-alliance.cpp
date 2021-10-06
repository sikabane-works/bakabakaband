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
#include <string>

void do_cmd_knowledge_alliance(player_type *player_ptr)
{
    FILE *fff = NULL;
    GAME_TEXT file_name[FILE_NAME_SIZE];
    if (!open_temporary_file(&fff, file_name))
        return;
    
    for(auto a : alliance_list) {
        if(a.second->id == 0) continue;
        fprintf(fff, "%-30s: %+5d\n", a.second->name.c_str(), a.second->calcImplessionPoint(player_ptr));
    }

    angband_fclose(fff);
    (void)show_file(player_ptr, true, file_name, _("各アライアンスからの印象値", "Impression degree from each alliances"), 0, 0);
    fd_kill(file_name);

}
