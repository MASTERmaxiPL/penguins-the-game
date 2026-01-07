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
 */
void PlacementPhase_Run(GameManager *gm) {
    printf("Placement Phase\n");

    for (int turn = 1; turn < gm->penguinsPerPlayer + 1; turn++) {
        printf("Turn %d\n", turn);

        for (int currentPlayerIndex = 0; currentPlayerIndex < gm->numOfPlayers; currentPlayerIndex++) {
            Print_Board(&gm->gb);
            Player_Placement_Turn(gm, currentPlayerIndex);
        }
    }

    printf("Placement Phase finished successfully!\n");
    Print_Board(&gm->gb);

    for (int i = 0; i < gm->numOfPlayers; i++) {
        printf("Player %d has %d fish after placement phase.\n",
               i + 1, gm->playersScore[i]);
    }
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
 */
void Player_Placement_Turn(GameManager *gm, const int currentPlayerIndex)
{
    int x, y;

    while (1)
    {
        printf("Player %d, put your x y coordinates: \n", currentPlayerIndex + 1);
        const int inputCount = scanf("%d %d", &x, &y);

        if (inputCount != 2) {
            printf("Invalid input! Please enter two integers.\n");
            while (getchar() != '\n'){}
            continue;
        }

        if (x >= gm->gb.boardWidth || y >= gm->gb.boardHeight || x < 0 || y < 0) {
            printf("Your coordinates are out of bounds!\n");
            continue;
        }

        IceFloe *floe = &gm->gb.floeGrid[y][x];

        if (floe->isFloating && floe->occupantId == -1 && floe->fishCount == 1) {
            Player_Place(currentPlayerIndex, gm->playersScore, floe, x, y);
            break;
        }

        printf("You cannot place your penguin here. Try again! (%d,%d)!\n", x, y);
    }
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

    printf("Current Player placed his penguin on (%d,%d) and obtained %d fish. "
           "Now has %d fish.\n",
           x, y, floe->fishCount, players[playerIndex]);
}
