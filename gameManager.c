#include "gameManager.h"
#include "boardGenerator.h"
#include "placementPhase.h"
#include "movementPhase.h"

#include <stdio.h>
#include <stdlib.h>

void GameManager_Init(GameManager *gm) {
    gm->numOfPlayers = 2; // Currently fixed to 2 players
    gm->isRunning = true;
    const int boardWidth = 8 + rand() % 4;
    const int boardHeight = 8 + rand() % 4;
    GameBoard_Init(&gm->gb, boardWidth, boardHeight);

    //TO-DO Init players and needed data
    printf("Game initialized!\n");

    GameBoard_Print(&gm->gb);
}

void GameManager_Run(GameManager *gm) {
    printf("Game running...\n");
    while (gm->isRunning) {
        // Placement and Movement Phases
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
