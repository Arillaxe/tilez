#include "game_loader.h"
#include "game/game.h"
#include <raylib.h>
#include "globals.h"

int main()
{
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FPS Test");
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

  unloadGameLib();

  CloseWindow();

  return 0;
}
