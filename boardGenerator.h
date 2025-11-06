#ifndef BOARD_GENERATOR_H
#define BOARD_GENERATOR_H

#include "gameBoard.h"

void GameBoard_Init(GameBoard *gb, int boardWidth, int boardHeight);
void GameBoard_Generate(const GameBoard *gb);
void GameBoard_Print(const GameBoard *gb);
void GameBoard_Cleanup(GameBoard *gb);

#endif
