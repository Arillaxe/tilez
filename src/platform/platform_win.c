#include "platform.h"
#include <windows.h>
#include <shlwapi.h>

time_t getFileModTime(char *path)
{
  WIN32_FILE_ATTRIBUTE_DATA fileInfo;

  if (!GetFileAttributesEx(path, GetFileExInfoStandard, &fileInfo))
  {
    return (time_t)(-1);
  }

  ULARGE_INTEGER ull;
  ull.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
  ull.HighPart = fileInfo.ftLastWriteTime.dwHighDateTime;

  return (time_t)((ull.QuadPart / 10000000ULL) - 11644473600ULL);
}

LibHandle loadLibrary(char *path)
{
  return LoadLibrary(path);
}

void unloadLibrary(LibHandle handle)
{
  FreeLibrary(handle);
}

void *getLibraryFunction(LibHandle handle, char *functionName)
{
  return GetProcAddress(handle, functionName);
}

void getExecutableDirectory(char *out)
{
  GetModuleFileName(NULL, out, MAX_PATH);
  PathRemoveFileSpec(out);
}

void openFileDialog(char *out, size_t outSize)
{
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(ofn));

  out[0] = '\0';

  getExecutableDirectory(out);

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = out;
  ofn.nMaxFile = outSize;
  ofn.lpstrInitialDir = out;
  ofn.lpstrFilter = "Level Files\0*.lvl\0All Files\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  GetOpenFileName(&ofn);
}

void saveFileDialog(char *out, size_t outSize)
{
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(ofn));

  out[0] = '\0';

  getExecutableDirectory(out);

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = out;
  ofn.nMaxFile = outSize;
  ofn.lpstrInitialDir = out;
  ofn.lpstrFilter = "Level Files\0*.lvl\0All Files\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

  GetSaveFileName(&ofn);
}

const char *getGameLibraryName()
{
  return "game.dll";
}
