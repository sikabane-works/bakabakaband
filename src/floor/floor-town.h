#pragma once

#include "system/angband.h"

class store_type;

/*
 * A structure describing a town with
 * stores and buildings
 */
typedef struct town_type {
	GAME_TEXT name[32];
	uint32_t seed;      /* Seed for RNG */
	store_type *store;    /* The stores [MAX_STORES] */
	byte numstores;
} town_type;

extern int16_t max_towns;
extern town_type *town_info;
