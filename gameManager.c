/**
 * @file gameManager.c
 * @brief Handles overall game lifecycle: initialization, running main phases,
 *        and cleanup at the end of the game.
 *
 * This module prompts the user for game settings, initializes the board,
 * manages the placement and movement phases, and shuts down the game cleanly.
 */

#include "boardGenerator.h"
#include "gameManager.h"
#include "inputHandler.h"
#include "messages.h"
#include "movementPhase.h"
#include "placementPhase.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define minPlayerCount 2
#define maxPlayerCount 4
#define minWidth 2
#define maxWidth 30
#define minHeight 2
#define maxHeight 30

#define newGameOption 1
#define loadGameOption 2

#define tryAgainOption 1
#define changeSettingsOption 2

static InputStatus Get_Game_Settings(GameManager *gm, int *outWidth, int *outHeight);
static InputStatus Get_Number_Of_Penguins_Per_Player(GameManager *gm);
static InputStatus Initialize_New_Board(GameManager *gm, const int boardWidth, const int boardHeight);
static void Print_Final_Scores(const GameManager *gm);

/**
 * @brief Initialize the GameManager and prompt the user for game setup values.
 *
 * Allocates memory for scores, initializes the board, and sets initial state.
 *
 * @param gm Pointer to the GameManager to initialize.
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus GameManager_Init(GameManager *gm) {
    InputStatus status;

    int choice;
    char prompt[TEXT_BUFFER_SIZE];

    while (1) {
        snprintf(prompt, sizeof(prompt), MSG_ENTER_NEW_OR_LOAD_GAME, newGameOption, loadGameOption);
        status = GetIntegerInRange(prompt, newGameOption, loadGameOption, &choice);

        if (status == INPUT_EXIT) return status;

        if (choice == loadGameOption) {
            // LOAD GAME
            return INPUT_EXIT;
        }

        if (choice == newGameOption) {
            gm->currentPhase = PHASE_INIT;

            int w, h;
            status = Get_Game_Settings(gm, &w, &h);
            if (status != INPUT_VALID) return status;

            bool setup_complete = false;
            while (!setup_complete)
            {
                status = Initialize_New_Board(gm, w, h);

                if (status == INPUT_ERROR_INIT_FAILED) {
                    snprintf(prompt, sizeof(prompt), MSG_ENTER_TRY_AGAIN_OR_NEW_SETTINGS, tryAgainOption, changeSettingsOption);
                    status = GetIntegerInRange(prompt, tryAgainOption, changeSettingsOption, &choice);
                    if (status == INPUT_EXIT) {
                        return status;
                    }

                    if (choice == tryAgainOption) {
                        GameBoard_Cleanup(&gm->gb);
                        continue;
                    }
                    if (choice == changeSettingsOption) {
                        GameBoard_Cleanup(&gm->gb);
                        break;
                    }
                }

                gm->playersScore = calloc(gm->numOfPlayers, sizeof(int));
                if (Get_Number_Of_Penguins_Per_Player(gm) != INPUT_VALID) {
                    GameBoard_Cleanup(&gm->gb);
                    free(gm->playersScore);
                    break;
                }

                gm->isRunning = true;
                setup_complete = true;
            }

            if (setup_complete)
            {
                gm->currentPhase = PHASE_PLACEMENT;
                return INPUT_VALID;
            }
        }
    }
}

/**
 * @brief Prompt user for game settings: number of players, board width, and height.
 *
 * @param gm Pointer to the GameManager to store number of players.
 * @param outWidth Pointer to store the chosen board width.
 * @param outHeight Pointer to store the chosen board height.
 * @return InputStatus indicating success or failure of input operations.
 */
static InputStatus Get_Game_Settings(GameManager *gm, int *outWidth, int *outHeight){
    char prompt[TEXT_BUFFER_SIZE];
    InputStatus status;

    snprintf(prompt, sizeof(prompt), MSG_ENTER_NUMBER_OF_PLAYERS, minPlayerCount, maxPlayerCount);
    status = GetIntegerInRange(prompt, minPlayerCount, maxPlayerCount, &gm->numOfPlayers);
    if (status != INPUT_VALID) return status;

    snprintf(prompt, sizeof(prompt), MSG_ENTER_BOARD_WIDTH, minWidth, maxWidth);
    status = GetIntegerInRange(prompt, minWidth, maxWidth, outWidth);
    if (status != INPUT_VALID) return status;

    snprintf(prompt, sizeof(prompt), MSG_ENTER_BOARD_HEIGHT, minHeight, maxHeight);
    status = GetIntegerInRange(prompt, minHeight, maxHeight, outHeight);
    return status;
}

