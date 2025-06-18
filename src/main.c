#include "game_loader.h"
#include "game/game.h"
#include <raylib.h>
#include "globals.h"

#include "box2d/box2d.h"

int main()
{
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FPS Test");
  SetTargetFPS(60);

  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = (b2Vec2){0.0f, 10.0f};
  b2WorldId worldId = b2CreateWorld(&worldDef);

  b2BodyDef groundBodyDef = b2DefaultBodyDef();
  groundBodyDef.position = (b2Vec2){0.0f, 100.0f};

  b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);

  b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);

  b2ShapeDef groundShapeDef = b2DefaultShapeDef();
  b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

  b2BodyDef bodyDef = b2DefaultBodyDef();
  bodyDef.type = b2_dynamicBody;
  bodyDef.position = (b2Vec2){0.0f, 50.0f};
  b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

  b2Polygon dynamicBox = b2MakeBox(10.0f, 10.0f);

  b2ShapeDef shapeDef = b2DefaultShapeDef();
  shapeDef.density = 1.0f;
  shapeDef.material.friction = 0.3f;

  b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

  float timeStep = 1.0f / 60.0f;
  int subStepCount = 40;

  while (!WindowShouldClose())
  {
    loadGameLib();

    tickFuncT gameTick = getGameTickFunc();

    if (gameTick)
    {
      gameTick();
    }

    b2World_Step(worldId, GetFrameTime(), subStepCount);
    b2Vec2 position = b2Body_GetPosition(bodyId);
    b2Rot rotation = b2Body_GetRotation(bodyId);

    b2Vec2 positionGround = b2Body_GetPosition(groundId);

    DrawRectangle(positionGround.x + 50, positionGround.y + 50, 100, 20, GRAY);
    DrawRectangle(position.x + 50, position.y + 50, 20, 20, BLUE);

    if (IsKeyDown(KEY_D))
    {
      b2Body_ApplyLinearImpulse(bodyId, (b2Vec2){100.0f, 0}, (b2Vec2){position.x, position.y}, true);
    }
  }

  CloseWindow();

  unloadGameLib();

  return 0;
}
