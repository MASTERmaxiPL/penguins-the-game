/**
 * @file gameManager.c
 * @brief Handles overall game lifecycle: initialization, running main phases,
 *        and cleanup at the end of the game.
 *
 * This module prompts the user for game settings, initializes the board,
 * manages the placement and movement phases, and shuts down the game cleanly.
 */

#include "gameManager.h"
#include "boardGenerator.h"
#include "placementPhase.h"
#include "messages.h"
#include "movementPhase.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define minPlayerCount 2
#define maxPlayerCount 4
#define minWidth 8
#define maxWidth 30
#define minHeight 8
#define maxHeight 30

static void Print_Final_Scores(const GameManager *gm);

/**
 * @brief Initialize the GameManager and all required game parameters.
 *
 * Performs the following:
 *  - Prompts the user for the number of players.
 *  - Prompts for valid board width/height.
 *  - Initializes the GameBoard.
 *  - Allocates per-player score storage.
 *  - Determines legal number of penguins per player.
 *
 * @param gm Pointer to the GameManager instance to initialize.
 */
void GameManager_Init(GameManager *gm) {
    int boardWidth;
    int boardHeight;

    while (1) {
        printf(MSG_ENTER_NUMBER_OF_PLAYERS, minPlayerCount, maxPlayerCount);
        const int inputCountNOP = scanf("%d", &gm->numOfPlayers);
        if (inputCountNOP != 1) {
            printf(MSG_INVALID_INPUT_TYPE);
            while (getchar() != '\n'){}
            continue;
        }
        if (gm->numOfPlayers < minPlayerCount || gm->numOfPlayers > maxPlayerCount) {
            printf(MSG_INPUT_OUT_OF_RANGE, minPlayerCount, maxPlayerCount);
        }
        else
            break;
    }

    while (1) {
        printf(MSG_ENTER_BOARD_WIDTH, minWidth, maxWidth);
        const int inputCountBW = scanf("%d", &boardWidth);
        if (inputCountBW != 1) {
            printf(MSG_INVALID_INPUT_TYPE);
            while (getchar() != '\n'){}
            continue;
        }
        if (boardWidth < minWidth || boardWidth > maxWidth) {
            printf(MSG_INPUT_OUT_OF_RANGE, minWidth, maxWidth);
            continue;
        }
        printf(MSG_ENTER_BOARD_HEIGHT, minHeight, maxHeight);
        const int inputCountBH = scanf("%d", &boardHeight);
        if (inputCountBH != 1) {
            printf(MSG_INVALID_INPUT_TYPE);
            while (getchar() != '\n'){}
            continue;
        }
        if (boardHeight < minHeight || boardHeight > maxHeight) {
            printf(MSG_INPUT_OUT_OF_RANGE, minHeight, maxHeight);
        }
        else
            break;
    }

    GameBoard_Init(&gm->gb, boardWidth, boardHeight);

    if (gm->gb.placeableFloeCount < gm->numOfPlayers) {
        printf(MSG_NOT_ENOUGH_TILES);
        return;
    }

    gm->playersScore = calloc(gm->numOfPlayers, sizeof(int));
    gm->isRunning = true;

    const int maxPenguins = floor(gm->gb.placeableFloeCount / gm->numOfPlayers);

    while (1) {
        printf(MSG_ENTER_NUMBERS_OF_PENGUINS, maxPenguins);
        const int inputCountPPP = scanf("%d", &gm->penguinsPerPlayer);
        if (inputCountPPP != 1) {
            printf(MSG_INVALID_INPUT_TYPE);
            while (getchar() != '\n'){}
            continue;
        }
        if (gm->penguinsPerPlayer < 1 || gm->penguinsPerPlayer > maxPenguins) {
            printf(MSG_INPUT_OUT_OF_RANGE, 1, maxPenguins);
        }
        else
            break;
    }

    printf(MSG_INITIALIZED);
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
    printf(MSG_GAME_RUNNING);
    while (gm->isRunning) {
        PlacementPhase_Run(gm);
        MovementPhase_Run(gm);
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