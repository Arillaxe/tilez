#include "player.h"

Player createPlayer()
{
  Player player = {
      .position = {100, 100},
      .velocity = {0, 0},
      .bounds = {100, 100, 32, 32}, // Player size 32x32
      .onGround = false,
      .facingRight = true,
      .isJumping = false,
      .speed = 200.0f,
      .jumpForce = -350.0f,
      .gravity = 800.0f};

  return player;
}

void UpdatePlayer(Player *player, float delta, Level *level)
{
  // Horizontal movement
  if (IsKeyDown(KEY_RIGHT))
  {
    player->velocity.x = player->speed;
    player->facingRight = true;
  }
  else if (IsKeyDown(KEY_LEFT))
  {
    player->velocity.x = -player->speed;
    player->facingRight = false;
  }
  else
  {
    player->velocity.x = 0;
  }

  // Jump
  if (IsKeyPressed(KEY_SPACE) && player->onGround)
  {
    player->velocity.y = player->jumpForce;
    player->onGround = false;
    player->isJumping = true;
  }

  // Apply gravity
  player->velocity.y += player->gravity * delta;

  // Move X
  player->position.x += player->velocity.x * delta;
  UpdatePlayerBounds(player);
  ResolveCollisionsX(player, level);

  // Move Y
  player->position.y += player->velocity.y * delta;
  UpdatePlayerBounds(player);
  ResolveCollisionsY(player, level);
}

int GetTileIndex(float pos)
{
  return (int)floorf(pos / BLOCK_SIZE);
}

bool IsTileSolid(Level *level, int x, int y)
{
  if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
    return false;
  return (level->tiles[x][y] == TILE_GROUND); // x first, y second
}

void ResolveCollisionsX(Player *player, Level *level)
{
  int top = (int)(player->bounds.y / BLOCK_SIZE);
  int bottom = (int)((player->bounds.y + player->bounds.height - 1) / BLOCK_SIZE);

  if (player->velocity.x > 0)
  { // Moving right
    int right = (int)((player->bounds.x + player->bounds.width) / BLOCK_SIZE);
    for (int y = top; y <= bottom; y++)
    {
      if (IsTileSolid(level, right, y))
      {
        player->position.x = right * BLOCK_SIZE - player->bounds.width;
        player->velocity.x = 0;
        break;
      }
    }
  }
  else if (player->velocity.x < 0)
  { // Moving left
    int left = (int)(player->bounds.x / BLOCK_SIZE);
    for (int y = top; y <= bottom; y++)
    {
      if (IsTileSolid(level, left, y))
      {
        player->position.x = (left + 1) * BLOCK_SIZE;
        player->velocity.x = 0;
        break;
      }
    }
  }
  UpdatePlayerBounds(player);
}

void ResolveCollisionsY(Player *player, Level *level)
{
  int left = (int)(player->bounds.x / BLOCK_SIZE);
  int right = (int)((player->bounds.x + player->bounds.width - 1) / BLOCK_SIZE);

  player->onGround = false;

  if (player->velocity.y > 0)
  { // Falling
    int bottom = (int)((player->bounds.y + player->bounds.height) / BLOCK_SIZE);
    for (int x = left; x <= right; x++)
    {
      if (IsTileSolid(level, x, bottom))
      {
        player->position.y = bottom * BLOCK_SIZE - player->bounds.height;
        player->velocity.y = 0;
        player->onGround = true;
        break;
      }
    }
  }
  else if (player->velocity.y < 0)
  { // Jumping
    int top = (int)(player->bounds.y / BLOCK_SIZE);
    for (int x = left; x <= right; x++)
    {
      if (IsTileSolid(level, x, top))
      {
        player->position.y = (top + 1) * BLOCK_SIZE;
        player->velocity.y = 0;
        break;
      }
    }
  }
  UpdatePlayerBounds(player);
}

void UpdatePlayerBounds(Player *player)
{
  player->bounds.x = player->position.x;
  player->bounds.y = player->position.y;
}

void DrawDebugGrid(Level *level)
{
  // Draw grid lines
  for (int x = 0; x <= MAP_SIZE; x++)
  {
    DrawLine(x * BLOCK_SIZE, 0, x * BLOCK_SIZE, MAP_SIZE * BLOCK_SIZE, Fade(BLUE, 0.3f));
  }
  for (int y = 0; y <= MAP_SIZE; y++)
  {
    DrawLine(0, y * BLOCK_SIZE, MAP_SIZE * BLOCK_SIZE, y * BLOCK_SIZE, Fade(BLUE, 0.3f));
  }

  // Draw solid tiles
  for (int x = 0; x < MAP_SIZE; x++)
  {
    for (int y = 0; y < MAP_SIZE; y++)
    {
      if (level->tiles[x][y] == TILE_GROUND)
      {
        DrawRectangleLines(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, RED);
      }
    }
  }
}

void DrawPlayerDebug(Player *player)
{
  DrawRectangleLines(player->bounds.x, player->bounds.y, player->bounds.width, player->bounds.height, GREEN);
}

void DrawPlayer(Player *player, Texture2D *tileset)
{
  DrawTexturePro(*tileset, (Rectangle){8 * 0, 8 * 7, 8, 8}, player->bounds, (Vector2){0}, 0, WHITE);

  // DrawRectangleRec(player->bounds, RED); // simple rectangle for now
}
