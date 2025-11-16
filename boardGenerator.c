#include "boardGenerator.h"

#include <stdio.h>
#include <stdlib.h>

#define IS_FLOATING_PROBABILITY 75
#define MAX_FISH_COUNT 3


void GameBoard_Init(GameBoard *gb, const int boardWidth, const int boardHeight) {
    gb->boardWidth = boardWidth;
    gb->boardHeight = boardHeight;
    gb->floeGrid = nullptr;

    gb->floeGrid = malloc(boardHeight * sizeof(IceFloe *));
    for (int i = 0; i < boardHeight; i++) {
        gb->floeGrid[i] = malloc(boardWidth * sizeof(IceFloe));
    }

    if (!gb->floeGrid) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    GameBoard_Generate(gb);
}

void GameBoard_Generate(const GameBoard *gb) {
    for (int y = 0; y < gb->boardHeight; y++) {
        for (int x = 0; x < gb->boardWidth; x++) {
            IceFloe *floe = &gb->floeGrid[y][x];

            floe->isFloating = (rand() % 100) < IS_FLOATING_PROBABILITY;

            if (floe->isFloating) {
                floe->fishCount = (rand() % (MAX_FISH_COUNT + 1));
            } else {
                floe->fishCount = 0;
            }

            floe->occupantId = -1; // No occupant
        }
    }
}

void GameBoard_Print(const GameBoard *gb) {
    for (int y = 0; y < gb->boardHeight; y++) {
        for (int x = 0; x < gb->boardWidth; x++) {
            const IceFloe *floe = &gb->floeGrid[y][x];

            if (floe->occupantId != -1) // if occupied
            {
                printf("| P%d ", floe->occupantId + 1);
            }
            else if (floe->isFloating) //if not occupied, if ice floe
            {
                printf("| %d  ", floe->fishCount);
            }
            else // no ice floe
            {
                printf("| X  ");
            }
        }
        printf("| \n");
    }
}

void GameBoard_Cleanup(GameBoard *gb) {
    for (int i = 0; i < gb->boardHeight; i++) {
        free(gb->floeGrid[i]);
    }
    free(gb->floeGrid);
    gb->floeGrid = nullptr;
}