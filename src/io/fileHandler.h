#ifndef PENGUINS_THE_GAME_FILEHANDLER_H
#define PENGUINS_THE_GAME_FILEHANDLER_H

/** * @file fileHandler.h
 * @brief Declares functions for saving and loading game state in JSON format.
 *
 * This module provides functionality to serialize the game state to a JSON
 * file and deserialize it back into the game's data structures.
 */

#include "../../cmake-build-debug/_deps/cjson-src/cJSON.h"

/**
 * Saves a cJSON object to a file in JSON format.
 *
 * @param json Pointer to the cJSON object to save.
 * @param path File path where the JSON should be saved.
 * @return true if the operation was successful, false otherwise.
 */
bool SaveJsonToFile(const cJSON *json, const char *path);

/**
 * Loads a cJSON object from a file containing JSON data.
 *
 * @param path File path from which to load the JSON.
 * @return Pointer to the loaded cJSON object, or NULL on failure.
 */
cJSON *LoadJsonFromFile(const char *path);

#endif