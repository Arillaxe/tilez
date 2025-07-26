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
  Camera2D playerCamera;
  Camera2D editorCamera;
  Player player;
  TILE_TYPE currentEditorTileBrush;
  bool editorEnabled;
  Vector2 prevMousePos;
} GameState;

GameState *getGameState();
