#pragma once

#include "game/level.h"
#include "raylib.h"
#include "game/tile_rules.h"
#include "game/player.h"

typedef struct GameState
{
  Texture2D tilesetTexture;
  Level level;
  TileRule *tileRulesHashMap;
  Camera2D camera;
  Player player;
  bool editorEnabled;
} GameState;

GameState *getGameState();
