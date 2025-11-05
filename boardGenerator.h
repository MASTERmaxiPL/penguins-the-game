#ifndef BOARD_GENERATOR_H
#define BOARD_GENERATOR_H

typedef struct {
    int boardWidth, boardHeight;
} GameBoard;

void GameBoard_Init(GameBoard *gb, int boardWidth, int boardHeight);

#endif
