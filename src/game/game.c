#include "game.h"

#define TILE_SIZE 8
#define ATLAS_WIDTH 64
#define TILES_PER_ROW (ATLAS_WIDTH / TILE_SIZE)

#include <stdint.h>

typedef struct
{
  uint8_t index; // sprite sheet index
  uint8_t mask;  // required bits
} TileRule;

#define TILE_RULE_COUNT 47

void print_bitset(uint8_t mask)
{
  for (int i = 7; i >= 0; i--)
  {
    printf("%c", (mask & (1 << i)) ? '1' : '0');
  }
}

const TileRule tileRules[TILE_RULE_COUNT] = {
    {0, 0b00000000},
    {1, 0b00000010},
    {2, 0b00001000},
    {3, 0b00000010 | 0b00001000},
    {4, 0b00010000},
    {5, 0b00000100},
    {6, 0b00000001},
    {7, 0b00000010 | 0b00010000},
    {8, 0b00000010 | 0b00000100},
    {9, 0b00000010 | 0b00000001},
    {10, 0b00001000 | 0b00100000},
    {11, 0b00100000},
    {12, 0b00001000 | 0b01000000},
    {13, 0b01000000},
    {14, 0b00010000 | 0b10000000},
    {15, 0b10000000},
    {16, 0b00010000 | 0b01000000},
    {17, 0b00001000 | 0b00010000},
    {18, 0b00001000 | 0b00010000 | 0b00000010},
    {19, 0b00001000 | 0b00010000 | 0b01000000},
    {4, 0b00000010 | 0b00001000 | 0b01000000},
    {21, 0b00000010 | 0b00010000 | 0b01000000},
    {22, 0b00000010 | 0b01000000},
    {23, 0b00001000 | 0b01000000},
    {24, 0b00010000 | 0b01000000},
    {25, 0b00000010 | 0b00001000},
    {26, 0b00000010 | 0b00010000},
    {27, 0b00001000 | 0b00010000},
    {28, 0b01000000 | 0b10000000},
    {29, 0b01000000 | 0b00100000},
    {30, 0b00000001 | 0b00000010 | 0b00000100},
    {31, 0b00100000 | 0b01000000 | 0b10000000},
    {32, 0b00000001 | 0b00001000},
    {33, 0b00000100 | 0b00010000},
    {34, 0b00000010 | 0b00000100 | 0b00000001},
    {35, 0b00000010 | 0b00001000 | 0b00000100},
    {36, 0b00000010 | 0b00001000 | 0b00000001},
    {37, 0b00001000 | 0b01000000 | 0b00100000},
    {38, 0b00010000 | 0b01000000 | 0b10000000},
    {39, 0b00001000 | 0b01000000 | 0b10000000},
    {40, 0b00010000 | 0b01000000 | 0b00100000},
    {41, 0b00000010 | 0b01000000 | 0b10000000},
    {42, 0b00000010 | 0b01000000 | 0b00100000},
    {43, 0b00000010 | 0b00001000 | 0b00010000 | 0b01000000},
    {44, 0b11111111},
    {45, 0b00001000 | 0b00010000 | 0b01000000},
    {46, 0b00001000 | 0b00010000 | 0b00000010}};

uint8_t get_tile_index(uint8_t mask)
{
  for (int i = 0; i < TILE_RULE_COUNT; ++i)
  {
    uint8_t pattern = tileRules[i].mask;

    // Match if all required bits in pattern are present in mask
    if ((mask | pattern) == pattern)
    {
      print_bitset(mask);
      printf("\n%d\n", tileRules[i].index);
      return tileRules[i].index;
    }
  }

  return 0; // default if no pattern matched
}

Rectangle getTileSourceRect(int index)
{
  return (Rectangle){
      .x = (index % TILES_PER_ROW) * TILE_SIZE,
      .y = (index / TILES_PER_ROW) * TILE_SIZE,
      .width = TILE_SIZE,
      .height = TILE_SIZE};
}

uint8_t getTileBitmask(Level *level, int x, int y)
{
  const TILE_TYPE center = level->tiles[y][x];
  uint8_t mask = 0;

  const int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
  const int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
  const uint8_t bits[8] = {1, 2, 4, 8, 16, 32, 64, 128};

  for (int i = 0; i < 8; ++i)
  {
    int nx = x + dx[i];
    int ny = y + dy[i];

    TILE_TYPE neighbor =
        (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE)
            ? level->tiles[ny][nx]
            : TILE_GROUND;

    if (neighbor == center)
    {
      mask |= bits[i];
    }
  }

  return mask;
}

EXPORT void gameTick(GameState *gameState)
{
  BeginDrawing();
  ClearBackground(BLACK);

  DrawFPS(10, 10);

  if (!gameState->level.generated)
  {
    generateLevel(&gameState->level);
  }

  Level level = gameState->level;

  for (int i = 0; i < MAP_SIZE; i++)
  {
    for (int j = 0; j < MAP_SIZE; j++)
    {
      if (level.tiles[i][j] == TILE_GROUND)
      {
        uint8_t mask = getTileBitmask(&level, i, j);
        int index = get_tile_index(mask);
        Rectangle srcRect = getTileSourceRect(index);

        DrawTexturePro(gameState->tilesetTexture, srcRect, (Rectangle){i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}, (Vector2){0, 0}, 0.0f, WHITE);
      }
    }
  }

  // drawUI();

  EndDrawing();
}
