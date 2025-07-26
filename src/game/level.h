#pragma once

#include "raylib.h"

#define MAP_SIZE 256
#define BLOCK_SIZE 32

typedef struct GameState GameState;

typedef enum TILE_TYPE
{
  TILE_VOID,
  TILE_GROUND,
  TILE_PORTAL,
  TILE_AETHER_PORTAL,
  TILE_KEY,
  TILE_LENGTH,
} TILE_TYPE;

typedef struct Level
{
  TILE_TYPE tiles[MAP_SIZE][MAP_SIZE];
  RenderTexture2D levelTexture;
} Level;

void initLevel(Level *level);

void UpdateTileRender(GameState *gameState, int tileX, int tileY);

void InitLevelTexture(GameState *gameState);

void DrawLevel(GameState *gameState);
