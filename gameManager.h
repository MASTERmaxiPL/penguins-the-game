#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "gameBoard.h"   // Provides the GameBoard structure used inside GameManager

// Stores all global data needed to run the game.
// Controls initialization, game flow, and cleanup.
typedef struct {
    GameBoard gb;            // The main game board
    int numOfPlayers;        // Total number of players
    int *playersScore;       // Dynamically allocated array storing player scores
    int penguinsPerPlayer;   // Number of penguins assigned to each player
    bool isRunning;          // Indicates whether the game loop should continue
} GameManager;

// Initializes game settings, allocates memory, and prepares the playing board.
void GameManager_Init(GameManager *gm);

// Executes the gameplay loop (placement phase → movement phase).
void GameManager_Run(GameManager *gm);

// Frees memory and resets game state after a finished session.
void GameManager_Cleanup(GameManager *gm);

#endif
