#pragma once

#define MAP_SIZE 256
#define BLOCK_SIZE 32

typedef enum TILE_TYPE
{
  TILE_VOID,
  TILE_GROUND,
} TILE_TYPE;

typedef struct Level
{
  int generated;
  TILE_TYPE tiles[MAP_SIZE][MAP_SIZE];
} Level;

void generateLevel(Level *level);
