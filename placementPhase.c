/**
 * @file placementPhase.c
 * @brief Implements the placement phase of the game, where players place penguins
 *        onto valid ice floes and collect initial fish.
 *
 * The placement phase proceeds in rounds. Each turn, all players place exactly
 * one penguin until every player has placed the required number.
 */

#include "placementPhase.h"
#include "boardGenerator.h"
#include "inputHandler.h"
#include "messages.h"

#include <stdio.h>

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
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus PlacementPhase_Run(GameManager *gm) {
    printf(MSG_PLACEMENT_PHASE);

    for (int round = 1; round < gm->penguinsPerPlayer + 1; round++) {
        printf(MSG_ROUND, round);

        for (int currentPlayerIndex = 0; currentPlayerIndex < gm->numOfPlayers; currentPlayerIndex++) {
            printf(MSG_PLAYER_TURN, currentPlayerIndex+1);
            Print_Board(&gm->gb);
            const InputStatus status = Player_Placement_Turn(gm, currentPlayerIndex);
            if (status == INPUT_EXIT) return status;
        }
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
 * @param currentPlayerIndex Index of the player taking the turn.
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus Player_Placement_Turn(GameManager *gm, const int currentPlayerIndex)
{
    int x, y;

    while (1)
    {
        const InputStatus status = GetCoordinatesInRange(MSG_ENTER_COORDINATES,
                                                   0, gm->gb.boardWidth - 1,
                                                   0, gm->gb.boardHeight - 1,
                                                   &x, &y);

        if (status == INPUT_EXIT) return status;

        IceFloe *floe = &gm->gb.floeGrid[y][x];

        if (floe->isFloating && floe->occupantId == -1 && floe->fishCount == 1) {
            Player_Place(currentPlayerIndex, gm->playersScore, floe, x, y);
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
                  const int x, const int y) {

    floe->occupantId = playerIndex;
    players[playerIndex] += floe->fishCount;

    printf(MSG_AFTER_POSITION_UPDATE, x, y, floe->fishCount, players[playerIndex]);
}
