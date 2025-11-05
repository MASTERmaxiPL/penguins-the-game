#ifndef BOARD_GENERATOR_H
#define BOARD_GENERATOR_H

#include "gameBoard.h"

void GameBoard_Init(GameBoard *gb, int boardWidth, int boardHeight);
void GameBoard_Generate(GameBoard *gb);
void Board_Print(const GameBoard *gb);

#endif
