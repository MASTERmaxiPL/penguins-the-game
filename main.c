#include <stdio.h>
#include "gameManager.h"

/**
 * @file main.c
 * @brief Program entry point and top-level game loop.
 *
 * Repeatedly initializes GameManager, runs a game session, cleans up, and asks
 * the user whether to play again.
 */

/**
 * @brief Program entry point.
 *
 * Initializes a GameManager and runs the game loop until the user declines to play again.
 *
 * @return 0 on success.
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
    }
    while (session_active);

    printf("Game Closed!");
    return 0;
}
