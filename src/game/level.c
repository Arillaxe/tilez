#include "level.h"

void generateLevel(Level *level)
{
  // Clear level
  for (int y = 0; y < 16; ++y)
    for (int x = 0; x < 16; ++x)
      level->tiles[y][x] = TILE_VOID;

  // Outer walls
  for (int y = 0; y < 16; ++y)
  {
    level->tiles[y][0] = TILE_GROUND;
    level->tiles[y][16 - 1] = TILE_GROUND;
  }

  for (int x = 0; x < 16; ++x)
  {
    level->tiles[0][x] = TILE_GROUND;
    level->tiles[16 - 1][x] = TILE_GROUND;
  }

  level->generated = 1;
}
