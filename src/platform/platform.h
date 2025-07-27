#pragma once

#include <time.h>

typedef void *LibHandle;

time_t getFileModTime(char *path);

LibHandle loadLibrary(char *path);

void unloadLibrary(LibHandle handle);

void *getLibraryFunction(LibHandle handle, char *functionName);

void getExecutableDirectory(char *out, size_t outSize);

void openFileDialog(char *out, size_t outSize);

void saveFileDialog(char *out, size_t outSize);

void buildPath(char *out, size_t outSize, const char *string);

const char *getGameLibraryName();
