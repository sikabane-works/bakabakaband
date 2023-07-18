﻿#pragma once

enum class MonsterRaceId : int16_t {
    PLAYER = 0, // Dummy.
    MAGGOT = 8,
    BEGGAR = 12,
    LEPER = 13,
    LION_HEART = 19,
    NOV_PRIEST = 45,
    GRIP = 53,
    WOLF = 54,
    FANG = 55,
    LOUSE = 69,
    PIRANHA = 70,
    COPPER_COINS = 85,
    SWORDFISH = 88,
    NOV_PALADIN = 97,
    NOV_PRIEST_G = 109,
    SILVER_COINS = 117,
    D_ELF = 122,
    MANES = 128,
    NOV_PALADIN_G = 147,
    PHANTOM_W = 152,
    WOUNDED_BEAR = 159,
    D_ELF_MAGE = 178,
    D_ELF_WARRIOR = 182,
    BLUE_HORROR = 189,
    GOLD_COINS = 195,
    MASTER_YEEK = 224,
    PRIEST = 225,
    D_ELF_PRIEST = 226,
    MITHRIL_COINS = 239,
    PINK_HORROR = 242,
    IMP = 296,
    LIZARD_KING = 332,
    WYVERN = 334,
    SABRE_TIGER = 339,
    D_ELF_LORD = 348,
    ARCH_VILE = 357,
    JADE_MONK = 370,
    D_ELF_WARLOCK = 375,
    MENELDOR = 384,
    PHANTOM_B = 385,
    IT = 393,
    D_ELF_DRUID = 400,
    GWAIHIR = 410,
    ADAMANT_COINS = 423,
    COLBRAN = 435,
    MITHRIL_GOLEM = 464,
    THORONDOR = 468,
    GHOUL_KING = 483,
    NINJA = 485,
    BICLOPS = 490,
    IVORY_MONK = 492,
    GOEMON = 505,
    WATER_ELEM = 512,
    BLOODLETTER = 523,
    STAR_VAMPIRE = 536,
    RAAL = 557,
    NIGHTBLADE = 564,
    PLASMA_VORTEX = 588,
    SKY_WHALE = 594,
    BARON_HELL = 609,
    ANCIENT_CRISTAL_DRAGON = 646,
    FALLEN_ANGEL = 652,
    D_ELF_SORC = 657,
    DREADMASTER = 690,
    DROLEM = 691,
    DAWN = 693,
    NAZGUL = 696,
    SMAUG = 697,
    STORMBRINGER = 698,
    ULTRA_PALADIN = 699,
    S_TYRANNO = 705,
    FAFNER = 712,
    G_BALROG = 720,
    BULLGATES = 732,
    LORD_CHAOS = 737,
    NIGHTWALKER = 768,
    SHADOWLORD = 774,
    JABBERWOCK = 778,
    SHAMBLER = 786,
    TIAMAT = 795,
    BLACK_REAVER = 798,
    NULL_ = 803,
    UNMAKER = 815,
    CYBER = 816,
    ANGMAR = 825,
    WYRM_POWER = 847,
    JORMUNGAND = 854,
    SAURON = 858,
    UNICORN_ORD = 859,
    OBERON = 860,
    MORGOTH = 861,
    SERPENT = 862,
    ONE_RING = 864,
    BEHINDER = 868,
    EBONY_MONK = 870,
    HAGURE = 871,
    DIO = 878,
    OHMU = 879,
    WONG = 880,
    ZOMBI_SERPENT = 883,
    SHALLOW_PUDDLE = 885,
    D_ELF_SHADE = 886,
    TROLL_KING = 894,
    ELF_LORD = 900,
    TSUCHINOKO = 926,
    MARIO = 927,
    LOCKE_CLONE = 930,
    CALDARM = 931,
    BANORLUPART = 932,
    BANOR = 933,
    LUPART = 934,
    KENSHIROU = 936,
    W_KNIGHT = 938,
    DEEP_PUDDLE = 944,
    BIKETAL = 945,
    IKETA = 949,
    B_DEATH_SWORD = 953,
    YASE_HORSE = 955,
    HORSE = 956,
    BOTEI = 963,
    KAGE = 964,
    CHEST_MIMIC_02 = 965,
    LUIGI = 966,
    JAIAN = 967,
    FENGHUANG = 988,
    SUKE = 1001,
    KAKU = 1002,
    A_GOLD = 1010,
    A_SILVER = 1011,
    ROLENTO = 1013,
    RAOU = 1018,
    GRENADE = 1023,
    DEBBY = 1032,
    KNI_TEMPLAR = 1037,
    PALADIN = 1038,
    CHAMELEON = 1040,
    CHAMELEON_K = 1041,
    TOPAZ_MONK = 1047,
    M_MINDCRAFTER = 1056,
    ELDER_VAMPIRE = 1058,
    NOBORTA = 1059,
    MORI_TROLL = 1060,
    BARNEY = 1061,
    GROO = 1062,
    LOUSY = 1063,
    WYRM_SPACE = 1064,
    JIZOTAKO = 1065,
    TANUKI = 1067,
    ALIEN_JURAL = 1082,
    JURAL_WITCHKING = 1089,
    MIDDLE_AQUA_FIRST = 1152,
    LARGE_AQUA_FIRST = 1153,
    EXTRA_LARGE_AQUA_FIRST = 1154,
    MIDDLE_AQUA_SECOND = 1156,
    LARGE_AQUA_SECOND = 1157,
    EXTRA_LARGE_AQUA_SECOND = 1158,
    SMALL_MOAI = 1159,
    TOTEM_MOAI = 1161,
    VAIF = 1162,
    DESLAYER_SENIOR = 1164,
    DESLAYER_MEMBER = 1165,
    MASTER_MYSTIC = 1178,
    BRONZE_LICH = 1180,
    ULT_BEHOLDER = 1185,
    G_TITAN = 1187,
    WYRM_COLOURS = 1198,
    MIRMULNIR = 1203,
    ALDUIN = 1209,
    FOLLOWER_WARRIOR = 1210,
    FOLLOWER_MAGE = 1211,
    MIRAAK = 1212,
    SCARAB = 1220,
    IMHOTEP = 1221,
    EDGE = 1245,
    EYE_PHORN = 1246,
    JOBZ = 1247,
    VESPOID = 1252,
    QUEEN_VESPOID = 1253,
    DRAGON_WORM = 1256,
    DRAGON_CENTIPEDE = 1257,
    CAIT_SITH = 1262,
    SHIVA_BOOTS = 1264,
    BIG_RAVEN = 1268,
    MELKO = 1287,
    STOLENMAN = 1288,
    DOKACHAN = 1307,
    OLDMAN_60TY = 1353,
    BROTHER_45TH = 1354,
    YENDOR_WIZARD_1 = 1360,
    YENDOR_WIZARD_2 = 1361,
    DOPPIO = 1366,
    DIAVOLO = 1367,
    MANIMANI = 1368,
    LOSTRINGIL = 1383,
    POWERFUL_EYE_SENIOR = 1338,
    MISUMI = 1393,
    LEE_QIEZI = 1399,
    DONELD = 1401,
    THUNDERS = 1402,
    BUFFALO = 1404,
    GOLDEN_EAGLE = 1405,
    SWALLOW = 1406,
    FIGHTER = 1407,
    HAWK = 1408,
    LION = 1409,
    BOTTLE_GNOME = 1444,
    EARTH_DESTROYER = 1445,
    OOTSUKI = 1453,
    TURBAN_KID = 1467,
    INARIMAN_1 = 1518,
    INARIMAN_2 = 1519,
    INARIMAN_3 = 1520,
    CHEST_MIMIC_03 = 1529,
    CHEST_MIMIC_04 = 1530,
    CHEST_MIMIC_11 = 1531,
    SHITTO_MASK = 1546,
    BINZYOU_MUR = 1587,
    PRINCESS_KETHOLDETH = 1590,

};
