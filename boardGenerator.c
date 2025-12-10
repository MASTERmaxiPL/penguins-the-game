#include "boardGenerator.h"
#include "noise.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // NOLINT - suppress 'possibly unused #include' in IDE (clang-tidy/clangd)
#include <time.h>
#include <math.h>

#define MAX_FISH_COUNT 3   // Maximum number of fish assigned to a floating ice floe

// Initializes the random number generator only once per program run
static void Init_Random_Seed() {
    static int seeded = 0;   // Ensures srand() is called only once

    if (!seeded) {
        // Seed randomness with current time XOR memory address
        srand((unsigned int)time(nullptr) ^ (unsigned int)(uintptr_t)&seeded);
        seeded = 1;
    }
}

void GameBoard_Init(GameBoard *gb, const int boardWidth, const int boardHeight) {
    gb->boardWidth = boardWidth;    // Store width of the board
    gb->boardHeight = boardHeight;  // Store height of the board
    gb->floeGrid = nullptr;         // Initialize grid pointer

    // Allocate rows for the 2D array
    gb->floeGrid = malloc(boardHeight * sizeof(IceFloe *));
    for (int i = 0; i < boardHeight; i++) {
        // Allocate columns for each row
        gb->floeGrid[i] = malloc(boardWidth * sizeof(IceFloe));
    }

    // Check for allocation failure
    if (!gb->floeGrid) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    Init_Random_Seed();   // Ensure random number generator is initialized
    Generate_Board(gb);   // Create a noise-based game board
}

void Generate_Board(GameBoard *gb) {
    // Configuration parameters for FBM (fractal Brownian motion)
    const int fbmOctaves = 5;
    const double fbmLacunarity = 2.0;
    const double fbmGain = 0.5;
    const unsigned int fbmNoiseSeed = (unsigned int)rand();

    // Dimensions and scaling factors for noise sampling
    const double width = gb->boardWidth;
    const double height = gb->boardHeight;
    const double minDim = width < height ? width : height;

    const double noiseScale = minDim / 5.6;       // Controls size of islands
    const double edgeFalloffExponent = 2;         // Controls circular landscape fade-out
    const double threshold = -0.04;               // Determines ice vs. water threshold
    const double maxRandomJitter = 0.15;          // Adds randomness for natural shapes

    int placeableFloe = 0; // Count tiles with exactly 1 fish (used in placement phase)

    // Generate floating ice and fish distribution
    for (int y = 0; y < gb->boardHeight; y++) {
        const double noiseY = ((double)y - height * 0.5) / noiseScale;
        const double centeredOffsetY = ((double)y - height * 0.5) / (minDim * 0.5);

        for (int x = 0; x < gb->boardWidth; x++) {
            IceFloe *floe = &gb->floeGrid[y][x];

            const double noiseX = ((double)x - width * 0.5) / noiseScale;
            const double centeredOffsetX = ((double)x - width * 0.5) / (minDim * 0.5);

            // Apply FBM noise
            const double noiseValue = Fbm(
                noiseX, noiseY,
                fbmOctaves,
                fbmLacunarity,
                fbmGain,
                fbmNoiseSeed
            );

            // Distance from center → creates circular map shape
            const double radialNorm =
                sqrt(centeredOffsetX * centeredOffsetX + centeredOffsetY * centeredOffsetY);

            const double falloff = pow(radialNorm, edgeFalloffExponent);

            // Random jitter to soften edges
            const double randomUnit = (double)rand() / (double)RAND_MAX;
            const double randomJitter = (randomUnit - 0.5) * maxRandomJitter;

            // Final terrain value
            const double terrainValue = noiseValue - falloff + randomJitter;

            // Determine whether a tile is floating
            floe->isFloating = (terrainValue > threshold) ? 1 : 0;

            // Assign 1–MAX_FISH_COUNT fish if floating, otherwise 0
            floe->fishCount =
                floe->isFloating ? rand() % MAX_FISH_COUNT + 1 : 0;

            floe->occupantId = -1; // No penguin on the tile initially

            // Count single-fish floes for placement phase rules
            if (floe->fishCount == 1) {
                placeableFloe += 1;
            }
        }
    }

    // Save number of floes where penguins may be placed
    gb->placeableFloeCount = placeableFloe;
}

void Print_Board(const GameBoard *gb) {
    // Print column index header
    printf("   ");
    for (int x = 0; x < gb->boardWidth; x++) {
        if (x < 10)
            printf("  %d  ", x);
        else
            printf(" %3d ", x);
    }
    printf("\n");

    // Print board contents row by row
    for (int y = 0; y < gb->boardHeight; y++) {
        printf("%2d ", y); // Row index

        for (int x = 0; x < gb->boardWidth; x++) {
            const IceFloe *floe = &gb->floeGrid[y][x];

            if (floe->occupantId != -1) {
                printf("| P%d ", floe->occupantId + 1); // Penguin present
            }
            else if (floe->isFloating) {
                printf("| %d  ", floe->fishCount);       // Ice floe with fish
            }
            else {
                printf("| X  ");                          // Water tile
            }
        }

        printf("| \n");
    }
}

void GameBoard_Cleanup(GameBoard *gb) {
    // Free each row of the grid
    for (int i = 0; i < gb->boardHeight; i++) {
        free(gb->floeGrid[i]);
    }

    // Free array of row pointers
    free(gb->floeGrid);
    gb->floeGrid = nullptr; // Avoid dangling pointer
}
