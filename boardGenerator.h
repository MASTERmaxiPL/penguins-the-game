#ifndef BOARD_GENERATOR_H
#define BOARD_GENERATOR_H

#include "gameBoard.h"

/**
 * @file boardGenerator.h
 * @brief Declarations for board creation, procedural generation, printing,
 *        and cleanup routines used in the penguin game.
 *
 * This module manages the life cycle of the GameBoard: allocation,
 * generating floating tiles and fish values, rendering the board, and freeing
 * all associated memory.
 */

/**
 * @brief Allocate and initialize the GameBoard structure.
 *
 * Allocates the 2D array of IceFloe rows and columns and then calls
 * Generate_Board() to populate the board with floating tiles and fish counts.
 *
 * @param gb Pointer to the GameBoard to initialize.
 * @param boardWidth Width of the board in tiles.
 * @param boardHeight Height of the board in tiles.
 */
void GameBoard_Init(GameBoard *gb, int boardWidth, int boardHeight);

/**
 * @brief Procedurally generate the ice board layout.
 *
 * Uses FBM noise, radial falloff, and random jitter to determine whether
 * tiles are floating or water. Assigns fish counts and tracks tiles with
 * exactly one fish for the placement phase.
 *
 * @param gb Pointer to the GameBoard to populate.
 */
void Generate_Board(GameBoard *gb);

/**
 * @brief Print the board as an ASCII grid.
 *
 * Floating tiles display their fish count, occupied tiles display the player
 * number, and water tiles display 'X'.
 *
 * @param gb Pointer to the GameBoard to print.
 */
void Print_Board(const GameBoard *gb);

/**
 * @brief Free all memory associated with the GameBoard.
 *
 * Releases each allocated row and the parent pointer array.
 *
 * @param gb Pointer to the GameBoard to clean up.
 */
void GameBoard_Cleanup(GameBoard *gb);

#endif
