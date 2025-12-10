#ifndef MOVEMENT_PHASE_H
#define MOVEMENT_PHASE_H

#include "gameBoard.h"
#include <stdbool.h>

/**
 * @file movementPhase.h
 * @brief Movement phase API: checking and executing penguin moves.
 */

/**
 * @brief Run the movement phase for all players until no moves remain.
 *
 * This function drives the round-robin moves until every player has no move.
 *
 * @param gm Pointer to GameManager (const).
 */
void MovementPhase_Run(const GameManager *gm);

/**
 * @brief Return true if the given player has any legal moves on the board.
 *
 * @param gb Pointer to GameBoard to inspect.
 * @param currentPlayerIndex Index of player to check.
 * @return true if at least one move exists for that player.
 */
bool Check_Player_Has_Any_Moves(const GameBoard *gb, const int currentPlayerIndex);

/**
 * @brief Check whether a single penguin at (posX,posY) has any moves.
 *
 * This function performs a light-weight check to see if there is at least one
 * direction with potential moves (note: more robust check uses Is_Valid_Move).
 *
 * @param gb Pointer to GameBoard.
 * @param posX X coordinate (column).
 * @param posY Y coordinate (row).
 * @param boardHeight Board height (rows).
 * @param boardWidth Board width (columns).
 * @param currentPlayerIndex Player index owning the penguin.
 * @return true if any move exists from this penguin.
 */
bool Check_Penguin_Has_Any_Moves(const GameBoard *gb, const int posX, const int posY, const int boardHeight, const int boardWidth, const int currentPlayerIndex);

/**
 * @brief Prompt and execute a player's movement turn.
 *
 * Interactive: asks user for start and end coordinates, validates and moves penguin.
 *
 * @param gb Pointer to GameBoard.
 * @param currentPlayerIndex Index of the active player.
 */
void Player_Movement_Turn(const GameBoard *gb, const int currentPlayerIndex);

/**
 * @brief Check move coordinates are in board bounds.
 *
 * @param gb Pointer to GameBoard.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return true if (x,y) is a valid cell in the board.
 */
bool Is_Move_In_Bounds(const GameBoard *gb, const int x, const int y);

/**
 * @brief Validate whether a penguin may move from start to end.
 *
 * Ensures straight-line motion, path is clear, source has a penguin, target is floating and free.
 *
 * @param gb Pointer to GameBoard.
 * @param startX Starting X.
 * @param startY Starting Y.
 * @param endX Destination X.
 * @param endY Destination Y.
 * @return true if the move is valid.
 */
bool Is_Valid_Move(const GameBoard *gb, const int startX, const int startY, const int endX, const int endY);

/**
 * @brief Execute a validated penguin move: transfer occupant and remove start floe.
 *
 * @param gb Pointer to GameBoard.
 * @param startX Starting X.
 * @param startY Starting Y.
 * @param endX Destination X.
 * @param endY Destination Y.
 * @return true if move executed.
 */
bool Move_Penguin(const GameBoard *gb, const int startX, const int startY, const int endX, const int endY);

#endif
