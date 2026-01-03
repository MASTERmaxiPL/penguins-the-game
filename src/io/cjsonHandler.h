#ifndef PENGUINS_THE_GAME_CJSONHANDLER_H
#define PENGUINS_THE_GAME_CJSONHANDLER_H

/**
 * @file cjsonHandler.h
 * @brief Declares functions for serializing and deserializing the GameManager
 *        state to and from cJSON objects.
 *
 * This module provides functionality to convert the entire game state into
 * a JSON representation for saving, as well as loading a saved game state
 * back into the GameManager structure.
 */

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