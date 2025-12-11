#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "iceFloe.h"

/**
 * @file gameBoard.h
 * @brief Defines the GameBoard structure, which stores the entire ice floe grid.
 *
 * The GameBoard tracks board dimensions, the number of floes eligible for
 * penguin placement (floes containing exactly one fish), and a dynamically
 * allocated 2D grid of IceFloe structures.
 */

/**
 * @struct GameBoard
 * @brief Container for all ice floes and board metadata.
 *
 * This struct represents the entire playfield for the penguin game.
 */
typedef struct {
    int boardWidth;          /**< Width of the board in tiles (columns). */
    int boardHeight;         /**< Height of the board in tiles (rows). */
    int placeableFloeCount;  /**< Number of floes with exactly 1 fish (used in placement phase). */
    IceFloe **floeGrid;      /**< 2D dynamic array of IceFloe pointers: floeGrid[row][column]. */
} GameBoard;

#endif
