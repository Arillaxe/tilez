#include "editor.h"

static bool isActive = false;

void setEditorActive(bool active)
{
  isActive = active;
}

static Vector2 prevMouse = {0};

void updateEditor(GameState *gameState)
{
  if (IsKeyPressed(KEY_F1))
  {
    isActive = !isActive;
  }

  if (!isActive)
  {
    return;
  }

  // camera panning

  if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
    prevMouse = GetMousePosition();

  if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON))
  {
    Vector2 mouse = GetMousePosition();
    Vector2 delta = Vector2Subtract(prevMouse, mouse);
    gameState->camera.target = Vector2Add(gameState->camera.target, Vector2Scale(delta, 1.0f / gameState->camera.zoom));
    prevMouse = mouse;
  }
  // --------------

  // placing blocks

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
  {
    Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameState->camera);
    int x = floor(worldPos.x / BLOCK_SIZE);
    int y = floor(worldPos.y / BLOCK_SIZE);

    if (!(x < 0 || y < 0 || x > 255 || y > 255))
    {
      gameState->level.tiles[x][y] = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? TILE_GROUND : TILE_VOID;
    }
  }

  drawEditor();
}

void drawEditor()
{
  DrawText("Editor mode", SCREEN_WIDTH - 150, 10, 24, WHITE);
}
