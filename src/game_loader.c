#include "game_loader.h"

static tickFuncT gameTick = NULL;
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
  static const char *dllPath = "./out/libgamelib.dll";
#elif defined MACOS
  static const char *dllPath = "./out/libgamelib.dylib";
#endif
  time_t currentModTime = getFileModTime(dllPath);

  if (currentModTime == 0)
  {
    fprintf(stderr, "Failed to get mod time for %s\n", dllPath);
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
    }

    gameLib = LoadLibrary(dllPath);

    if (!gameLib)
    {
      printf("Failed to load DLL\n");
      exit(1);
    }

    gameTick = (tickFuncT)GetProcAddress(gameLib, "gameTick");

    lastModTime = currentModTime;
    printf("Reloaded DLL at %ld\n", lastModTime);
#elif defined MACOS
    if (handle)
    {
      dlclose(handle);
      handle = NULL;
      gameTick = NULL;
    }

    handle = dlopen(dll_path, RTLD_LAZY);
    if (!handle)
    {
      fprintf(stderr, "dlopen error: %s\n", dlerror());
      exit(1);
    }
    else
    {
      gameTick = (tickFuncT)dlsym(handle, "gameTick");
      char *error = dlerror();
      if (error != NULL)
      {
        fprintf(stderr, "dlsym error: %s\n", error);
        dlclose(handle);
        handle = NULL;
        gameTick = NULL;
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
  }
#elif defined MACOS
  dlclose(handle);
  handle = NULL;
  gameTick = NULL;
#endif
}

tickFuncT getGameTickFunc()
{
  return gameTick;
}
