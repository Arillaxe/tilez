#include "game_loader.h"
#include <shlwapi.h>

static tickFuncT gameTick = NULL;
static initFuncT initGame = NULL;
static time_t lastModTime = 0;

#ifdef WINDOWS
static HMODULE gameLib = NULL;
#elif defined MACOS
static void *handle = NULL;
#endif

time_t getFileModTime(const char *path)
{
#ifdef WINDOWS
  WIN32_FILE_ATTRIBUTE_DATA fileInfo;
  if (!GetFileAttributesEx(path, GetFileExInfoStandard, &fileInfo))
  {
    return (time_t)(-1);
  }

  ULARGE_INTEGER ull;
  ull.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
  ull.HighPart = fileInfo.ftLastWriteTime.dwHighDateTime;

  return (time_t)((ull.QuadPart / 10000000ULL) - 11644473600ULL);
#elif defined MACOS
  struct stat attr;
  if (stat(path, &attr) == 0)
    return attr.st_mtime;
  return 0;
#endif
}

void loadGameLib()
{
#ifdef WINDOWS
  char exePath[MAX_PATH];
  GetModuleFileName(NULL, exePath, MAX_PATH);
  PathRemoveFileSpec(exePath);

  char dllPath[MAX_PATH];
  char dllCopyPath[MAX_PATH];

  snprintf(dllPath, sizeof(dllPath), "%s\\%s", exePath, "libgamelib.dll");
  snprintf(dllCopyPath, sizeof(dllCopyPath), "%s\\%s", exePath, "libgamelib-copy.dll");

#elif defined MACOS
  static const char *dllPath = "./out/libgamelib.dylib";
#endif
  time_t currentModTime = getFileModTime(dllPath);

  if (currentModTime <= 0)
  {
    return;
  }

  if (currentModTime != lastModTime)
  {
#ifdef WINDOWS
    if (gameLib)
    {
      FreeLibrary(gameLib);
      gameLib = NULL;
      gameTick = NULL;
      initGame = NULL;
    }

    Sleep(100);

    CopyFile(dllPath, dllCopyPath, FALSE);

    Sleep(100);

    gameLib = LoadLibrary(dllCopyPath);

    if (!gameLib)
    {
      printf("Failed to load DLL");
      exit(1);
    }

    gameTick = (tickFuncT)GetProcAddress(gameLib, "gameTick");
    initGame = (initFuncT)GetProcAddress(gameLib, "initGame");

    lastModTime = currentModTime;
    printf("Reloaded DLL at %lld\n", lastModTime);
#elif defined MACOS
    if (handle)
    {
      dlclose(handle);
      handle = NULL;
      gameTick = NULL;
      initGame = NULL;
    }

    handle = dlopen(dllPath, RTLD_LAZY);
    if (!handle)
    {
      fprintf(stderr, "dlopen error: %s\n", dlerror());
      exit(1);
    }
    else
    {
      gameTick = (tickFuncT)dlsym(handle, "gameTick");
      initGame = (tickFuncT)dlsym(handle, "initGame");
      char *error = dlerror();
      if (error != NULL)
      {
        fprintf(stderr, "dlsym error: %s\n", error);
        dlclose(handle);
        handle = NULL;
        gameTick = NULL;
        initGame = NULL;
      }
      else
      {
        lastModTime = currentModTime;
        printf("Reloaded DLL at %ld\n", lastModTime);
      }
    }
#endif
  }
}

void unloadGameLib()
{
#ifdef WINDOWS
  if (gameLib)
  {
    FreeLibrary(gameLib);
    gameLib = NULL;
    gameTick = NULL;
    initGame = NULL;
  }
#elif defined MACOS
  dlclose(handle);
  handle = NULL;
  gameTick = NULL;
  initGame = NULL;
#endif
}

tickFuncT getGameTickFunc()
{
  return gameTick;
}

initFuncT getInitGameFunc()
{
  return initGame;
}
