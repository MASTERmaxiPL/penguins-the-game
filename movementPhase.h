#ifndef MOVEMENT_PHASE_H
#define MOVEMENT_PHASE_H

#include "gameBoard.h"
#include "gameManager.h"

bool MovementPhase_IsInBounds(const GameBoard *gb, int x, int y);
bool MovementPhase_IsValidMove(const GameBoard *gb, int startx, int starty, int endx, int endy);
bool MovementPhase_MovePenguin(GameBoard gb, int startx, int starty, int endx, int endy);
void MovementPhase_Movement(GameBoard gb, int currentPlayer);

#endif
