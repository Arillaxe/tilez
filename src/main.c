#include "game_loader.h"
#include "game/game.h"
#include <raylib.h>
#include "globals.h"
#include "game_state.h"
#include "platform/platform.h"

int main()
{
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FPS Test");
  // SetWindowMonitor(1);
  SetTargetFPS(GetMonitorRefreshRate(0));

  char texturePath[256];
  buildPath(texturePath, 256, "../resources/tiles_ramazan.png");

  GameState *gameState = getGameState();
  gameState->tilesetTexture = LoadTexture(texturePath);
  SetTextureFilter(gameState->tilesetTexture, TEXTURE_FILTER_POINT);

  loaderInit();
  GameFunctions *gameFunctions = getGameFunctions();

  gameFunctions->init(gameState);

  while (!WindowShouldClose())
  {
    loaderUpdate();

    gameFunctions->tick(gameState);
  }

  loaderCleanUp();
  CloseWindow();

  return 0;
}
