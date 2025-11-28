#include "gameManager.h"
#include "boardGenerator.h"
#include "placementPhase.h"
#include "movementPhase.h"

#include <stdio.h>
#include <stdlib.h>

void GameManager_Init(GameManager *gm) {
    //gm->numOfPlayers = 2;
    //gm->penguinsPerPlayer = 3;
    const int boardWidth;
    const int boardHeight;
    while (1) { // Board sizes
        printf("Enter board width (5<x<20): ");
        const int inputCountBW = scanf("%d", &boardWidth);
        if (inputCountBW != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n');
            continue;
        }
        if (boardWidth < 5 || boardWidth > 20) {
            printf("The conditions are not met!\n");
            continue;
        }
        printf("Enter board height (5<y<20): ");
        const int inputCountBH = scanf("%d", &boardHeight);
        if (inputCountBH != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n');
            continue;
        }
        if (boardHeight < 5 || boardHeight > 20) {
            printf("The conditions are not met!\n");
        }
        else
            break;
    }
    while (1) { // Number of players
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
    while (1) { // Number of penguins per player
        printf("Enter number of penguins per player (n>0): ");
        const int inputCountPPP = scanf("%d", &gm->penguinsPerPlayer);
        if (inputCountPPP != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n');
            continue;
        }
        if (gm->penguinsPerPlayer < 1) {
            printf("The number of penguins must be greater than 0.\n");
        }
        else
            break;
    }
    gm->playersScore = calloc(gm->numOfPlayers, sizeof(int));
    gm->isRunning = true;
    //const int boardWidth = 8 + rand() % 4;
    //const int boardHeight = 8 + rand() % 4;
    GameBoard_Init(&gm->gb, boardWidth, boardHeight);

    printf("Game initialized!\n");

    Print_Board(&gm->gb);
}

void GameManager_Run(GameManager *gm) {
    printf("Game running...\n");
    while (gm->isRunning) {
        PlacementPhase_Run(gm);
        MovementPhase_Run(gm);
        /*
        * if no more moves available
        * if (something)
        *    gm->isRunning = 0;
        */
        gm->isRunning=false;
    }
}

void GameManager_Cleanup(GameManager *gm) {
    GameBoard_Cleanup(&gm->gb);
    printf("Game cleaned up!\n");
}
