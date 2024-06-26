#include "info-reader/feature-info-tokens-table.h"

/*!
 * @brief 地形属性トークンの定義 / Feature info flags
 */
const std::unordered_map<std::string_view, TerrainCharacteristics> f_info_flags = {
    { "LOS", TerrainCharacteristics::LOS },
    { "PROJECT", TerrainCharacteristics::PROJECT },
    { "MOVE", TerrainCharacteristics::MOVE },
    { "PLACE", TerrainCharacteristics::PLACE },
    { "DROP", TerrainCharacteristics::DROP },
    { "SECRET", TerrainCharacteristics::SECRET },
    { "NOTICE", TerrainCharacteristics::NOTICE },
    { "REMEMBER", TerrainCharacteristics::REMEMBER },
    { "OPEN", TerrainCharacteristics::OPEN },
    { "CLOSE", TerrainCharacteristics::CLOSE },
    { "BASH", TerrainCharacteristics::BASH },
    { "SPIKE", TerrainCharacteristics::SPIKE },
    { "DISARM", TerrainCharacteristics::DISARM },
    { "STORE", TerrainCharacteristics::STORE },
    { "TUNNEL", TerrainCharacteristics::TUNNEL },
    { "MAY_HAVE_GOLD", TerrainCharacteristics::MAY_HAVE_GOLD },
    { "HAS_GOLD", TerrainCharacteristics::HAS_GOLD },
    { "HAS_ITEM", TerrainCharacteristics::HAS_ITEM },
    { "DOOR", TerrainCharacteristics::DOOR },
    { "TRAP", TerrainCharacteristics::TRAP },
    { "STAIRS", TerrainCharacteristics::STAIRS },
    { "RUNE_PROTECTION", TerrainCharacteristics::RUNE_PROTECTION },
    { "LESS", TerrainCharacteristics::LESS },
    { "MORE", TerrainCharacteristics::MORE },
    { "AVOID_RUN", TerrainCharacteristics::AVOID_RUN },
    { "FLOOR", TerrainCharacteristics::FLOOR },
    { "WALL", TerrainCharacteristics::WALL },
    { "PERMANENT", TerrainCharacteristics::PERMANENT },
    { "CHAOS_TAINTED", TerrainCharacteristics::CHAOS_TAINTED },
    { "VOID", TerrainCharacteristics::VOID },
    // { "XXX00", TerrainCharacteristics::XXX00 },
    // { "XXX01", TerrainCharacteristics::XXX01 },
    // { "XXX02", TerrainCharacteristics::XXX02 },
    { "HIT_TRAP", TerrainCharacteristics::HIT_TRAP },

    // { "BRIDGE", TerrainCharacteristics::BRIDGE },
    // { "RIVER", TerrainCharacteristics::RIVER },
    // { "LAKE", TerrainCharacteristics::LAKE },
    // { "BRIDGED", TerrainCharacteristics::BRIDGED },
    // { "COVERED", TerrainCharacteristics::COVERED },
    { "GLOW", TerrainCharacteristics::GLOW },
    { "ENSECRET", TerrainCharacteristics::ENSECRET },
    { "WATER", TerrainCharacteristics::WATER },
    { "LAVA", TerrainCharacteristics::LAVA },
    { "SHALLOW", TerrainCharacteristics::SHALLOW },
    { "DEEP", TerrainCharacteristics::DEEP },
    { "POISON_PUDDLE", TerrainCharacteristics::POISON_PUDDLE },
    { "HURT_ROCK", TerrainCharacteristics::HURT_ROCK },
    // { "HURT_FIRE", TerrainCharacteristics::HURT_FIRE },
    // { "HURT_COLD", TerrainCharacteristics::HURT_COLD },
    // { "HURT_ACID", TerrainCharacteristics::HURT_ACID },
    { "COLD_PUDDLE", TerrainCharacteristics::COLD_PUDDLE },
    { "ACID_PUDDLE", TerrainCharacteristics::ACID_PUDDLE },
    // { "OIL", TerrainCharacteristics::OIL },
    { "ELEC_PUDDLE", TerrainCharacteristics::ELEC_PUDDLE },
    // { "CAN_CLIMB", TerrainCharacteristics::CAN_CLIMB },
    { "CAN_FLY", TerrainCharacteristics::CAN_FLY },
    { "CAN_SWIM", TerrainCharacteristics::CAN_SWIM },
    { "CAN_PASS", TerrainCharacteristics::CAN_PASS },
    // { "CAN_OOZE", TerrainCharacteristics::CAN_OOZE },
    { "CAN_DIG", TerrainCharacteristics::CAN_DIG },
    // { "HIDE_ITEM", TerrainCharacteristics::HIDE_ITEM },
    // { "HIDE_SNEAK", TerrainCharacteristics::HIDE_SNEAK },
    // { "HIDE_SWIM", TerrainCharacteristics::HIDE_SWIM },
    // { "HIDE_DIG", TerrainCharacteristics::HIDE_DIG },
    // { "KILL_HUGE", TerrainCharacteristics::KILL_HUGE },
    // { "KILL_MOVE", TerrainCharacteristics::KILL_MOVE },

    // { "PICK_TRAP", TerrainCharacteristics::PICK_TRAP },
    // { "PICK_DOOR", TerrainCharacteristics::PICK_DOOR },
    // { "ALLOC", TerrainCharacteristics::ALLOC },
    // { "CHEST", TerrainCharacteristics::CHEST },
    // { "DROP_1D2", TerrainCharacteristics::DROP_1D2 },
    // { "DROP_2D2", TerrainCharacteristics::DROP_2D2 },
    // { "DROP_GOOD", TerrainCharacteristics::DROP_GOOD },
    // { "DROP_GREAT", TerrainCharacteristics::DROP_GREAT },
    // { "HURT_POIS", TerrainCharacteristics::HURT_POIS },
    // { "HURT_ELEC", TerrainCharacteristics::HURT_ELEC },
    // { "HURT_WATER", TerrainCharacteristics::HURT_WATER },
    // { "HURT_BWATER", TerrainCharacteristics::HURT_BWATER },
    // { "USE_FEAT", TerrainCharacteristics::USE_FEAT },
    // { "GET_FEAT", TerrainCharacteristics::GET_FEAT },
    // { "GROUND", TerrainCharacteristics::GROUND },
    // { "OUTSIDE", TerrainCharacteristics::OUTSIDE },
    // { "EASY_HIDE", TerrainCharacteristics::EASY_HIDE },
    // { "EASY_CLIMB", TerrainCharacteristics::EASY_CLIMB },
    // { "MUST_CLIMB", TerrainCharacteristics::MUST_CLIMB },
    { "TREE", TerrainCharacteristics::TREE },
    // { "NEED_TREE", TerrainCharacteristics::NEED_TREE },
    // { "BLOOD", TerrainCharacteristics::BLOOD },
    // { "DUST", TerrainCharacteristics::DUST },
    // { "SLIME", TerrainCharacteristics::SLIME },
    { "PLANT", TerrainCharacteristics::PLANT },
    // { "XXX2", TerrainCharacteristics::XXX2 },
    // { "INSTANT", TerrainCharacteristics::INSTANT },
    // { "EXPLODE", TerrainCharacteristics::EXPLODE },
    // { "TIMED", TerrainCharacteristics::TIMED },
    // { "ERUPT", TerrainCharacteristics::ERUPT },
    // { "STRIKE", TerrainCharacteristics::STRIKE },
    // { "SPREAD", TerrainCharacteristics::SPREAD },

    { "SPECIAL", TerrainCharacteristics::SPECIAL },
    { "HURT_DISI", TerrainCharacteristics::HURT_DISI },
    { "QUEST_ENTER", TerrainCharacteristics::QUEST_ENTER },
    { "QUEST_EXIT", TerrainCharacteristics::QUEST_EXIT },
    { "QUEST", TerrainCharacteristics::QUEST },
    { "SHAFT", TerrainCharacteristics::SHAFT },
    { "MOUNTAIN", TerrainCharacteristics::MOUNTAIN },
    { "BLDG", TerrainCharacteristics::BLDG },
    { "RUNE_EXPLOSION", TerrainCharacteristics::RUNE_EXPLOSION },
    { "PATTERN", TerrainCharacteristics::PATTERN },
    { "TOWN", TerrainCharacteristics::TOWN },
    { "ENTRANCE", TerrainCharacteristics::ENTRANCE },
    { "MIRROR", TerrainCharacteristics::MIRROR },
    { "UNPERM", TerrainCharacteristics::UNPERM },
    { "TELEPORTABLE", TerrainCharacteristics::TELEPORTABLE },
    { "CONVERT", TerrainCharacteristics::CONVERT },
    { "GLASS", TerrainCharacteristics::GLASS },
    { "DUNG_POOL", TerrainCharacteristics::DUNG_POOL },
    { "PLASMA", TerrainCharacteristics::PLASMA },
    { "RUNE_HEALING", TerrainCharacteristics::RUNE_HEALING },
    { "SLOW", TerrainCharacteristics::SLOW },
    { "THORN", TerrainCharacteristics::THORN },
    { "TENTACLE", TerrainCharacteristics::TENTACLE },

};
