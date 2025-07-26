#include "ui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "../globals.h"

void drawUI()
{
  GuiPanel((Rectangle){SCREEN_WIDTH - 210, 10, 200, SCREEN_HEIGHT - 20}, "Test panel");
}
