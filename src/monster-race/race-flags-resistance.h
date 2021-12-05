#pragma once

enum race_flags_resistance {
	RFR_IM_ACID = 0x00000001, /* Immunity acid */
    RFR_IM_ELEC = 0x00000002, /* Immunity elec */
    RFR_IM_FIRE = 0x00000004, /* Immunity fire */
    RFR_IM_COLD = 0x00000008, /* Immunity cold */
    RFR_IM_POIS = 0x00000010, /* Immunity poison */
    RFR_RES_LITE = 0x00000020, /* Resist lite */
    RFR_RES_DARK = 0x00000040, /* Resist dark */
    RFR_RES_NETH = 0x00000080, /* Resist nether */
    RFR_RES_WATE = 0x00000100, /* Resist water */
    RFR_RES_PLAS = 0x00000200, /* Resist plasma */
    RFR_RES_SHAR = 0x00000400, /* Resist shards */
    RFR_RES_SOUN = 0x00000800, /* Resist sound */
    RFR_RES_CHAO = 0x00001000, /* Resist chaos */
    RFR_RES_NEXU = 0x00002000, /* Resist nexus */
    RFR_RES_DISE = 0x00004000, /* Resist disenchantment */
    RFR_RES_WALL = 0x00008000, /* Resist force */
    RFR_RES_INER = 0x00010000, /* Resist inertia */
    RFR_RES_TIME = 0x00020000, /* Resist time */
    RFR_RES_GRAV = 0x00040000, /* Resist gravity */
    RFR_RES_ALL = 0x00080000, /* Resist all */
    RFR_RES_TELE = 0x00100000, /* Resist teleportation */
    RFR_RES_ACID = 0x00200000, /* Resist acid */
    RFR_RES_ELEC = 0x00400000, /* Resist elec */
    RFR_RES_FIRE = 0x00800000, /* Resist fire */
    RFR_RES_COLD = 0x01000000, /* Resist cold */
    RFR_RES_POIS = 0x02000000, /* Resist poison */
    RFR_XXX26 = 0x04000000,
    RFR_XXX27 = 0x08000000,
    RFR_XXX28 = 0x10000000,
    RFR_XXX29 = 0x20000000,
    RFR_XXX30 = 0x40000000,
    RFR_XXX31 = 0x80000000,
};

#define RFR_EFF_IM_ACID_MASK (RFR_RES_ACID | RFR_IM_ACID | RFR_RES_ALL)
#define RFR_EFF_IM_ELEC_MASK (RFR_RES_ELEC | RFR_IM_ELEC | RFR_RES_ALL)
#define RFR_EFF_IM_FIRE_MASK (RFR_RES_FIRE | RFR_IM_FIRE | RFR_RES_ALL)
#define RFR_EFF_IM_COLD_MASK (RFR_RES_COLD | RFR_IM_COLD | RFR_RES_ALL)
#define RFR_EFF_IM_POIS_MASK (RFR_RES_POIS | RFR_IM_POIS | RFR_RES_ALL)
#define RFR_EFF_RES_SHAR_MASK (RFR_RES_SHAR | RFR_RES_ALL)
#define RFR_EFF_RES_CHAO_MASK (RFR_RES_CHAO | RFR_RES_ALL)
#define RFR_EFF_RES_NEXU_MASK (RFR_RES_NEXU | RFR_RES_ALL)
