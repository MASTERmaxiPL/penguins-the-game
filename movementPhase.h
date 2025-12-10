#ifndef MOVEMENT_PHASE_H
#define MOVEMENT_PHASE_H

#include "gameBoard.h"    // Provides board structure and access to IceFloe grid
#include "gameManager.h"  // Provides GameManager used to control game flow

// Runs the movement phase until no player can make any valid move.
void MovementPhase_Run(const GameManager *gm);

// Checks if a given player has at least one penguin that can make a valid move.
bool Check_Player_Has_Any_Moves(const GameBoard *gb, int currentPlayerIndex);

// Checks if a specific penguin located at (posX, posY) has any legal movement options.
bool Check_Penguin_Has_Any_Moves(
    const GameBoard *gb,
    int posX,
    int posY,
    int boardHeight,
    int boardWidth,
    int currentPlayerIndex
);

// Returns true if the given coordinates lie inside the board boundaries.
bool Is_Move_In_Bounds(const GameBoard *gb, int x, int y);

// Validates whether a move from (startX,startY) to (endX,endY) is legal.
bool Is_Valid_Move(
    const GameBoard *gb,
    int startX,
    int startY,
    int endX,
    int endY
);

// Performs the actual movement of a penguin if the move is valid.
bool Move_Penguin(
    const GameBoard *gb,
    int startX,
    int startY,
    int endX,
    int endY
);

// Handles a single player's turn during the movement phase.
void Player_Movement_Turn(const GameBoard *gb, int currentPlayerIndex);

#endif
