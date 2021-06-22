﻿/*!
 * @brief これまでの行いを表示する
 * @date 2021/06/21
 * @author Deskull
 */

#include "core/show-file.h"
#include "io-dump/dump-util.h"
#include "system/player-type-definition.h"
#include "util/angband-files.h"
#include <string>

void do_cmd_knowledge_incident(player_type *creature_ptr)
{
    FILE *fff = NULL;
    GAME_TEXT file_name[FILE_NAME_SIZE];
    if (!open_temporary_file(&fff, file_name))
        return;

    fprintf(fff, _("あなたはこれまで%d歩進んだ。\n", "You walked %d steps\n"), creature_ptr->incident[INCIDENT::WALK]);
    fprintf(fff, _("あなたはこれまで%d回攻撃動作を行い、%d回攻撃を実行した。\n", "You have prepared %d attack action and execute %d attacks. \n"),
        creature_ptr->incident[INCIDENT::ATTACK_ACT_COUNT], creature_ptr->incident[INCIDENT::ATTACK_EXE_COUNT]);
    angband_fclose(fff);
    (void)show_file(creature_ptr, true, file_name, _("これまでの出来事", "Incidents"), 0, 0);
    fd_kill(file_name);

}
