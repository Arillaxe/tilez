#include "level.h"
#include "memory.h"
#include "raylib.h"
#include "tile_rules.h"
#include "../game_state.h"
#include "rlgl.h"
#include <stdio.h>

void initLevel(Level *level)
{
  level->levelTexture = LoadRenderTexture(MAP_SIZE * BLOCK_SIZE, MAP_SIZE * BLOCK_SIZE);

  for (int i = 0; i < MAP_SIZE; i++)
  {
    for (int j = 0; j < MAP_SIZE; j++)
    {
      level->tiles[i][j] = i < 10 && j < 10 ? TILE_VOID : TILE_GROUND;
    }
  }

  return;

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

void UpdateTileRender(GameState *gameState, int tileX, int tileY)
{
  BeginTextureMode(gameState->level.levelTexture);

  for (int dx = -1; dx <= 1; dx++)
  {
    for (int dy = -1; dy <= 1; dy++)
    {
      int x = tileX + dx;
      int y = tileY + dy;

      if (x < 0 || y < 0 || x >= MAP_SIZE || y >= MAP_SIZE)
        continue;

      int tile = gameState->level.tiles[x][y];
      Rectangle dest = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};

      if (tile == TILE_VOID)
      {
        if (tile == TILE_VOID)
        {
          rlSetBlendMode(RL_BLEND_CUSTOM_SEPARATE);
          rlSetBlendFactorsSeparate(
              RL_ZERO, RL_ZERO,        // RGB source/destination factors
              RL_ZERO, RL_ZERO,        // Alpha source/destination factors
              RL_FUNC_ADD, RL_FUNC_ADD // Blend equations (RGB, Alpha)
          );
          DrawRectangleRec(dest, BLANK);  // Overwrite with transparency
          rlSetBlendMode(RL_BLEND_ALPHA); // Reset blending
        }
      }
      else if (tile == TILE_GROUND)
      {
        Rectangle src = getTileSourceRect(gameState->tileRulesHashMap, &gameState->level, x, y);
        DrawTexturePro(gameState->tilesetTexture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
      }
      else if (tile == TILE_PORTAL)
      {
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){0, 48, 8, 8}, dest, (Vector2){0, 0}, 0.0f, WHITE);
      }
      else if (tile == TILE_AETHER_PORTAL)
      {
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){16, 48, 8, 8}, dest, (Vector2){0, 0}, 0.0f, WHITE);
      }
      else if (tile == TILE_KEY)
      {
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){8, 48, 8, 8}, dest, (Vector2){0, 0}, 0.0f, WHITE);
      }
    }
  }

  EndTextureMode();
}

void InitLevelTexture(GameState *gameState)
{
  BeginTextureMode(gameState->level.levelTexture);
  ClearBackground(BLANK);

  for (int i = 0; i < MAP_SIZE; i++)
  {
    for (int j = 0; j < MAP_SIZE; j++)
    {
      TILE_TYPE tile = gameState->level.tiles[i][j];
      if (tile == TILE_VOID)
        continue;

      Rectangle dest = {i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
      if (tile == TILE_GROUND)
        DrawTexturePro(gameState->tilesetTexture, getTileSourceRect(gameState->tileRulesHashMap, &gameState->level, i, j), dest, (Vector2){0, 0}, 0.0f, WHITE);
      else if (tile == TILE_PORTAL)
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){0, 48, 8, 8}, dest, (Vector2){0, 0}, 0.0f, WHITE);
      else if (tile == TILE_AETHER_PORTAL)
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){16, 48, 8, 8}, dest, (Vector2){0, 0}, 0.0f, WHITE);
      else if (tile == TILE_KEY)
        DrawTexturePro(gameState->tilesetTexture, (Rectangle){8, 48, 8, 8}, dest, (Vector2){0, 0}, 0.0f, WHITE);
    }
  }

  EndTextureMode();
}

void DrawLevel(GameState *gameState)
{
  DrawTextureRec(gameState->level.levelTexture.texture,
                 (Rectangle){0, 0, MAP_SIZE * BLOCK_SIZE, -MAP_SIZE * BLOCK_SIZE}, // negative height flips vertically
                 (Vector2){0, 0},
                 WHITE);
}
