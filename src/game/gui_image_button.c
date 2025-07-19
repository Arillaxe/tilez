#include "gui_image_button.h"

bool GuiImageButton(Rectangle bounds, Texture2D texture, Rectangle srcRect)
{
  bool clicked = CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

  DrawTexturePro(texture, srcRect, bounds, (Vector2){0}, 0.0f, WHITE);

  DrawRectangleLinesEx(bounds, 2, GRAY);

  if (CheckCollisionPointRec(GetMousePosition(), bounds))
  {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
      DrawRectangleRec(bounds, Fade(BLACK, 0.3f));
    else
      DrawRectangleRec(bounds, Fade(BLACK, 0.1f));
  }

  return clicked;
}
