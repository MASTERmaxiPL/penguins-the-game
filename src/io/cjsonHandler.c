/** * @file cjsonHandler.c
 * @brief Functions to serialize and deserialize GameManager to/from cJSON.
 */

#include "cjsonHandler.h"

#include <stdlib.h>
#include <string.h>

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
 * @brief Create a cJSON array representing whether each player is a bot.
 *
 * @param gm Pointer to the GameManager containing bot player info.
 * @return cJSON array of booleans indicating bot players.
 */
static cJSON* create_isbot_array(const GameManager *gm) {
    cJSON *arr = cJSON_CreateArray();
    if (!arr) return NULL;
    for (int i = 0; i < gm->numOfPlayers; ++i) {
        const bool isBot = (gm->isBotPlayers && i < gm->numOfPlayers) ? gm->isBotPlayers[i] : false;
        cJSON_AddItemToArray(arr, cJSON_CreateBool(isBot));
    }
    return arr;
}

/**
 * @brief Serialize the GameManager into a cJSON object.
 *
 * @param gm Pointer to the GameManager to serialize.
 * @return cJSON object representing the GameManager.
 */
cJSON* CJSON_CreateFromGameManager(const GameManager *gm) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "numOfPlayers", gm->numOfPlayers);
    cJSON_AddItemToObject(json, "playersScore", create_players_score_array(gm));
    cJSON_AddNumberToObject(json, "penguinsPerPlayer", gm->penguinsPerPlayer);
    cJSON_AddBoolToObject(json, "isRunning", gm->isRunning);

    const char *phaseStr =
        gm->currentPhase == PHASE_INIT ? "PHASE_INIT" :
        gm->currentPhase == PHASE_PLACEMENT ? "PHASE_PLACEMENT" :
        gm->currentPhase == PHASE_MOVEMENT ? "PHASE_MOVEMENT" :
        "PHASE_CLEANUP";

    cJSON_AddStringToObject(json, "currentPhase", phaseStr);
    cJSON_AddNumberToObject(json, "currentPlayerIndex", gm->currentPlayerIndex);
    cJSON_AddNumberToObject(json, "currentRound", gm->currentRound);

    cJSON *botArr = create_isbot_array(gm);
    if (botArr) cJSON_AddItemToObject(json, "isBotPlayers", botArr);

    cJSON *boardObj = create_board_object(gm);
    if (boardObj) cJSON_AddItemToObject(json, "gb", boardObj);

    return json;
}

/** * @brief Parse a phase string into its corresponding enum value.
 *
 * @param s Phase string to parse.
 * @return Corresponding Phase enum value.
 */
static int parse_phase_string(const char *s) {
    if (strcmp(s, "PHASE_INIT") == 0) return PHASE_INIT;
    if (strcmp(s, "PHASE_PLACEMENT") == 0) return PHASE_PLACEMENT;
    if (strcmp(s, "PHASE_MOVEMENT") == 0) return PHASE_MOVEMENT;
    return PHASE_CLEANUP;
}

/**
 * @brief Load the GameManager state from a cJSON object.
 *
 * @param gm Pointer to the GameManager to populate.
 * @param json cJSON object containing the serialized GameManager state.
 * @return true if loading was successful, false otherwise.
 */
