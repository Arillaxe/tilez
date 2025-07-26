#pragma once

#include "game_state.h"

typedef void (*TickFuncT)(GameState *gameState);
typedef void (*InitFuncT)(GameState *gameState);

typedef struct GameFunctions
{
  InitFuncT init;
  TickFuncT tick;
} GameFunctions;

void loaderInit();

void loaderUpdate();

void loaderCleanUp();

GameFunctions *getGameFunctions();
