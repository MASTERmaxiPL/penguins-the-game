#include "gameManager.h"
#include "boardGenerator.h"
#include "placementPhase.h"
#include "movementPhase.h"

#include <stdio.h>
#include <stdlib.h>

void GameManager_Init(GameManager *gm) {
    gm->numOfPlayers = 2; // Currently fixed to 2 players
    gm->penguinsPerPlayer = 8; // Currently fixed to 8 penguins per player
    gm->playersScore = calloc(gm->numOfPlayers, sizeof(int));
    gm->isRunning = true;

    const int boardWidth = 8 + rand() % 4;
    const int boardHeight = 8 + rand() % 4;
    GameBoard_Init(&gm->gb, boardWidth, boardHeight);

    printf("Game initialized!\n");

    GameBoard_Print(&gm->gb);
}

void GameManager_Run(GameManager *gm) {
    printf("Game running...\n");
    while (gm->isRunning) {
        PlacementPhase_Run(gm);
        MovementPhase_IsValidMove(gb, startX, startY, endX, endY);
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