bool CJSON_LoadGameManagerFromJson(GameManager *gm, const cJSON *json) {
    if (!gm || !json) return false;

    const cJSON *item = NULL;

    item = cJSON_GetObjectItemCaseSensitive(json, "numOfPlayers");
    if (!cJSON_IsNumber(item)) return false;
    gm->numOfPlayers = item->valueint;

    item = cJSON_GetObjectItemCaseSensitive(json, "playersScore");
    if (!cJSON_IsArray(item)) return false;
    gm->playersScore = calloc(gm->numOfPlayers, sizeof(int));
    if (!gm->playersScore) return false;
    int idx = 0;
    cJSON *scoreIt = NULL;
    cJSON_ArrayForEach(scoreIt, item) {
        if (idx >= gm->numOfPlayers) break;
        gm->playersScore[idx++] = cJSON_IsNumber(scoreIt) ? scoreIt->valueint : 0;
    }

    item = cJSON_GetObjectItemCaseSensitive(json, "penguinsPerPlayer");
    gm->penguinsPerPlayer = cJSON_IsNumber(item) ? item->valueint : 0;

    item = cJSON_GetObjectItemCaseSensitive(json, "isRunning");
    gm->isRunning = cJSON_IsBool(item) ? cJSON_IsTrue(item) : false;

    item = cJSON_GetObjectItemCaseSensitive(json, "currentPhase");
    const char *phaseStr = cJSON_IsString(item) ? item->valuestring : NULL;
    gm->currentPhase = parse_phase_string(phaseStr);

    item = cJSON_GetObjectItemCaseSensitive(json, "currentPlayerIndex");
    gm->currentPlayerIndex = cJSON_IsNumber(item) ? item->valueint : 0;

    item = cJSON_GetObjectItemCaseSensitive(json, "currentRound");
    gm->currentRound = cJSON_IsNumber(item) ? item->valueint : 0;

    item = cJSON_GetObjectItemCaseSensitive(json, "isBotPlayers");
    if (cJSON_IsArray(item)) {
        gm->isBotPlayers = calloc(gm->numOfPlayers, sizeof(bool));
        if (!gm->isBotPlayers) return false;
        int bidx = 0;
        cJSON *botIt = NULL;
        cJSON_ArrayForEach(botIt, item) {
            if (bidx >= gm->numOfPlayers) break;
            gm->isBotPlayers[bidx++] = cJSON_IsTrue(botIt) ? true : false;
        }
        /* in case JSON array shorter than numOfPlayers, remaining default to false (human) */
        for (; bidx < gm->numOfPlayers; ++bidx) gm->isBotPlayers[bidx] = false;
    } else {
        /* default: all human */
        gm->isBotPlayers = calloc(gm->numOfPlayers, sizeof(bool));
        if (!gm->isBotPlayers) return false;
        for (int i = 0; i < gm->numOfPlayers; ++i) gm->isBotPlayers[i] = false;
    }

    const cJSON *gbObj = cJSON_GetObjectItemCaseSensitive(json, "gb");
    if (!cJSON_IsObject(gbObj)) return false;

    cJSON *tmp = cJSON_GetObjectItemCaseSensitive(gbObj, "boardWidth");
    gm->gb.boardWidth = cJSON_IsNumber(tmp) ? tmp->valueint : 0;
    tmp = cJSON_GetObjectItemCaseSensitive(gbObj, "boardHeight");
    gm->gb.boardHeight = cJSON_IsNumber(tmp) ? tmp->valueint : 0;
    tmp = cJSON_GetObjectItemCaseSensitive(gbObj, "placeableFloeCount");
    gm->gb.placeableFloeCount = cJSON_IsNumber(tmp) ? tmp->valueint : 0;

    const int height = gm->gb.boardHeight, width = gm->gb.boardWidth;
    if (height > 0 && width > 0) {
        gm->gb.floeGrid = calloc(height, sizeof(IceFloe*));
        if (!gm->gb.floeGrid) return false;
        for (int r = 0; r < height; ++r) {
            gm->gb.floeGrid[r] = calloc(width, sizeof(IceFloe));
            if (!gm->gb.floeGrid[r]) {
                for (int k = 0; k < r; ++k) free(gm->gb.floeGrid[k]);
                free(gm->gb.floeGrid);
                gm->gb.floeGrid = NULL;
                return false;
            }
        }

        cJSON *gridArr = cJSON_GetObjectItemCaseSensitive(gbObj, "floeGrid");
        if (cJSON_IsArray(gridArr)) {
            int row = 0;
            cJSON *rowIt = NULL;
            cJSON_ArrayForEach(rowIt, gridArr) {
                if (row >= height) break;
                cJSON *colIt = NULL;
                int col = 0;
                cJSON_ArrayForEach(colIt, rowIt) {
                    if (col >= width) break;
                    const cJSON *isFloating = cJSON_GetObjectItemCaseSensitive(colIt, "isFloating");
                    const cJSON *fishCount = cJSON_GetObjectItemCaseSensitive(colIt, "fishCount");
                    const cJSON *occupantId = cJSON_GetObjectItemCaseSensitive(colIt, "occupantId");
                    gm->gb.floeGrid[row][col].isFloating = cJSON_IsBool(isFloating) ? cJSON_IsTrue(isFloating) : false;
                    gm->gb.floeGrid[row][col].fishCount = cJSON_IsNumber(fishCount) ? fishCount->valueint : 0;
                    gm->gb.floeGrid[row][col].occupantId = cJSON_IsNumber(occupantId) ? occupantId->valueint : -1;
                    ++col;
                }
                ++row;
            }
        }
    }
    return true;
}
