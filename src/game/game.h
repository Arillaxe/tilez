#pragma once

#include "../game_state.h"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#elif
#define EXPORT
#endif

void init(GameState *gameState);

void tick(GameState *gameState);
