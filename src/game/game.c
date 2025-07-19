#include "game.h"

EXPORT void initGame(GameState *gameState)
{
  initTileRulesHashMap(&gameState->tileRulesHashMap);
  initLevel(&gameState->level);

  gameState->playerCamera.target = (Vector2){0.0f, 0.0f};
  gameState->playerCamera.offset = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
  gameState->playerCamera.rotation = 0.0f;
  gameState->playerCamera.zoom = 1.5f;

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
  gameState->playerCamera.target = gameState->player.position;

  // DrawDebugGrid(&gameState->level);
  // DrawPlayerDebug(&gameState->player);

  EndMode2D();

  DrawFPS(10, 10);

  updateEditor(gameState);

  // drawUI();

  EndDrawing();
}
