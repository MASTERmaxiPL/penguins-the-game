/**
 * @file boardGenerator.c
 * @brief Implements board allocation, procedural generation using FBM noise,
 *        ASCII printing, and cleanup routines for the penguin game.
 *
 * This module creates the ice floe board, assigns fish counts, determines
 * floating vs. water tiles, and provides utilities for visualizing and freeing
 * the board.
 */

#include "boardGenerator.h"
#include "messages.h"
#include "noise.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // NOLINT - suppress 'possibly unused #include' in IDE (clang-tidy/clangd)
#include <time.h>
#include <math.h>

#define MAX_FISH_COUNT 3

/**
 * @brief Initialize the random number generator once.
 *
 * Seeds `srand()` using system time and a stack address. This method ensures
 * deterministic seeding only once, regardless of how many times the function
 * is called.
 *
 * @note This function is internal to this translation unit.
 */
static void Init_Random_Seed() {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(nullptr) ^ (unsigned int)(uintptr_t) &seeded);
        seeded = 1;
    }
}

/**
 * @brief Allocate and initialize the GameBoard structure.
 *
 * Allocates the 2D dynamic array `floeGrid`, seeds randomness, and immediately
 * calls Generate_Board() to fill in tile values.
 *
 * @param gb Pointer to the GameBoard structure to initialize.
 * @param boardWidth Width of the board in tiles.
 * @param boardHeight Height of the board in tiles.
 *
 * @note Uses `nullptr` intentionally because the student code is written in
 *       C++-compatible style.
 */
void GameBoard_Init(GameBoard *gb, const int boardWidth, const int boardHeight) {
    gb->boardWidth = boardWidth;
    gb->boardHeight = boardHeight;
    gb->floeGrid = nullptr;

    gb->floeGrid = malloc(boardHeight * sizeof(IceFloe *));
    for (int i = 0; i < boardHeight; i++) {
        gb->floeGrid[i] = malloc(boardWidth * sizeof(IceFloe));
    }

    if (!gb->floeGrid) {
        fprintf(stderr, MSG_MEMORY_ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }

    Init_Random_Seed();
    Generate_Board(gb);
}

/**
 * @brief Generate the board using FBM noise, radial falloff, and randomness.
 *
 * Steps performed:
 *  1. Compute FBM noise at each tile.
 *  2. Apply radial falloff to shape the island.
 *  3. Add a small random jitter.
 *  4. Classify tile as floating or water.
 *  5. Assign fish count (1–3) to floating tiles.
 *  6. Count tiles containing exactly 1 fish (used for placement phase).
 *
 * @param gb Pointer to GameBoard that will be populated.
 */
void Generate_Board(GameBoard *gb) {
    const int fbmOctaves = 5;
    const double fbmLacunarity = 2.0;
    const double fbmGain = 0.5;
    const unsigned int fbmNoiseSeed = (unsigned int)rand();

    const double width = gb->boardWidth;
    const double height = gb->boardHeight;
    const double minDim = width < height ? width : height;

    const double noiseScale = minDim / 5.6;
    const double edgeFalloffExponent = 2;
    const double threshold = -0.04;
    const double maxRandomJitter = 0.15;

    int placeableFloe = 0;

    for (int y = 0; y < gb->boardHeight; y++) {
        const double noiseY = ((double)y - height * 0.5) / noiseScale;
        const double centeredOffsetY = ((double)y - height * 0.5) / (minDim * 0.5);

        for (int x = 0; x < gb->boardWidth; x++) {
            IceFloe *floe = &gb->floeGrid[y][x];

            const double noiseX = ((double)x - width * 0.5) / noiseScale;
            const double centeredOffsetX = ((double)x - width * 0.5) / (minDim * 0.5);

            const double noiseValue = Fbm(noiseX, noiseY, fbmOctaves, fbmLacunarity, fbmGain, fbmNoiseSeed);

            const double radialNorm = sqrt(centeredOffsetX * centeredOffsetX + centeredOffsetY * centeredOffsetY);
            const double falloff = pow(radialNorm, edgeFalloffExponent);

            const double randomUnit = (double)rand() / (double)RAND_MAX;
            const double randomJitter = (randomUnit - 0.5) * maxRandomJitter;

            const double terrainValue = noiseValue - falloff + randomJitter;

            floe->isFloating = terrainValue > threshold ? true : false;
            floe->fishCount = floe->isFloating ? rand() % MAX_FISH_COUNT + 1 : 0;
            floe->occupantId = -1;

            if (floe->fishCount == 1) {
                placeableFloe += 1;
            }
        }
    }
    gb->placeableFloeCount = placeableFloe;
}

/**
 * @brief Print the entire board in an ASCII visual format.
 *
 * Displays coordinate headers and prints either:
 *  - `"P#"` if the tile is occupied by a player's penguin,
 *  - the number of fish on floating tiles,
 *  - or `"X"` for water tiles.
 *
 * @param gb Pointer to the GameBoard to print.
 */
void Print_Board(const GameBoard *gb) {
    printf("   ");
    for (int x = 0; x < gb->boardWidth; x++) {
        if (x<10)
            printf("  %d  ", x);
        else
            printf(" %3d ", x);
    }
    printf("\n");
    for (int y = 0; y < gb->boardHeight; y++) {
        printf("%2d ", y);
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

/**
 * @brief Free all allocated memory associated with the GameBoard.
 *
 * Frees each row of `floeGrid`, then frees the row pointer array itself.
 *
 * @param gb Pointer to GameBoard whose memory will be released.
 *
 * @note The caller is responsible for ensuring no more references exist to the
 *       floeGrid after this call.
 */
void GameBoard_Cleanup(GameBoard *gb) {
    for (int i = 0; i < gb->boardHeight; i++) {
        free(gb->floeGrid[i]);
    }
    free(gb->floeGrid);
    gb->floeGrid = nullptr;
}
