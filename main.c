/**
 * @file main.c
 * @brief Entry point for the penguin game application.
 *
 * This file contains the main loop that:
 *  - Initializes the game,
 *  - Runs all game phases,
 *  - Cleans up allocated resources,
 *  - And allows the user to replay the game if desired.
 */

#include <stdio.h>
#include "gameManager.h"

/**
 * @brief Program entry point.
 *
 * Creates a GameManager instance, runs a complete game session, and repeatedly
 * offers the player the option to start a new session. The loop terminates
 * when the user chooses not to continue.
 *
 * @return Returns 0 upon successful execution.
 */
int main() {
    GameManager game;
    bool session_active = true;

    do {
        char input;
        printf("Welcome to penguin game!\n");
        printf("Initiating...\n");

        GameManager_Init(&game);

        printf("Trying to run...\n");
        GameManager_Run(&game);

        printf("Game finished, cleaning...\n");
        GameManager_Cleanup(&game);

        printf("Do you want to play again? (print 'y' to continue, else program finishes)\n");
        scanf(" %c", &input);

        if (input != 'y' && input != 'Y')
            session_active = false;

    } while (session_active);

    printf("Game Closed!");
    return 0;
}
