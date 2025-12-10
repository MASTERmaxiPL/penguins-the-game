#include "boardGenerator.h"
#include "noise.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // NOLINT - suppress 'possibly unused #include' in IDE (clang-tidy/clangd)
#include <time.h>
#include <math.h>

#define MAX_FISH_COUNT 3

/**
 * @file boardGenerator.c
 * @brief Implementation of GameBoard allocation, procedural generation and printing.
 */

/**
 * @internal
 * @brief Initialize srand() once per program run.
 *
 * This function ensures the random sequence is seeded only once.
 */
static void Init_Random_Seed() {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)&seeded);
        seeded = 1;
    }
}

/**
 * @brief Initialize GameBoard memory and generate contents.
 *
 * Allocates the floeGrid as an array of row pointers and individual rows,
 * then initializes the board via Generate_Board.
 *
 * @param gb Pointer to GameBoard structure to initialize.
 * @param boardWidth Number of columns.
 * @param boardHeight Number of rows.
 */
void GameBoard_Init(GameBoard *gb, const int boardWidth, const int boardHeight) {
    gb->boardWidth = boardWidth;
    gb->boardHeight = boardHeight;
    gb->floeGrid = NULL;

    gb->floeGrid = malloc(boardHeight * sizeof(IceFloe *));
    for (int i = 0; i < boardHeight; i++) {
        gb->floeGrid[i] = malloc(boardWidth * sizeof(IceFloe));
    }

    if (!gb->floeGrid) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    Init_Random_Seed();
    Generate_Board(gb);
}

/**
 * @brief Generate the procedural board layout.
 *
 * Uses FBM noise to compute a terrain value for each grid cell, applies a
 * radial falloff to make islands, and assigns fish counts and occupancy.
 *
 * @param gb Pointer to GameBoard to populate.
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

            floe->isFloating = (terrainValue > threshold) ? 1 : 0;
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
 * @brief Print the board in a human-readable ASCII format.
 *
 * The grid prints column headers, then each row. Occupied cells (occupantId != -1)
 * print as "P#", floating cells show their fish count, water cells are 'X'.
 *
 * @param gb Pointer to GameBoard to print.
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
 * @brief Free all memory used by the GameBoard.
 *
 * Frees each allocated row and then the row pointer array.
 *
 * @param gb Pointer to GameBoard to cleanup.
 */
void GameBoard_Cleanup(GameBoard *gb) {
    for (int i = 0; i < gb->boardHeight; i++) {
        free(gb->floeGrid[i]);
    }
    free(gb->floeGrid);
    gb->floeGrid = NULL;
}
