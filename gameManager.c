/**
 * @file gameManager.c
 * @brief Handles overall game lifecycle: initialization, running main phases,
 *        and cleanup at the end of the game.
 *
 * This module prompts the user for game settings, initializes the board,
 * manages the placement and movement phases, and shuts down the game cleanly.
 */

#include "gameManager.h"
#include "boardGenerator.h"
#include "placementPhase.h"
#include "movementPhase.h"
#include "math.h"

#include <stdio.h>
#include <stdlib.h>

#define minWidth 10
#define maxWidth 30
#define minHeight 10
#define maxHeight 30

static void Print_Final_Scores(const GameManager *gm);

/**
 * @brief Initialize the GameManager and all required game parameters.
 *
 * Performs the following:
 *  - Prompts the user for the number of players.
 *  - Prompts for valid board width/height.
 *  - Initializes the GameBoard.
 *  - Allocates per-player score storage.
 *  - Determines legal number of penguins per player.
 *
 * @param gm Pointer to the GameManager instance to initialize.
 */
void GameManager_Init(GameManager *gm) {
    int boardWidth;
    int boardHeight;

    while (1) {
        printf("Enter number of players (n>1): ");
        const int inputCountNOP = scanf("%d", &gm->numOfPlayers);
        if (inputCountNOP != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n'){}
            continue;
        }
        if (gm->numOfPlayers < 2) {
            printf("The number of players must be greater than 1.\n");
        }
        else
            break;
    }

    while (1) {
        printf("Enter board width (10<x<30): ");
        const int inputCountBW = scanf("%d", &boardWidth);
        if (inputCountBW != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n'){}
            continue;
        }
        if (boardWidth < minWidth || boardWidth > maxWidth) {
            printf("The conditions are not met!\n");
            continue;
        }
        printf("Enter board height (10<y<30): ");
        const int inputCountBH = scanf("%d", &boardHeight);
        if (inputCountBH != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n'){}
            continue;
        }
        if (boardHeight < minHeight || boardHeight > maxHeight) {
            printf("The conditions are not met!\n");
        }
        else
            break;
    }

    GameBoard_Init(&gm->gb, boardWidth, boardHeight);

    gm->playersScore = calloc(gm->numOfPlayers, sizeof(int));
    gm->isRunning = true;

    const int maxPenguins = floor(gm->gb.placeableFloeCount / gm->numOfPlayers);

    while (1) {
        printf("Enter number of penguins per player (0<n<=%d): ", maxPenguins);
        const int inputCountPPP = scanf("%d", &gm->penguinsPerPlayer);
        if (inputCountPPP != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n'){}
            continue;
        }
        if (gm->penguinsPerPlayer < 1 || gm->penguinsPerPlayer > maxPenguins) {
            printf("The number of penguins must be between 1 and %d.\n", maxPenguins);
        }
        else
            break;
    }

    printf("Game initialized!\n");
}

/**
 * @brief Run the game by executing placement and movement phases.
 *
 * The game runs while `gm->isRunning` is true.
 * After both phases finish once, the game ends.
 *
 * @param gm Pointer to the initialized GameManager.
 */
void GameManager_Run(GameManager *gm) {
    printf("Game running...\n");
    while (gm->isRunning) {
        PlacementPhase_Run(gm);
        MovementPhase_Run(gm);
        Print_Final_Scores(gm);
        gm->isRunning=false;
    }
}

/**
 * @brief Release all memory associated with the GameManager.
 *
 * Cleans up the GameBoard and prints a shutdown message.
 *
 * @param gm Pointer to the GameManager to clean up.
 *
 * @note In a full implementation, playersScore should also be freed.
 */
void GameManager_Cleanup(GameManager *gm) {
    GameBoard_Cleanup(&gm->gb);
    free(gm->playersScore);
    gm->playersScore = NULL;

    printf("Game cleaned up!\n");
}

static void Print_Final_Scores(const GameManager *gm) {
    printf("\n============================\n");
    printf("         FINAL SCORES       \n");
    printf("============================\n");

    for (int i = 0; i < gm->numOfPlayers; i++) {
        printf("Player %d: %d points\n", i + 1, gm->playersScore[i]);
    }

    int best = 0;
    for (int i = 1; i < gm->numOfPlayers; i++) {
        if (gm->playersScore[i] > gm->playersScore[best]){
            best = i;
        }
    }

    printf("Winner: Player %d!\n", best + 1);
    printf("============================\n\n");
}