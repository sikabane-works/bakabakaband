﻿#pragma once

#include "object/tval-types.h"
#include "store/store-owners.h"
#include "store/store.h"
#include "system/angband.h"
#include <map>

struct store_stock_item_type {
    ItemKindType tval;
    OBJECT_SUBTYPE_VALUE sval;
};

extern store_stock_item_type store_regular_table[MAX_STORES][STORE_MAX_KEEP];
extern const std::map<StoreSaleType, std::vector<store_stock_item_type>> store_sale_table;
