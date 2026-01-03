//
// Created by user on 03.01.2026.
//

#ifndef PENGUINS_THE_GAME_CJSONHANDLER_H
#define PENGUINS_THE_GAME_CJSONHANDLER_H

#include "gameManager.h"
#include "cmake-build-debug/_deps/cjson-src/cJSON.h"

cJSON* CJSON_CreateFromGameManager(const GameManager *gm);
bool CJSON_LoadGameManagerFromJson(GameManager *gm, const cJSON *json);

#endif