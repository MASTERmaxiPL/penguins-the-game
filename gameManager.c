#include "gameManager.h"
#include "boardGenerator.h"
#include "placementPhase.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>

void GameManager_Init(GameManager *gm) {
    gm->numOfPlayers = 2; // Currently fixed to 2 players
    gm->isRunning = true;
    gm->turn = 0;
    // placement phase player 0
    gm->players[0].symbol = 'A';
    gm->players[0].FishSum = 0;
    gm->players[0].remainingPenguins = 2; // value can be changed at any time
    // player 1
    gm->players[1].symbol = 'B';
    gm->players[1].FishSum = 0;
    gm->players[1].remainingPenguins = 2; // value can be changed at any time

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
        PlacementPhase_Run(gm);
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
