#include "editor.h"
#include "../platform/platform.h"
#include "raylib.h"
#include "raymath.h"
#include "../globals.h"
#include "raygui.h"
#include <stdio.h>
#include "gui_image_button.h"
#include <errno.h>

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
    gameState->prevMousePos = GetMousePosition();

  if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON))
  {
    Vector2 mouse = GetMousePosition();
    Vector2 delta = Vector2Subtract(gameState->prevMousePos, mouse);
    gameState->editorCamera.target = Vector2Add(gameState->editorCamera.target, Vector2Scale(delta, 1.0f / gameState->editorCamera.zoom));
    gameState->prevMousePos = mouse;
  }
  // --------------

  Rectangle buttons[] = {
      {SCREEN_WIDTH - 160, 40, 70, 30},  // Open
      {SCREEN_WIDTH - 80, 40, 70, 30},   // Save
      {SCREEN_WIDTH - 60, 80, 50, 50},   // TILE_GROUND
      {SCREEN_WIDTH - 120, 80, 50, 50},  // TILE_PORTAL
      {SCREEN_WIDTH - 60, 140, 50, 50},  // TILE_AETHER_PORTAL
      {SCREEN_WIDTH - 120, 140, 50, 50}, // TILE_KEY
  };

  bool mouseOverUI = false;
  Vector2 mouse = GetMousePosition();

  for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
  {
    if (CheckCollisionPointRec(mouse, buttons[i]))
      mouseOverUI = true;
  }

  if (GuiButton(buttons[0], "#1#Open"))
  {
    char filename[256];
    openFileDialog(filename, 256);

    FILE *file = fopen(filename, "rb");

    if (file)
    {
      fread(gameState->level.tiles, sizeof(TILE_TYPE), MAP_SIZE * MAP_SIZE, file);
      fclose(file);

      InitLevelTexture(gameState);
    }
    else
    {
      printf("fopen failed: %s\n", strerror(errno));
    }
  }

  if (GuiButton(buttons[1], "#2#Save"))
  {
    char filename[256];
    saveFileDialog(filename, 256);

    FILE *file = fopen(filename, "wb");

    if (file)
    {
      fwrite(gameState->level.tiles, sizeof(TILE_TYPE), MAP_SIZE * MAP_SIZE, file);
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
      gameState->level.tiles[x][y] = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? gameState->currentEditorTileBrush : TILE_VOID;
      UpdateTileRender(gameState, x, y);
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

  // Tile selection

  if (GuiImageButton(buttons[2], gameState->tilesetTexture, (Rectangle){8 * 0, 8 * 3, 8, 8}))
  {
    gameState->currentEditorTileBrush = TILE_GROUND;
  }

  if (GuiImageButton(buttons[3], gameState->tilesetTexture, (Rectangle){8 * 0, 8 * 6, 8, 8}))
  {
    gameState->currentEditorTileBrush = TILE_PORTAL;
  }

  if (GuiImageButton(buttons[4], gameState->tilesetTexture, (Rectangle){8 * 2, 8 * 6, 8, 8}))
  {
    gameState->currentEditorTileBrush = TILE_AETHER_PORTAL;
  }

  if (GuiImageButton(buttons[5], gameState->tilesetTexture, (Rectangle){8 * 1, 8 * 6, 8, 8}))
  {
    gameState->currentEditorTileBrush = TILE_KEY;
  }

  // --------------

  // draw level boundaries

  BeginMode2D(gameState->editorCamera);

  DrawRectangleLines(0, 0, BLOCK_SIZE * MAP_SIZE, BLOCK_SIZE * MAP_SIZE, RED);

  int levelSize = BLOCK_SIZE * MAP_SIZE;
  int roomsX = levelSize / SCREEN_WIDTH * gameState->playerCamera.zoom;
  int roomsY = levelSize / SCREEN_HEIGHT * gameState->playerCamera.zoom;
  int roomWidth = SCREEN_WIDTH / gameState->playerCamera.zoom;
  int roomHeigth = SCREEN_HEIGHT / gameState->playerCamera.zoom;

  for (int i = 1; i < roomsX; i++)
  {
    DrawLine(roomWidth * i, 0, roomWidth * i, levelSize, RED);
  }

  for (int i = 1; i < roomsY; i++)
  {
    DrawLine(0, roomHeigth * i, levelSize, roomHeigth * i, RED);
  }

  EndMode2D();

  // ---------------------

  DrawText("Editor mode", SCREEN_WIDTH - 150, 10, 24, WHITE);
}
