#include "level.h"
#include "memory.h"
#include "raylib.h"
#include "tile_rules.h"
#include "../game_state.h"

void initLevel(Level *level)
{
  FILE *file = fopen("./default.lvl", "rb");

  if (file)
  {
    fread(level->tiles, sizeof(TILE_TYPE), MAP_SIZE * MAP_SIZE, file);
    fclose(file);

    return;
  }

  memset(level->tiles, 0, sizeof(level->tiles));

  for (int x = 0; x < 10; x++)
  {
    level->tiles[0][x] = TILE_GROUND;
    level->tiles[9][x] = TILE_GROUND;
  }

  for (int y = 0; y < 10; y++)
  {
    level->tiles[y][0] = TILE_GROUND;
    level->tiles[y][9] = TILE_GROUND;
  }
}

void DrawLevel(GameState *gameState)
{
  Level level = gameState->level;

  for (int i = 0; i < MAP_SIZE; i++)
  {
    for (int j = 0; j < MAP_SIZE; j++)
    {
      if (level.tiles[i][j] == TILE_GROUND)
      {
        Rectangle srcRect = getTileSourceRect(gameState->tileRulesHashMap, &level, i, j);

        DrawTexturePro(gameState->tilesetTexture, srcRect, (Rectangle){i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}, (Vector2){0, 0}, 0.0f, WHITE);
      }
      else if (level.tiles[i][j] == TILE_PORTAL)
      {
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){8 * 0, 8 * 6, 8, 8}, (Rectangle){i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}, (Vector2){0, 0}, 0.0f, WHITE);
      }
      else if (level.tiles[i][j] == TILE_AETHER_PORTAL)
      {
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){8 * 2, 8 * 6, 8, 8}, (Rectangle){i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}, (Vector2){0, 0}, 0.0f, WHITE);
      }
      else if (level.tiles[i][j] == TILE_KEY)
      {
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){8 * 1, 8 * 6, 8, 8}, (Rectangle){i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}, (Vector2){0, 0}, 0.0f, WHITE);
      }
    }
  }
}
