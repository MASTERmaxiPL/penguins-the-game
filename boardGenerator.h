#ifndef BOARD_GENERATOR_H
#define BOARD_GENERATOR_H

#include "gameBoard.h"   // Provides definitions for GameBoard and IceFloe structures

// Initializes the game board and allocates memory for the ice floe grid.
// boardWidth  - width of the board (number of columns)
// boardHeight - height of the board (number of rows)
void GameBoard_Init(GameBoard *gb, int boardWidth, int boardHeight);

// Generates the board tiles using noise-based terrain generation.
// Fills each tile with data: whether it floats, how many fish it has, and occupancy state.
void Generate_Board(GameBoard *gb);

// Prints the entire board to the console in a readable form.
void Print_Board(const GameBoard *gb);

// Frees all memory allocated for the board grid and resets the pointer.
void GameBoard_Cleanup(GameBoard *gb);

#endif
