#pragma once

#include "game/level.h"
#include "raylib.h"
#include "game/tile_rules.h"

typedef struct GameState
{
  Texture2D tilesetTexture;
  Level level;
  TileRule *tileRulesHashMap;
  Camera2D camera;
  bool editorEnabled;
} GameState;

GameState *getGameState();
