#include "system/angband-version.h"
#include "system/angband.h"

void put_version(char *buf)
{
    std::string_view expr;
    switch (VERSION_STATUS) {
    case VersionStatusType::ALPHA:
        expr = "Alpha";
        break;
    case VersionStatusType::BETA:
        expr = "Beta";
        break;
    case VersionStatusType::RELEASE_CANDIDATE:
        expr = "RC";
        break;
    case VersionStatusType::RELEASE:
        expr = "";
        break;
    default:
        throw("Invalid version status was specified!");
    }

    if (VERSION_STATUS != VersionStatusType::RELEASE) {
        sprintf(buf, _("馬鹿馬鹿蛮怒 %d.%d.%d%s%d", "Bakabakaband %d.%d.%d%s%d"), H_VER_MAJOR, H_VER_MINOR, H_VER_PATCH, expr.data(), H_VER_EXTRA);
    } else {
        concptr mode = IS_STABLE_VERSION ? _("安定版", "Stable") : _("開発版", "Developing");
        sprintf(buf, _("馬鹿馬鹿蛮怒 %d.%d.%d.%d(%s)", "Bakabakaband %d.%d.%d.%d(%s)"), H_VER_MAJOR, H_VER_MINOR, H_VER_PATCH,
            H_VER_EXTRA, mode);
    }
}
