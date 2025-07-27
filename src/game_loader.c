#include "game_loader.h"
#include "platform/platform.h"
#include <stdio.h>

static time_t lastModeTime = 0;
static char exePath[1024];
static LibHandle gameLibHandle;
static GameFunctions gameFunctions;

void loaderInit()
{
  getExecutableDirectory(exePath, 1024);
  snprintf(exePath, sizeof(exePath), "%s\\%s", exePath, getGameLibraryName());

  // initial load
  loaderUpdate();
}

void loaderUpdate()
{
  time_t currentModTime = getFileModTime(exePath);

  if (currentModTime == lastModeTime)
  {
    return;
  }

  if (gameLibHandle)
  {
    unloadLibrary(gameLibHandle);
  }

  gameLibHandle = loadLibrary(exePath);

  if (!gameLibHandle)
  {
    printf("Failed to load game library\n");
    exit(1);
  }

  gameFunctions.init = (InitFuncT)getLibraryFunction(gameLibHandle, "init");
  gameFunctions.tick = (TickFuncT)getLibraryFunction(gameLibHandle, "tick");
}

void loaderCleanUp()
{
  unloadLibrary(gameLibHandle);
}

GameFunctions *getGameFunctions()
{
  return &gameFunctions;
}
