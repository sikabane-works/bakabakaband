﻿/*!
 * @brief ファイル入出力管理 / Purpose: code dealing with files (and death)
 * @date 2014/01/28
 * @author
 * <pre>
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 * 2014 Deskull rearranged comment for Doxygen.\n
 * </pre>
 */

#include "io/files-util.h"
#include "io-dump/character-dump.h"
#include "io/input-key-acceptor.h"
#include "io/uid-checker.h"
#include "monster-race/race-flags1.h"
#include "util/angband-files.h"

concptr ANGBAND_DIR; //!< Path name: The main "lib" directory This variable is not actually used anywhere in the code
concptr ANGBAND_DIR_APEX; //!< High score files (binary) These files may be portable between platforms
concptr ANGBAND_DIR_BONE; //!< Bone files for player ghosts (ascii) These files are portable between platforms
concptr ANGBAND_DIR_DATA; //!< Binary image files for the "*_info" arrays (binary) These files are not portable between platforms
concptr ANGBAND_DIR_EDIT; //!< Textual template files for the "*_info" arrays (ascii) These files are portable between platforms
concptr ANGBAND_DIR_SCRIPT; //!< Script files These files are portable between platforms.
concptr ANGBAND_DIR_FILE; //!< Various extra files (ascii) These files may be portable between platforms
concptr ANGBAND_DIR_HELP; //!< Help files (normal) for the online help (ascii) These files are portable between platforms
concptr ANGBAND_DIR_INFO; //!< Help files (spoilers) for the online help (ascii) These files are portable between platforms
concptr ANGBAND_DIR_PREF; //!< Default user "preference" files (ascii) These files are rarely portable between platforms
concptr ANGBAND_DIR_SAVE; //!< Savefiles for current characters (binary)
concptr ANGBAND_DIR_USER; //!< User "preference" files (ascii) These files are rarely portable between platforms
concptr ANGBAND_DIR_XTRA; //!< Various extra files (binary) These files are rarely portable between platforms

/*
 * Buffer to hold the current savefile name
 * 'savefile' holds full path name. 'savefile_base' holds only base name.
 */
char savefile[1024];
char savefile_base[40];

/*!
 * @brief プレイヤーステータスをファイルダンプ出力する
 * Hack -- Dump a character description file
 * @param creature_ptr プレーヤーへの参照ポインタ
 * @param name 出力ファイル名
 * @return エラーコード
 * @details
 * Allow the "full" flag to dump additional info,
 * and trigger its usage from various places in the code.
 */
errr file_character(player_type *creature_ptr, concptr name, update_playtime_pf update_playtime, display_player_pf display_player, map_name_pf map_name)
{
    char buf[1024];
    path_build(buf, sizeof(buf), ANGBAND_DIR_USER, name);
    int fd = fd_open(buf, O_RDONLY);
    if (fd >= 0) {
        char out_val[160];
        (void)fd_close(fd);
        (void)sprintf(out_val, _("現存するファイル %s に上書きしますか? ", "Replace existing file %s? "), buf);
        if (get_check_strict(creature_ptr, out_val, CHECK_NO_HISTORY))
            fd = -1;
    }

    FILE *fff = NULL;
    if (fd < 0)
        fff = angband_fopen(buf, "w");

    if (!fff) {
        prt(_("キャラクタ情報のファイルへの書き出しに失敗しました！", "Character dump failed!"), 0, 0);
        (void)inkey();
        return -1;
    }

    make_character_dump(creature_ptr, fff, update_playtime, display_player, map_name);
    angband_fclose(fff);
    msg_print(_("キャラクタ情報のファイルへの書き出しに成功しました。", "Character dump successful."));
    msg_print(NULL);
    return 0;
}

/*!
 * @brief ファイルからランダムに行を一つ取得する /
 * Get a random line from a file
 * @param file_name ファイル名
 * @param entry 特定条件時のN:タグヘッダID
 * @param output 出力先の文字列参照ポインタ
 * @return エラーコード
 * @details
 * <pre>
 * Based on the monster speech patch by Matt Graham,
 * </pre>
 */
errr get_rnd_line(concptr file_name, int entry, char *output)
{
    char buf[1024];
    path_build(buf, sizeof(buf), ANGBAND_DIR_FILE, file_name);
    FILE *fp;
    fp = angband_fopen(buf, "r");
    if (!fp)
        return -1;

    int test;
    int line_num = 0;
    while (TRUE) {
        if (angband_fgets(fp, buf, sizeof(buf)) != 0) {
            angband_fclose(fp);
            return -1;
        }

        line_num++;
        if ((buf[0] != 'N') || (buf[1] != ':'))
            continue;

        if (buf[2] == '*') {
            break;
        } else if (buf[2] == 'M') {
            if (r_info[entry].flags1 & RF1_MALE)
                break;
        } else if (buf[2] == 'F') {
            if (r_info[entry].flags1 & RF1_FEMALE)
                break;
        } else if (sscanf(&(buf[2]), "%d", &test) != EOF) {
            if (test == entry)
                break;
        } else {
            msg_format("Error in line %d of %s!", line_num, file_name);
            angband_fclose(fp);
            return -1;
        }
    }

    int counter;
    for (counter = 0;; counter++) {
        while (TRUE) {
            test = angband_fgets(fp, buf, sizeof(buf));
            if (!test) {
                /* Ignore lines starting with 'N:' */
                if ((buf[0] == 'N') && (buf[1] == ':'))
                    continue;

                if (buf[0] != '#')
                    break;
            } else
                break;
        }

        if (!buf[0])
            break;

        if (one_in_(counter + 1))
            strcpy(output, buf);
    }

    angband_fclose(fp);
    return counter ? 0 : -1;
}

