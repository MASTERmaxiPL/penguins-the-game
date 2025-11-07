#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "gameBoard.h"

typedef struct {
    GameBoard gb;
    int numOfPlayers;
    bool isRunning;
} GameManager;

void GameManager_Init(GameManager *gm);
void GameManager_Run(GameManager *gm);
void GameManager_Cleanup(GameManager *gm);

#endif