#include "game.h"

EXPORT void initGame(GameState *gameState)
{
  initTileRulesHashMap(&gameState->tileRulesHashMap);
  initLevel(&gameState->level);

  gameState->camera.target = (Vector2){0.0f, 0.0f};
  gameState->camera.offset = (Vector2){0.0f, 0.0f};
  gameState->camera.rotation = 0.0f;
  gameState->camera.zoom = 1.0f;

  gameState->player = createPlayer();
}

EXPORT void gameTick(GameState *gameState)
{
  BeginDrawing();
  ClearBackground(BLACK);

  BeginMode2D(gameState->camera);

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
    }
  }

  float deltaTime = GetFrameTime();
  UpdatePlayer(&gameState->player, deltaTime > 0.05f ? 0.05f : deltaTime, &gameState->level);

  DrawPlayer(&gameState->player, &gameState->tilesetTexture);

  // DrawDebugGrid(&gameState->level);
  // DrawPlayerDebug(&gameState->player);

  EndMode2D();

  DrawFPS(10, 10);

  updateEditor(gameState);

  // drawUI();

  EndDrawing();
}
