#pragma once

#include "stdbool.h"
#include "raylib.h"
#include "raymath.h"
#include "../game_state.h"
#include "../globals.h"
#include "raygui.h"

void setEditorActive(bool active);

void updateEditor(GameState *gameState);
