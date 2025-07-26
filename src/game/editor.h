#pragma once

#include "stdbool.h"
#include "raylib.h"
#include "raymath.h"
#include "../game_state.h"
#include "../globals.h"
#include "raygui.h"
#include <stdio.h>
#include "gui_image_button.h"

void setEditorActive(bool active);

void updateEditor(GameState *gameState);
