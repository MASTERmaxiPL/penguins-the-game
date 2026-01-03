#ifndef PENGUINS_THE_GAME_FILEHANDLER_H
#define PENGUINS_THE_GAME_FILEHANDLER_H

#include "cmake-build-debug/_deps/cjson-src/cJSON.h"

bool SaveJsonToFile(const cJSON *json, const char *path);
cJSON *LoadJsonFromFile(const char *path);

#endif