#pragma once

#include "raylib.h"
#include "level.h"
#include "math.h"

typedef struct Player
{
  Vector2 position; // The player's position (top-left or center)
  Vector2 velocity; // The player's velocity (x/y speed)
  Rectangle bounds; // Bounding box for collision detection
  bool onGround;    // Whether the player is standing on the ground
  bool facingRight; // Direction the player is facing
  bool isJumping;   // Whether the player is currently jumping
  float speed;      // Horizontal speed
  float jumpForce;  // Jump strength
  float gravity;    // Gravity applied to the player
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
