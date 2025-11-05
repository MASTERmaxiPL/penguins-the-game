#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

typedef struct {
    int isRunning;
} GameManager;

void GameManager_Init(GameManager *gm);
void GameManager_Run(GameManager *gm);
void GameManager_Cleanup(GameManager *gm);

#endif