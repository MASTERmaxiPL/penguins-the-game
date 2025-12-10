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

/**
 * @file gameManager.c
 * @brief Implementation of the GameManager initialization, main loop and cleanup.
 */

/**
 * @brief Interactively initialize game parameters and create the board.
 *
 * Prompts the user for:
 *  - number of players (must be > 1)
 *  - board width and height (each between min and max)
 *  - penguins per player (bounded by floor(placeableFloeCount / numOfPlayers))
 *
 * Allocates playersScore array.
 *
 * @param gm Pointer to GameManager to initialize.
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
 * @brief Run placement and movement phases until game ends.
 *
 * @param gm Pointer to GameManager to run.
 */
void GameManager_Run(GameManager *gm) {
    printf("Game running...\n");
    while (gm->isRunning) {
        PlacementPhase_Run(gm);
        MovementPhase_Run(gm);
        gm->isRunning=false;
    }
}

/**
 * @brief Cleanup board and other resources.
 *
 * Frees allocated board memory and should free playersScore (missing in original).
 *
 * @param gm Pointer to GameManager to cleanup.
 */
void GameManager_Cleanup(GameManager *gm) {
    GameBoard_Cleanup(&gm->gb);
    /* Recommended: free(gm->playersScore); gm->playersScore = NULL; */
    printf("Game cleaned up!\n");
}
