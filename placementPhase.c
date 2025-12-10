#include "placementPhase.h"
#include "boardGenerator.h"

#include <stdio.h>

/**
 * @file placementPhase.c
 * @brief Implementation of the placement phase where players place penguins.
 */

/**
 * @brief Run placement rounds until each player has placed all penguins.
 *
 * @param gm Pointer to GameManager with game settings and board.
 */
void PlacementPhase_Run(GameManager *gm) {
    for (int r = 0; r < gm->penguinsPerPlayer; r++) {
        for (int p = 0; p < gm->numOfPlayers; p++) {
            Player_Placement_Turn(gm, p);
        }
    }

    Print_Board(&gm->gb);

    printf("Placement phase finished. Scores:\n");
    for (int i = 0; i < gm->numOfPlayers; ++i) {
        printf("Player %d: %d\n", i + 1, gm->playersScore[i]);
    }
}

/**
 * @brief Execute a single player's placement turn (interactive).
 *
 * Validates selected floe is floating, unoccupied and has exactly 1 fish.
 *
 * @param gm Pointer to GameManager.
 * @param currentPlayerIndex Index of the player placing.
 */
void Player_Placement_Turn(GameManager *gm, const int currentPlayerIndex) {
    int x, y;
    printf("Player %d, choose a floe to place (x y) on a floe with 1 fish: ", currentPlayerIndex + 1);
    if (scanf("%d %d", &x, &y) != 2) {
        printf("Invalid input\n");
        while (getchar() != '\n') {}
        return;
    }

    if (!Is_Move_In_Bounds(&gm->gb, x, y) ||
        !gm->gb.floeGrid[y][x].isFloating ||
        gm->gb.floeGrid[y][x].occupantId != -1 ||
        gm->gb.floeGrid[y][x].fishCount != 1) {
        printf("Invalid placement. Must be a floating, unoccupied floe with 1 fish.\n");
        return;
    }

    Player_Place(currentPlayerIndex, gm->playersScore, &gm->gb.floeGrid[y][x], x, y);
}

/**
 * @brief Place a penguin for a player onto the given floe and update score.
 *
 * Adds the floe's fishCount to the player's score and marks occupant. Recommended
 * to set floe->fishCount to 0 to reflect collected fish.
 *
 * @param playerIndex Player placing (0-based).
 * @param players Array of player scores.
 * @param floe Pointer to target IceFloe.
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void Player_Place(const int playerIndex, int *players, IceFloe *floe, const int x, const int y) {
    players[playerIndex] += floe->fishCount;
    floe->occupantId = playerIndex;
    /* Recommended: floe->fishCount = 0; */
    printf("Player %d placed a penguin at (%d,%d) and collected %d fish.\n", playerIndex + 1, x, y, players[playerIndex]);
}
