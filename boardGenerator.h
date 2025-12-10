#ifndef BOARD_GENERATOR_H
#define BOARD_GENERATOR_H

#include "gameBoard.h"

/**
 * @file boardGenerator.h
 * @brief Board allocation, generation and printing utilities.
 *
 * Functions to initialize and cleanup the GameBoard, generate the procedural
 * ice floe layout using FBM noise, and print the board to stdout.
 */

/**
 * @brief Initialize a GameBoard with given width and height.
 *
 * This allocates the 2D floeGrid and calls Generate_Board to fill it.
 *
 * @param gb Pointer to GameBoard to initialize.
 * @param boardWidth Number of columns.
 * @param boardHeight Number of rows.
 */
void GameBoard_Init(GameBoard *gb, int boardWidth, int boardHeight);

/**
 * @brief Generate the board contents for an already-allocated GameBoard.
 *
 * Uses FBM noise from noise.c to determine which cells are floating floes,
 * assigns fishCount (1..3) to floes, and sets occupantId = -1.
 *
 * @param gb Pointer to GameBoard to populate.
 */
void Generate_Board(GameBoard *gb);

/**
 * @brief Print an ASCII representation of the board to stdout.
 *
 * Occupied tiles print as "| Pn ", floating unoccupied as "| f  ",
 * and water as "| X  ".
 *
 * @param gb Pointer to the GameBoard to print.
 */
void Print_Board(const GameBoard *gb);

/**
 * @brief Free memory associated with a GameBoard.
 *
 * Frees internal allocations and sets floeGrid to NULL.
 *
 * @param gb Pointer to GameBoard to cleanup.
 */
void GameBoard_Cleanup(GameBoard *gb);

#endif
