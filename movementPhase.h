#ifndef MOVEMENT_PHASE_H
#define MOVEMENT_PHASE_H

#include "gameBoard.h"
#include "gameManager.h"


void MovementPhase_Run(GameManager *gm);
bool Check_Player_Has_Any_Moves(const GameBoard *gb, int currentPlayerIndex);
bool Check_Penguin_Has_Any_Moves(const GameBoard *gb, int posX, int posY, int boardHeight, int boardWidth, int currentPlayerIndex);
bool Is_Move_In_Bounds(const GameBoard *gb, int x, int y);
bool Is_Valid_Move(const GameBoard *gb, int startX, int startY, int endX, int endY);
bool Move_Penguin(GameManager *gm, int startX, int startY, int endX, int endY);
void Player_Movement_Turn(GameManager *gm, int currentPlayerIndex);
void Print_Final_Scores(const GameManager *gm);

#endif