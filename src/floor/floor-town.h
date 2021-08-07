#pragma once

#include "system/angband.h"

typedef struct store_type store_type;

/*
 * A structure describing a town with
 * stores and buildings
 */
typedef struct town_type {
	GAME_TEXT name[32];
	u32b seed;      /* Seed for RNG */
	store_type *store;    /* The stores [MAX_STORES] */
	byte numstores;
    u16b entrance_x; /*!< 入口X座標*/
    u16b entrance_y; /*!< 入口Y座標*/
} town_type;

extern TOWN_IDX max_towns;
extern town_type *town_info;
