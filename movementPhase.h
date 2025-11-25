#ifndef MOVEMENT_PHASE_H
#define MOVEMENT_PHASE_H

static const int DIRS[4][2];
bool GameBoard_IsInBounds(const GameBoard *gb, int x, int y);
bool GameBoard_IsValidMove(const GameBoard *gb, int sx, int sy, int ex, int ey);

#endif
