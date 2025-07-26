#pragma once

#include <raylib.h>
#include "stdio.h"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#elif
#define EXPORT
#endif

#include "ui.h"
#include "../game_state.h"
#include "editor.h"

void init(GameState *gameState);

void tick(GameState *gameState);