/**
 * @brief Initialize a new game board with specified dimensions.
 *
 * Validates that there are enough placeable tiles for all players.
 *
 * @param gm Pointer to the GameManager containing the GameBoard.
 * @param boardWidth Desired width of the game board.
 * @param boardHeight Desired height of the game board.
 * @return InputStatus indicating success or failure of initialization.
 */
static InputStatus Initialize_New_Board(GameManager *gm, const int boardWidth, const int boardHeight) {
    GameBoard_Init(&gm->gb, boardWidth, boardHeight);

    if (gm->gb.placeableFloeCount < gm->numOfPlayers) {
        printf(MSG_NOT_ENOUGH_TILES);
        return INPUT_ERROR_INIT_FAILED;
    }

    return INPUT_VALID;
}

/**
 * @brief Prompt user for the number of penguins each player will have.
 *
 * The maximum number is determined by the total placeable tiles divided
 * by the number of players.
 *
 * @param gm Pointer to the GameManager to store penguins per player.
 * @return InputStatus indicating success or failure of input operation.
 */
static InputStatus Get_Number_Of_Penguins_Per_Player(GameManager *gm){
    char prompt[TEXT_BUFFER_SIZE];

    const int maxPenguins = floor(gm->gb.placeableFloeCount / gm->numOfPlayers);

    snprintf(prompt, sizeof(prompt), MSG_ENTER_NUMBERS_OF_PENGUINS, 1, maxPenguins);
    const InputStatus status = GetIntegerInRange(prompt, 1, maxPenguins, &gm->penguinsPerPlayer);
    if (status == INPUT_EXIT)
    {
        printf(MSG_GAME_CLOSED);
        return status;
    }
    return status;
}

/**
 * @brief Run the game by executing placement and movement phases.
 *
 * The game runs while `gm->isRunning` is true.
 * After both phases finish once, the game ends.
 *
 * @param gm Pointer to the initialized GameManager.
 */
void GameManager_Run(GameManager *gm) {
    InputStatus status;
    printf(MSG_GAME_RUNNING);
    while (gm->isRunning) {
        if (gm->currentPhase == PHASE_PLACEMENT)
        {
            status = PlacementPhase_Run(gm);
            gm->currentPhase = PHASE_MOVEMENT;
            if (status == INPUT_EXIT) {
                gm->isRunning = false;
                return;
            }
        }
        if (gm->currentPhase == PHASE_MOVEMENT)
        {
            status = MovementPhase_Run(gm);
            if (status == INPUT_EXIT) {
                gm->isRunning = false;
                return;
            }
            gm->currentPhase = PHASE_CLEANUP;
        }
        Print_Final_Scores(gm);
        gm->isRunning=false;
    }
}

/**
 * @brief Release all memory associated with the GameManager.
 *
 * Cleans up the GameBoard and prints a shutdown message.
 *
 * @param gm Pointer to the GameManager to clean up.
 *
 * @note In a full implementation, playersScore should also be freed.
 */
void GameManager_Cleanup(GameManager *gm) {
    GameBoard_Cleanup(&gm->gb);
    free(gm->playersScore);
    gm->playersScore = NULL;

    printf(MSG_GAME_CLEANED);
}

static void Print_Final_Scores(const GameManager *gm) {
    printf(MSG_FINAL_SCORES);

    for (int i = 0; i < gm->numOfPlayers; i++) {
        printf(MSG_PLAYER_POINTS, i + 1, gm->playersScore[i]);
    }

    int best = 0;
    for (int i = 1; i < gm->numOfPlayers; i++) {
        if (gm->playersScore[i] > gm->playersScore[best]){
            best = i;
        }
    }

    printf(MSG_WINNER, best + 1, gm->playersScore[best]);
}