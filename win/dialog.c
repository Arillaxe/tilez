#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shlwapi.h>
#include <commdlg.h>
#include <string.h>
#include "dialog.h"

static char filePath[MAX_PATH] = {0};

const char *openFileDialog()
{
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(ofn));
  filePath[0] = '\0';

  char exePath[MAX_PATH];
  GetModuleFileName(NULL, exePath, MAX_PATH);
  PathRemoveFileSpec(exePath);

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = filePath;
  ofn.nMaxFile = sizeof(filePath);
  ofn.lpstrInitialDir = exePath;
  ofn.lpstrFilter = "Level Files\0*.lvl\0All Files\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileName(&ofn))
    return filePath;
  return NULL;
}

const char *saveFileDialog()
{
  OPENFILENAME ofn;
  ZeroMemory(&ofn, sizeof(ofn));
  filePath[0] = '\0';

  char exePath[MAX_PATH];
  GetModuleFileName(NULL, exePath, MAX_PATH);
  PathRemoveFileSpec(exePath);

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = filePath;
  ofn.nMaxFile = sizeof(filePath);
  ofn.lpstrInitialDir = exePath;
  ofn.lpstrFilter = "Level Files\0*.lvl\0All Files\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

  if (GetSaveFileName(&ofn))
    return filePath;
  return NULL;
}
