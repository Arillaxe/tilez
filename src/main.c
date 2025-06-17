#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>
#include <raylib.h>
#include "game/game.h"

typedef void (*tick_func_t)();

time_t get_file_mod_time(const char *path)
{
  struct stat attr;
  if (stat(path, &attr) == 0)
    return attr.st_mtime;
  return 0;
}

int main()
{
  const char *dll_path = "./out/libgamelib.dylib";
  void *handle = NULL;
  tick_func_t tick = NULL;

  time_t last_mod_time = 0;

  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
  InitWindow(800, 400, "FPS Test");
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    time_t current_mod_time = get_file_mod_time(dll_path);

    if (current_mod_time == 0)
    {
      fprintf(stderr, "Failed to get mod time for %s\n", dll_path);
      break;
    }

    if (current_mod_time != last_mod_time)
    {
      // Reload the DLL
      if (handle)
      {
        dlclose(handle);
        handle = NULL;
        tick = NULL;
      }

      handle = dlopen(dll_path, RTLD_LAZY);
      if (!handle)
      {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        exit(1);
      }
      else
      {
        tick = (tick_func_t)dlsym(handle, "tick");
        char *error = dlerror();
        if (error != NULL)
        {
          fprintf(stderr, "dlsym error: %s\n", error);
          dlclose(handle);
          handle = NULL;
          tick = NULL;
        }
        else
        {
          last_mod_time = current_mod_time;
          printf("Reloaded DLL at %ld\n", last_mod_time);
        }
      }
    }

    if (tick)
      tick();
  }

  if (handle)
    dlclose(handle);

  CloseWindow();
  return 0;
}
