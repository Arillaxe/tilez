#include "game.h"

void tick()
{
  BeginDrawing();
  ClearBackground(RAYWHITE);

  DrawFPS(10, 10);

  DrawText("Yo wazzup", 10, 30, 24, RED);

  EndDrawing();
}
