//
// Created by user on 03.01.2026.
//

#ifndef PENGUINS_THE_GAME_CJSONHANDLER_H
#define PENGUINS_THE_GAME_CJSONHANDLER_H

#include "../core/gameManager.h"
#include "../../cmake-build-debug/_deps/cjson-src/cJSON.h"

/**
 * @brief Serialize the GameManager into a cJSON object.
 *
 * @param gm Pointer to the GameManager to serialize.
 * @return cJSON object representing the GameManager.
 */
cJSON* CJSON_CreateFromGameManager(const GameManager *gm);

/**
 * @brief Load the GameManager state from a cJSON object.
 *
 * @param gm Pointer to the GameManager to populate.
 * @param json cJSON object containing the serialized GameManager state.
 * @return true if loading was successful, false otherwise.
 */
bool CJSON_LoadGameManagerFromJson(GameManager *gm, const cJSON *json);

#endif