#ifdef JP
/*!
 * @brief ファイルからランダムに行を一つ取得する(日本語文字列のみ) /
 * @param file_name ファイル名
 * @param entry 特定条件時のN:タグヘッダID
 * @param output 出力先の文字列参照ポインタ
 * @param count 試行回数
 * @return エラーコード
 * @details
 */
errr get_rnd_line_jonly(concptr file_name, int entry, char *output, int count)
{
    errr result = 1;
    for (int i = 0; i < count; i++) {
        result = get_rnd_line(file_name, entry, output);
        if (result)
            break;
        bool kanji = FALSE;
        for (int j = 0; output[j]; j++)
            kanji |= iskanji(output[j]);
        if (kanji)
            break;
    }

    return result;
}
#endif

/*!
 * @brief ファイル位置をシーク /
 * @param creature_ptr プレーヤーへの参照ポインタ
 * @param fd ファイルディスクリプタ
 * @param where ファイルバイト位置
 * @param flag FALSEならば現ファイルを超えた位置へシーク時エラー、TRUEなら足りない間を0で埋め尽くす
 * @return エラーコード
 * @details
 */
static errr counts_seek(player_type *creature_ptr, int fd, u32b where, bool flag)
{
    char temp1[128], temp2[128];
#ifdef SAVEFILE_USE_UID
    (void)sprintf(temp1, "%d.%s.%d%d%d", creature_ptr->player_uid, savefile_base, creature_ptr->pclass, creature_ptr->pseikaku, creature_ptr->age);
#else
    (void)sprintf(temp1, "%s.%d%d%d", savefile_base, creature_ptr->pclass, creature_ptr->pseikaku, creature_ptr->age);
#endif
    for (int i = 0; temp1[i]; i++)
        temp1[i] ^= (i + 1) * 63;

    int seekpoint = 0;
    u32b zero_header[3] = { 0L, 0L, 0L };
    while (TRUE) {
        if (fd_seek(fd, seekpoint + 3 * sizeof(u32b)))
            return 1;
        if (fd_read(fd, (char *)(temp2), sizeof(temp2))) {
            if (!flag)
                return 1;
            /* add new name */
            fd_seek(fd, seekpoint);
            fd_write(fd, (char *)zero_header, 3 * sizeof(u32b));
            fd_write(fd, (char *)(temp1), sizeof(temp1));
            break;
        }

        if (strcmp(temp1, temp2) == 0)
            break;

        seekpoint += 128 + 3 * sizeof(u32b);
    }

    return fd_seek(fd, seekpoint + where * sizeof(u32b));
}

/*!
 * @brief ファイル位置を読み込む
 * @param creature_ptr プレーヤーへの参照ポインタ
 * @param where ファイルバイト位置
 * @return エラーコード
 * @details
 */
u32b counts_read(player_type *creature_ptr, int where)
{
    char buf[1024];
    path_build(buf, sizeof(buf), ANGBAND_DIR_DATA, _("z_info_j.raw", "z_info.raw"));
    int fd = fd_open(buf, O_RDONLY);

    u32b count = 0;
    if (counts_seek(creature_ptr, fd, where, FALSE) || fd_read(fd, (char *)(&count), sizeof(u32b)))
        count = 0;

    (void)fd_close(fd);

    return count;
}

/*!
 * @brief ファイル位置に書き込む /
 * @param creature_ptr プレーヤーへの参照ポインタ
 * @param where ファイルバイト位置
 * @param count 書き込む値
 * @return エラーコード
 * @details
 */
errr counts_write(player_type *creature_ptr, int where, u32b count)
{
    char buf[1024];
    path_build(buf, sizeof(buf), ANGBAND_DIR_DATA, _("z_info_j.raw", "z_info.raw"));

    safe_setuid_grab();
    int fd = fd_open(buf, O_RDWR);
    safe_setuid_drop();
    if (fd < 0) {
        safe_setuid_grab();
        fd = fd_make(buf, 0644);
        safe_setuid_drop();
    }

    safe_setuid_grab();
    errr err = fd_lock(fd, F_WRLCK);
    safe_setuid_drop();
    if (err)
        return 1;

    counts_seek(creature_ptr, fd, where, TRUE);
    fd_write(fd, (char *)(&count), sizeof(u32b));
    safe_setuid_grab();
    err = fd_lock(fd, F_UNLCK);
    safe_setuid_drop();

    if (err)
        return 1;

    (void)fd_close(fd);
    return 0;
}

/*!
 * @brief 墓のアスキーアートテンプレを読み込む
 * @param buf テンプレへのバッファ
 * @param buf_size バッファの長さ
 * @return なし
 */
void read_dead_file(char *buf, size_t buf_size)
{
    path_build(buf, buf_size, ANGBAND_DIR_FILE, _("dead_j.txt", "dead.txt"));

    FILE *fp;
    fp = angband_fopen(buf, "r");
    if (!fp)
        return;

    int i = 0;
    while (angband_fgets(fp, buf, buf_size) == 0) {
        put_str(buf, i++, 0);
    }

    angband_fclose(fp);
}
