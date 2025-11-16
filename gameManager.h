#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "gameBoard.h"

typedef struct {
    GameBoard gb;
    int numOfPlayers;
    int *players;
    int penguinsPerPlayer;
    bool isRunning;
} GameManager;

void GameManager_Init(GameManager *gm);
void GameManager_Run(GameManager *gm);
void GameManager_Cleanup(GameManager *gm);

#endif