#include "system/angband-version.h"
#include "system/angband.h"

void put_version(char *buf)
{
    if (IS_ALPHA_VERSION) {
        sprintf(buf, _("馬鹿馬鹿蛮怒 %d.%d.%dAlpha%d", "Bakabakaband %d.%d.%dAlpha%d"), H_VER_MAJOR, H_VER_MINOR, H_VER_PATCH, H_VER_EXTRA);
    } else {
        char *mode = IS_STABLE_VERSION ? _("安定版", "Stable") : _("開発版", "Developing");
        sprintf(buf, _("馬鹿馬鹿蛮怒 %d.%d.%d.%d(%s)", "Bakabakaband %d.%d.%d.%d(%s)"), H_VER_MAJOR, H_VER_MINOR, H_VER_PATCH, H_VER_EXTRA, mode);
    }
}