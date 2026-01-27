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

#include "core/gameManager.h"
#include "common/messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "input/inputHandler.h"

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
    InputStatus status;

    bool session_active = true;
    char input;

    do {
        printf(MSG_WELCOME);

        status = GameManager_Init(&game);

        if (status == INPUT_EXIT) {
            break;
        }

        if (status == INPUT_VALID || status == INPUT_LOADED)
        {
            srand((unsigned int)time(nullptr) ^ (unsigned int)clock());

            if (status == INPUT_LOADED)
            {
                printf(MSG_LOADED_GAME);
                GameManager_Run(&game, true);
            }
            else
            {
                printf(MSG_TRYING_TO_RUN);
                GameManager_Run(&game, false);
            }

            printf(MSG_GAME_FINISHED);
            GameManager_Cleanup(&game);

            printf(MSG_PLAY_AGAIN);
            scanf(" %c", &input);

            if (input != 'y' && input != 'Y')
                session_active = false;
        }

    } while (session_active);

    printf(MSG_GAME_CLOSED);
    return 0;
}
