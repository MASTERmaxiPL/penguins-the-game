#include "boardGenerator.h"
#include "noise.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_FISH_COUNT 3


static void Init_Random_Seed(void) {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(nullptr) ^ (unsigned int)(uintptr_t) &seeded);
        seeded = 1;
    }
}

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

    Init_Random_Seed();
    Generate_Board(gb);
}

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
            floe->fishCount = floe->isFloating ? rand() % (MAX_FISH_COUNT + 1) : 0;
            floe->occupantId = -1;
        }
    }
}

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

void GameBoard_Cleanup(GameBoard *gb) {
    for (int i = 0; i < gb->boardHeight; i++) {
        free(gb->floeGrid[i]);
    }
    free(gb->floeGrid);
    gb->floeGrid = nullptr;
}