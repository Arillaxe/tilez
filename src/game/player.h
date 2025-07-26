#pragma once

#include "raylib.h"
#include "level.h"

typedef struct Player
{
  Vector2 position;
  Vector2 velocity;
  Rectangle bounds;
  bool onGround;
  bool facingRight;
  bool isJumping;
  float speed;
  float jumpForce;
  float gravity;
} Player;

Player createPlayer();

void UpdatePlayer(Player *player, float delta, Level *level);

bool IsTileSolid(Level *level, int x, int y);

void UpdatePlayerBounds(Player *player);

void ResolveCollisionsX(Player *player, Level *level);

void ResolveCollisionsY(Player *player, Level *level);

void DrawPlayer(Player *player, Texture2D *tileset);

void DrawPlayerDebug(Player *player);

void DrawDebugGrid(Level *level);
