#pragma once

#include "game/level.h"
#include "raylib.h"

typedef struct GameState
{
  Texture2D tilesetTexture;
  Level level;
} GameState;

GameState *getGameState();
