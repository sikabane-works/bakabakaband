#include "io-dump/dump-remover.h"
#include "io-dump/dump-util.h"
#include "io/read-pref-file.h"
#include "term/z-form.h"
#include "util/angband-files.h"

/*!
 * @brief prefファイルを選択して処理する /
 * Ask for a "user pref line" and process it
 * @brief prf出力内容を消去する /
 * Remove old lines automatically generated before.
 * @param orig_file 消去を行うファイル名
 * @param auto_dump_mark 出力するヘッダマーク
 */
void remove_auto_dump(const std::filesystem::path &orig_file, std::string_view auto_dump_mark)
{
    bool between_mark = false;
    bool changed = false;
    int line_num = 0;
    long header_location = 0;

    const auto header_mark_str = format(auto_dump_header, auto_dump_mark.data());
    const auto footer_mark_str = format(auto_dump_footer, auto_dump_mark.data());
    size_t mark_len = footer_mark_str.length();

    FILE *orig_fff;
    orig_fff = angband_fopen(orig_file, FileOpenMode::READ);
    if (!orig_fff) {
        return;
    }

    FILE *tmp_fff = nullptr;
    char tmp_file[FILE_NAME_SIZE];
    if (!open_temporary_file(&tmp_fff, tmp_file)) {
        return;
    }

    while (true) {
        const auto buf = angband_fgets(orig_fff);
        if (!buf) {
            if (between_mark) {
                fseek(orig_fff, header_location, SEEK_SET);
                between_mark = false;
                continue;
            } else {
                break;
            }
        }

        if (!between_mark) {
            if (!strcmp(buf->data(), header_mark_str.data())) {
                header_location = ftell(orig_fff);
                line_num = 0;
                between_mark = true;
                changed = true;
            } else {
                fprintf(tmp_fff, "%s\n", buf->data());
            }

            continue;
        }

        if (!strncmp(buf->data(), footer_mark_str.data(), mark_len)) {
            int tmp;
            if (!sscanf(buf->data() + mark_len, " (%d)", &tmp) || tmp != line_num) {
                fseek(orig_fff, header_location, SEEK_SET);
            }

            between_mark = false;
            continue;
        }

        line_num++;
    }

    angband_fclose(orig_fff);
    angband_fclose(tmp_fff);

    if (changed) {
        tmp_fff = angband_fopen(tmp_file, FileOpenMode::READ);
        orig_fff = angband_fopen(orig_file, FileOpenMode::WRITE);
        while (true) {
            const auto buf = angband_fgets(tmp_fff);
            if (!buf) {
                break;
            }
            fprintf(orig_fff, "%s\n", buf->data());
        }

        angband_fclose(orig_fff);
        angband_fclose(tmp_fff);
    }

    fd_kill(tmp_file);
}
