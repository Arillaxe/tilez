#include "game_state.h"

static GameState gameState;

GameState *getGameState()
{
  return &gameState;
}
