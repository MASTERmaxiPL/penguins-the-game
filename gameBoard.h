#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "iceFloe.h"

/**
 * @file gameBoard.h
 * @brief Container for the game's grid of IceFloe cells.
 *
 * The GameBoard stores board dimensions, a count of placeable floes (1-fish floes),
 * and a 2D grid of IceFloe structures (row-major: floeGrid[row][column]).
 */

typedef struct {
    int boardWidth;         /**< width in columns */
    int boardHeight;        /**< height in rows */
    int placeableFloeCount; /**< count of tiles eligible for initial placement */
    IceFloe **floeGrid;     /**< 2D array: floeGrid[row][col] */
} GameBoard;

#endif
