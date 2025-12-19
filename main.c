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

#include "gameManager.h"
#include "messages.h"

#include <stdio.h>

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
        printf(MSG_WELCOME);
        printf(MSG_INITIALIZING);

        GameManager_Init(&game);

        printf(MSG_TRYING_TO_RUN);
        GameManager_Run(&game);

        printf(MSG_GAME_FINISHED);
        GameManager_Cleanup(&game);

        printf(MSG_PLAY_AGAIN);
        scanf(" %c", &input);

        if (input != 'y' && input != 'Y')
            session_active = false;

    } while (session_active);

    printf(MSG_GAME_CLOSED);
    return 0;
}
