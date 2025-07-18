#include "game_loader.h"
#include "game/game.h"
#include <raylib.h>
#include "globals.h"

#include "game_state.h"

int main()
{
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FPS Test");
  // SetWindowMonitor(1);
  // SetTargetFPS(60);

  GameState *gameState = getGameState();

  gameState->tilesetTexture = LoadTexture("./resources/tiles_ramazan.png");

  loadGameLib();

  initFuncT initGame = getInitGameFunc();

  initGame(gameState);

  while (!WindowShouldClose())
  {
    loadGameLib();

    tickFuncT gameTick = getGameTickFunc();

    if (gameTick)
    {
      gameTick(gameState);
    }
  }

  unloadGameLib();

  CloseWindow();

  return 0;
}
