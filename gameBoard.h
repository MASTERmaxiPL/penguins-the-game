#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "iceFloe.h"   // Provides the definition of the IceFloe structure

// Represents the full game board used for the penguin game.
// boardWidth         - number of columns in the board
// boardHeight        - number of rows in the board
// placeableFloeCount - number of tiles with exactly 1 fish (used to determine penguin limits)
// floeGrid           - dynamically allocated 2D array storing IceFloe tiles
typedef struct {
    int boardWidth;          // Width of the board
    int boardHeight;         // Height of the board
    int placeableFloeCount;  // Number of ice floes with exactly 1 fish
    IceFloe **floeGrid;      // Pointer to 2D grid of IceFloe structures
} GameBoard;

#endif
