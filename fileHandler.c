#include "fileHandler.h"

#include <stdio.h>
#include <stdlib.h>

#include "cmake-build-debug/_deps/cjson-src/cJSON.h"

/**
 * @file fileHandler.c
 * @brief Implements functions to save and load the game state to/from a JSON file.
 *
 * This module uses the cJSON library to serialize and deserialize the GameManager
 * structure, including player scores and the game board state.
 */

/**
    * @brief Create a cJSON array representing the players' scores.
    *
    * @param gm Pointer to the GameManager containing player scores.
    * @return cJSON array of player scores.
    */
static cJSON* create_players_score_array(const GameManager *gm) {
    return cJSON_CreateIntArray(gm->playersScore, gm->numOfPlayers);
}

/**
 * @brief Create a cJSON object representing a single IceFloe.
 *
 * @param floe Pointer to the IceFloe to serialize.
 * @return cJSON object representing the IceFloe.
 */
static cJSON* create_floe_object(const IceFloe *floe) {
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddBoolToObject(obj, "isFloating", floe->isFloating);
    cJSON_AddNumberToObject(obj, "fishCount", floe->fishCount);
    cJSON_AddNumberToObject(obj, "occupantId", floe->occupantId);
    return obj;
}

/**
 * @brief Create a cJSON array representing the 2D grid of IceFloes.
 *
 * @param gb Pointer to the GameBoard containing the floe grid.
 * @return cJSON array of arrays representing the floe grid.
 */
static cJSON* create_floe_grid_array(const GameBoard *gb) {
    cJSON *gridArr = cJSON_CreateArray();

    const IceFloe defaultFloe = { .isFloating = false, .fishCount = 0, .occupantId = -1 };

    const int height = gb->boardHeight;
    const int width = gb->boardWidth;
    for (int row = 0; row < height; ++row) {
        cJSON *rowArr = cJSON_CreateArray();
        for (int column = 0; column < width; ++column) {

            const IceFloe *floePtr = &defaultFloe;
            if (gb->floeGrid && gb->floeGrid[row]) floePtr = &gb->floeGrid[row][column];
            cJSON *floeObj = create_floe_object(floePtr);
            cJSON_AddItemToArray(rowArr, floeObj);
        }
        cJSON_AddItemToArray(gridArr, rowArr);
    }
    return gridArr;
}

/**
 * @brief Create a cJSON object representing the GameBoard.
 *
 * @param gm Pointer to the GameManager containing the GameBoard.
 * @return cJSON object representing the GameBoard.
 */
static cJSON* create_board_object(const GameManager *gm) {
    cJSON *boardObj = cJSON_CreateObject();

    cJSON_AddNumberToObject(boardObj, "boardWidth", gm->gb.boardWidth);
    cJSON_AddNumberToObject(boardObj, "boardHeight", gm->gb.boardHeight);
    cJSON_AddNumberToObject(boardObj, "placeableFloeCount", gm->gb.placeableFloeCount);

    cJSON *grid = create_floe_grid_array(&gm->gb);

    cJSON_AddItemToObject(boardObj, "floeGrid", grid);

    return boardObj;
}

/**
 * @brief Save the current game state to a JSON file.
 *
 * @param gm Pointer to the GameManager containing the game state.
 */
void Save_GameFile(const GameManager *gm) {
    cJSON *json = cJSON_CreateObject();
    if (!json) return;

    cJSON_AddNumberToObject(json, "numOfPlayers", gm->numOfPlayers);
    cJSON_AddItemToObject(json, "playersScore", create_players_score_array(gm));
    cJSON_AddNumberToObject(json, "penguinsPerPlayer", gm->penguinsPerPlayer);
    cJSON_AddBoolToObject(json, "isRunning", gm->isRunning);
    cJSON_AddStringToObject(json, "currentPhase",
                            gm->currentPhase == PHASE_INIT ? "PHASE_INIT" :
                            gm->currentPhase == PHASE_PLACEMENT ? "PHASE_PLACEMENT" :
                            gm->currentPhase == PHASE_MOVEMENT ? "PHASE_MOVEMENT" :
                            "PHASE_CLEANUP"
                            );

    cJSON *boardObj = create_board_object(gm);
    //Store data to cJSON object
    if (boardObj) cJSON_AddItemToObject(json, "board", boardObj);

    // convert the cJSON object to a JSON string
    char *json_str = cJSON_Print(json);
    const char *relative = "..\\data.json";

    FILE *fp = fopen(relative, "w");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        if (json_str) cJSON_free(json_str);
        cJSON_Delete(json);
    }
    else {
        printf("%s\n", json_str);
        fputs(json_str, fp);
        fclose(fp);
        // free the JSON string and cJSON object
        if (json_str) cJSON_free(json_str);
        cJSON_Delete(json);
    }
}

void Load_GameFile(GameManager *gm) {
}