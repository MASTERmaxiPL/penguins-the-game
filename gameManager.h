#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "gameBoard.h"
#include "inputHandler.h"

/**
 * @file gameManager.h
 * @brief Defines the GameManager structure and the lifecycle functions for
 *        initializing, running, and cleaning up the penguin game.
 *
 * The GameManager coordinates all major systems: board generation,
 * player scores, phase execution, and overall game state.
 */

/**
 * @enum GamePhase
 * @brief Represents the different phases of the game lifecycle.
 *
 * This enumeration helps track which phase the game is currently in, allowing
 * to load a game in a correct state.
 */
typedef enum
{
    PHASE_INIT,
    PHASE_PLACEMENT,
    PHASE_MOVEMENT,
    PHASE_CLEANUP
} GamePhase;

/**
 * @struct GameManager
 * @brief Central controller that holds all relevant game-wide information.
 *
 * This structure owns the game board and all player-related variables,
 * such as scores and the number of penguins each player controls.
 */
typedef struct {
    GameBoard gb;          /**< The main game board structure. */
    int numOfPlayers;      /**< Number of players participating in the game. */
    int *playersScore;     /**< Dynamically allocated array storing each player's score. */
    int penguinsPerPlayer; /**< Number of penguins each player places during setup. */
    bool isRunning;        /**< Main loop control flag for GameManager_Run(). */
    GamePhase currentPhase;  /**< Current phase of the game being executed. */
} GameManager;

/**
 * @brief Initialize the GameManager and prompt the user for game setup values.
 *
 * Allocates memory for scores, initializes the board, and sets initial state.
 *
 * @param gm Pointer to the GameManager to initialize.
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus GameManager_Init(GameManager *gm);

/**
 * @brief Run the full game: placement phase followed by movement phase.
 *
 * Continues running until internal state sets `isRunning` to false.
 *
 * @param gm Pointer to the GameManager to run.
 * returns InputStatus indicating success or failure of the game run.
 */
void GameManager_Run(GameManager *gm);

/**
 * @brief Clean up all allocated resources owned by the GameManager.
 *
 * Frees the board and prints a cleanup message.
 *
 * @param gm Pointer to the GameManager to clean up.
 */
void GameManager_Cleanup(GameManager *gm);

#endif
