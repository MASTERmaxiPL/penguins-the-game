#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "gameBoard.h"
#include "player.h"

typedef struct {
    GameBoard gb;
    Player players[2];
    int numOfPlayers;
    bool isRunning;
    int turn;
} GameManager;

void GameManager_Init(GameManager *gm);
void GameManager_Run(GameManager *gm);
void GameManager_Cleanup(GameManager *gm);

#endif