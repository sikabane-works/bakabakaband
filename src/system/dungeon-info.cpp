#include "system/dungeon-info.h"
#include "game-option/birth-options.h"
#include "grid/feature.h"
#include "io/input-key-acceptor.h"
#include "main/sound-of-music.h"
#include "monster-race/monster-race.h"
#include "system/floor-type-definition.h"
#include "system/monster-race-definition.h"
#include "system/player-type-definition.h"
#include "term/screen-processor.h"
#include "util/int-char-converter.h"
#include "view/display-messages.h"
#include "world/world.h"

/*
 * The dungeon arrays
 */
std::vector<dungeon_type> dungeons_info;

/*
 * Maximum number of dungeon in DungeonDefinitions.txt
 */
std::vector<DEPTH> max_dlv;
