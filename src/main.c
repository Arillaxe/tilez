#include "game_loader.h"
#include "game/game.h"
#include <raylib.h>

int main()
{
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(800, 400, "FPS Test");
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    loadGameLib();

    tickFuncT gameTick = getGameTickFunc();

    if (gameTick)
    {
      gameTick();
    }
  }

  CloseWindow();

  unloadGameLib();

  return 0;
}
