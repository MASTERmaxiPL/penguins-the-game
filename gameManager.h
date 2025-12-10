#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "gameBoard.h"

/**
 * @file gameManager.h
 * @brief High-level manager that owns game state and orchestrates phases.
 *
 * GameManager stores the board and player-related settings and provides
 * lifecycle functions: Init, Run and Cleanup.
 */

typedef struct {
    GameBoard gb;        /**< the game board */
    int numOfPlayers;    /**< number of players (>1) */
    int *playersScore;   /**< dynamically allocated array length numOfPlayers */
    int penguinsPerPlayer; /**< number of penguins per player */
    bool isRunning;      /**< running flag used by GameManager_Run */
} GameManager;

/**
 * @brief Initialize a GameManager, prompt user for settings and allocate resources.
 *
 * @param gm Pointer to GameManager to initialize.
 */
void GameManager_Init(GameManager *gm);

/**
 * @brief Run the game loop: placement and movement phases.
 *
 * @param gm Pointer to GameManager to run.
 */
void GameManager_Run(GameManager *gm);

/**
 * @brief Cleanup resources allocated by the GameManager.
 *
 * Frees board resources; note playersScore should be freed here as well.
 *
 * @param gm Pointer to GameManager to cleanup.
 */
void GameManager_Cleanup(GameManager *gm);

#endif
