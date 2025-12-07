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

void GameManager_Init(GameManager *gm) {
    int boardWidth;
    int boardHeight;

    while (1) {
        printf("Enter number of players (n>1): ");
        const int inputCountNOP = scanf("%d", &gm->numOfPlayers);
        if (inputCountNOP != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n');
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
            while (getchar() != '\n');
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
            while (getchar() != '\n');
            continue;
        }
        if (boardHeight < minHeight || boardHeight > maxHeight) {
            printf("The conditions are not met!\n");
        }
        else
            break;
    }

    gm->playersScore = calloc(gm->numOfPlayers, sizeof(int));
    gm->isRunning = true;

    GameBoard_Init(&gm->gb, boardWidth, boardHeight);

    const int maxPenguins = floor(gm->gb.placeableFloeCount / gm->numOfPlayers);

    while (1) {
        printf("Enter number of penguins per player (0<n<=%d): ", maxPenguins);
        const int inputCountPPP = scanf("%d", &gm->penguinsPerPlayer);
        if (inputCountPPP != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n');
            continue;
        }
        if (gm->penguinsPerPlayer < 1 || gm->penguinsPerPlayer > maxPenguins) {
            printf("The number of penguins must be between 1 and %d.\n", maxPenguins);
        }
        else
            break;
    }

    printf("Game initialized!\n");

    Print_Board(&gm->gb);
}

void GameManager_Run(GameManager *gm) {
    printf("Game running...\n");
    while (gm->isRunning) {
        PlacementPhase_Run(gm);
        MovementPhase_Run(gm);
        gm->isRunning=false;
    }
}

void GameManager_Cleanup(GameManager *gm) {
    GameBoard_Cleanup(&gm->gb);
    printf("Game cleaned up!\n");
}
