#pragma once

#include <stdint.h>
#include "uthash.h"
#include "raylib.h"
#include "level.h"
#include "stdio.h"

#define TILE_SIZE 8
#define ATLAS_WIDTH 64
#define TILES_PER_ROW (ATLAS_WIDTH / TILE_SIZE)

typedef struct
{
  uint8_t mask;
  uint8_t index;
  UT_hash_handle hh;
} TileRule;

void initTileRulesHashMap(TileRule **tileRulesHashMap);

uint8_t getTileIndex(TileRule *tileRulesHashMap, uint8_t mask);

Rectangle getTileSourceRect(TileRule *tileRulesHashMap, Level *level, int x, int y);

uint8_t getTileBitmask(Level *level, int x, int y);
