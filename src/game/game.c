#include "game.h"

EXPORT void gameTick()
{
  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawFPS(10, 10);

  DrawText("Yo wazzup 123", 10, 30, 24, RED);

  drawUI();

  EndDrawing();
}
