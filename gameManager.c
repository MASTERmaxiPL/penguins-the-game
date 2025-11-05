#include "gameManager.h"
#include "boardGenerator.h"
#include "placementPhase.h"
#include "movementPhase.h"

#include <stdio.h>

void GameManager_Init(GameManager *gm) {
    gm->isRunning = 1;
    // Init players, board and needed data
    printf("Game initialized!\n");
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
        gm->isRunning=0;
    }
}

void GameManager_Cleanup(GameManager *gm) {
    printf("Game cleaned up!\n");
}
