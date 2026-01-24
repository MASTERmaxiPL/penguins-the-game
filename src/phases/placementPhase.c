/**
 * @file placementPhase.c
 * @brief Implements the placement phase of the game, where players place penguins
 *        onto valid ice floes and collect initial fish.
 *
 * The placement phase proceeds in rounds. Each turn, all players place exactly
 * one penguin until every player has placed the required number.
 */

#include "placementPhase.h"
#include "../core/boardGenerator.h"
#include "../input/inputHandler.h"
#include "../common/messages.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Run the full placement phase of the game.
 *
 * For each required penguin (per player), the function:
 *  - Prints the board,
 *  - Prompts each player to choose a placement location,
 *  - Validates the location,
 *  - Places the penguin and awards fish.
 *
 * @param gm Pointer to the GameManager controlling the game.
 * @param isLoadedGame Indicates if the game was loaded from a saved state.
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus PlacementPhase_Run(GameManager *gm, const bool isLoadedGame) {
    int *playerIndex = &gm->currentPlayerIndex;
    int *round = &gm->currentRound;

    if (!isLoadedGame)
    {
        *playerIndex = 0;
        *round = 1;
    }
    printf(MSG_PLACEMENT_PHASE);

    for (;*round < gm->penguinsPerPlayer + 1; (*round)++) {
        printf(MSG_ROUND, *round);

        for (;*playerIndex < gm->numOfPlayers; (*playerIndex)++) {
            printf(MSG_PLAYER_TURN, *playerIndex + 1);
            Print_Board(&gm->gb);
            const InputStatus status = Player_Placement_Turn(gm);
            if (status == INPUT_EXIT) return status;
        }
        *playerIndex = 0;
    }

    printf(MSG_PLACEMENT_PHASE_FINISHED);
    Print_Board(&gm->gb);

    for (int i = 0; i < gm->numOfPlayers; i++) {
        printf(MSG_PLAYER_POINTS, i + 1, gm->playersScore[i]);
    }
    return INPUT_VALID;
}

/**
 * @brief Handle a single player's placement turn.
 *
 * Prompts the player for board coordinates and validates:
 *  - Coordinates are within bounds,
 *  - The floe is floating,
 *  - The floe contains exactly 1 fish,
 *  - The floe is not occupied.
 *
 * If valid, Player_Place() is called to perform the placement.
 *
 * @param gm Pointer to GameManager.
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus Player_Placement_Turn(GameManager *gm)
{
    int x, y;

    // If the current player is a bot, perform automated placement: choose a random
    // available floe with fishCount == 1.
    if (gm->isBotPlayers && gm->isBotPlayers[gm->currentPlayerIndex]) {
        // collect candidates
        typedef struct { int x; int y; } Pos;
        Pos *candidates = malloc(sizeof(Pos) * gm->gb.placeableFloeCount);
        int candCount = 0;
        for (int i = 0; i < gm->gb.boardHeight; ++i) {
            for (int j = 0; j < gm->gb.boardWidth; ++j) {
                IceFloe *floe = &gm->gb.floeGrid[i][j];
                if (floe->isFloating && floe->occupantId == -1 && floe->fishCount == 1) {
                    candidates[candCount].x = j;
                    candidates[candCount].y = i;
                    candCount++;
                }
            }
        }

        if (candCount == 0) {
            // no valid place — behave like skipping (shouldn't usually happen)
            free(candidates);
            return INPUT_VALID;
        }

        int pick = rand() % candCount;
        x = candidates[pick].x;
        y = candidates[pick].y;
        free(candidates);

        IceFloe *floe = &gm->gb.floeGrid[y][x];
        Player_Place(gm->currentPlayerIndex, gm->playersScore, floe, x, y, gm);
        return INPUT_VALID;
    }

    while (1)
    {
        const InputStatus status = GetCoordinatesInRange(MSG_ENTER_COORDINATES,
                                                   0, gm->gb.boardWidth - 1,
                                                   0, gm->gb.boardHeight - 1,
                                                   &x, &y);

        if (status == INPUT_SAVE || status == INPUT_SAVE_AND_EXIT)
            GameManager_SaveToFile(gm, "..\\saves\\data.json");
        if (status == INPUT_SAVE) continue;
        if (status == INPUT_SAVE_AND_EXIT || status == INPUT_EXIT) return INPUT_EXIT;

        IceFloe *floe = &gm->gb.floeGrid[y][x];

        if (floe->isFloating && floe->occupantId == -1 && floe->fishCount == 1) {
            Player_Place(gm->currentPlayerIndex, gm->playersScore, floe, x, y, gm);
            break;
        }

        printf(MSG_TILE_NOT_AVAILABLE, x, y);
    }
    return INPUT_VALID;
}

/**
 * @brief Place a penguin on a valid tile and award fish to the player.
 *
 * This function:
 *  - Updates the floe to show which player occupies it,
 *  - Adds the fish count on the tile to the player's total,
 *  - Prints a message summarizing the placement.
 *
 * @param playerIndex Index of the player placing the penguin.
 * @param players Pointer to the array containing player fish totals.
 * @param floe Pointer to the IceFloe being occupied.
 * @param x X coordinate of the placement.
 * @param y Y coordinate of the placement.
 */
void Player_Place(const int playerIndex, int *players, IceFloe *floe,
                  const int x, const int y, const GameManager *gm) {

    floe->occupantId = playerIndex;
    players[playerIndex] += floe->fishCount;

    if (gm->isBotPlayers && gm->isBotPlayers[playerIndex]) {
        printf(MSG_AFTER_POSITION_UPDATE_BOT, x, y, floe->fishCount, players[playerIndex]);
    } else {
        printf(MSG_AFTER_POSITION_UPDATE, x, y, floe->fishCount, players[playerIndex]);
    }
}
