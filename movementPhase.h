#ifndef MOVEMENT_PHASE_H
#define MOVEMENT_PHASE_H

#include "gameManager.h"

typedef struct {
    bool isFloating;
    int fishCount;
    int occupantId;
} IceFloe;

typedef struct {
    int boardWidth;
    int boardHeight;
    IceFloe **floeGrid;
} GameBoard;

bool MovementPhase_IsInBounds(const GameBoard *gb, int x, int y);
bool MovementPhase_IsValidMove(const GameBoard *gb, int startx, int starty, int endx, int endy);
bool MovementPhase_MovePenguin(GameBoard gb, int startx, int starty, int endx, int endy);
void MovementPhase_Movement(GameBoardgb, int currentPlayer);

#endif
