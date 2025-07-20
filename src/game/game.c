#include "game.h"

EXPORT void initGame(GameState *gameState)
{
  initTileRulesHashMap(&gameState->tileRulesHashMap);
  initLevel(&gameState->level);

  gameState->playerCamera.target = (Vector2){0.0f, 0.0f};
  gameState->playerCamera.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
  gameState->playerCamera.rotation = 0.0f;
  gameState->playerCamera.zoom = 2.0f;

  gameState->editorCamera.target = (Vector2){0.0f, 0.0f};
  gameState->editorCamera.offset = (Vector2){0.0f, 0.0f};
  gameState->editorCamera.rotation = 0.0f;
  gameState->editorCamera.zoom = 1.0f;

  gameState->player = createPlayer();

  gameState->currentEditorTileBrush = TILE_GROUND;
}

EXPORT void gameTick(GameState *gameState)
{
  BeginDrawing();
  ClearBackground(BLACK);

  BeginMode2D(gameState->editorEnabled ? gameState->editorCamera : gameState->playerCamera);

  DrawLevel(gameState);

  float deltaTime = GetFrameTime();
  UpdatePlayer(&gameState->player, deltaTime > 0.05f ? 0.05f : deltaTime, &gameState->level);

  DrawPlayer(&gameState->player, &gameState->tilesetTexture);

  // Camera follow
  float roomW = SCREEN_WIDTH / gameState->playerCamera.zoom;
  float roomH = SCREEN_HEIGHT / gameState->playerCamera.zoom;

  int roomX = (int)floorf(gameState->player.position.x / roomW);
  int roomY = (int)floorf(gameState->player.position.y / roomH);

  gameState->playerCamera.target.x = roomX * roomW + roomW / 2.0f;
  gameState->playerCamera.target.y = roomY * roomH + roomH / 2.0f;

  // DrawDebugGrid(&gameState->level);
  // DrawPlayerDebug(&gameState->player);

  EndMode2D();

  DrawFPS(10, 10);

  updateEditor(gameState);

  DrawText("Press F1 to toggle editor mode", SCREEN_WIDTH - 380, SCREEN_HEIGHT - 30, 24, WHITE);

  // drawUI();

  EndDrawing();
}
