#pragma once

#include "system/angband.h"

#define HISTPREF_LIMIT 1024

extern std::optional<std::string> histpref_buf;

class PlayerType;
errr interpret_pref_file(PlayerType *player_ptr, char *buf);
