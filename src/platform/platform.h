#pragma once

#include <time.h>

typedef void *LibHandle;

time_t getFileModTime(char *path);

LibHandle loadLibrary(char *path);

void unloadLibrary(LibHandle handle);

void *getLibraryFunction(LibHandle handle, char *functionName);

void getExecutableDirectory(char *out);

void openFileDialog(char *out, size_t outSize);

void saveFileDialog(char *out, size_t outSize);

const char *getGameLibraryName();
