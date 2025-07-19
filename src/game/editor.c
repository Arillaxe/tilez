#include "editor.h"

static Vector2 prevMouse = {0};

void updateEditor(GameState *gameState)
{
  if (IsKeyPressed(KEY_F1))
  {
    gameState->editorEnabled = !gameState->editorEnabled;
  }

  if (!gameState->editorEnabled)
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
    gameState->editorCamera.target = Vector2Add(gameState->editorCamera.target, Vector2Scale(delta, 1.0f / gameState->editorCamera.zoom));
    prevMouse = mouse;
  }
  // --------------

  Rectangle buttons[] = {
      {SCREEN_WIDTH - 160, 40, 70, 30}, // Open
      {SCREEN_WIDTH - 80, 40, 70, 30},  // Save
  };

  bool mouseOverUI = false;
  Vector2 mouse = GetMousePosition();

  for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
  {
    if (GuiButton(buttons[i], (i == 0) ? "#1#Open" : "#2#Save"))
    {
      // Handle button click
    }

    if (CheckCollisionPointRec(mouse, buttons[i]))
      mouseOverUI = true;
  }

  if (GuiButton(buttons[0], "#1#Open"))
  {
    const char *filename = openFileDialog();
    FILE *file = fopen(filename, "rb");

    if (file)
    {
      fread(&gameState->level.tiles, sizeof(TILE_TYPE), MAP_SIZE * MAP_SIZE, file);
      fclose(file);
    }
  }

  if (GuiButton(buttons[1], "#2#Save"))
  {
    const char *filename = saveFileDialog();
    FILE *file = fopen(filename, "wb");

    if (file)
    {
      fwrite(&gameState->level.tiles, sizeof(TILE_TYPE), MAP_SIZE * MAP_SIZE, file);
      fclose(file);
    }
  }

  // placing blocks

  if (!mouseOverUI && (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)))
  {
    Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameState->editorCamera);
    int x = floor(worldPos.x / BLOCK_SIZE);
    int y = floor(worldPos.y / BLOCK_SIZE);

    if (!(x < 0 || y < 0 || x > 255 || y > 255))
    {
      gameState->level.tiles[x][y] = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? TILE_GROUND : TILE_VOID;
    }
  }
  // ------------

  // camera zoom scrollwheel

  float wheel = GetMouseWheelMove();

  if (wheel != 0.0f)
  {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), gameState->editorCamera);

    float zoomIncrement = 1.0f + (wheel * 0.1f);
    gameState->editorCamera.zoom *= zoomIncrement;

    if (gameState->editorCamera.zoom < 0.1f)
      gameState->editorCamera.zoom = 0.1f;
    if (gameState->editorCamera.zoom > 3.0f)
      gameState->editorCamera.zoom = 3.0f;

    Vector2 newMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), gameState->editorCamera);
    gameState->editorCamera.target = Vector2Add(gameState->editorCamera.target, Vector2Subtract(mouseWorldPos, newMouseWorldPos));
  }

  // -----------------------

  DrawText("Editor mode", SCREEN_WIDTH - 150, 10, 24, WHITE);
}
