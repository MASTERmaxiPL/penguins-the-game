#ifndef MOVEMENT_PHASE_H
#define MOVEMENT_PHASE_H

#include "gameBoard.h"
#include "gameManager.h"

/**
 * @file movementPhase.h
 * @brief Declares all functions used during the movement phase of the game.
 *
 * This module provides logic to:
 *  - Check whether a player can move,
 *  - Validate individual penguin moves,
 *  - Execute movement,
 *  - And run the complete movement phase loop.
 */

/**
 * @brief Run the movement phase until all players become blocked.
 *
 * @param gm Pointer to the GameManager controlling the game.
 */
void MovementPhase_Run(GameManager *gm);

/**
 * @brief Determine whether a given player has at least one legal move available.
 *
 * Scans every tile for penguins owned by the player and checks
 * whether any of them can move.
 *
 * @param gb Pointer to the GameBoard.
 * @param currentPlayerIndex Index of the player being checked.
 * @return true if the player has a legal move, false otherwise.
 */
bool Check_Player_Has_Any_Moves(const GameBoard *gb, int currentPlayerIndex);

/**
 * @brief Check whether a specific penguin may have at least one valid move.
 *
 * This function performs a simplified check, only determining whether
 * an adjacent tile exists that is not occupied by the same player.
 *
 * @param gb Pointer to GameBoard.
 * @param posX Penguin X coordinate.
 * @param posY Penguin Y coordinate.
 * @param boardHeight Height of the board.
 * @param boardWidth Width of the board.
 * @param currentPlayerIndex Index of the player owning the penguin.
 * @return true if at least one potential movement direction exists.
 */
bool Check_Penguin_Has_Any_Moves(const GameBoard *gb, int posX, int posY,
                                 int boardHeight, int boardWidth,
                                 int currentPlayerIndex);

/**
 * @brief Check if a coordinate lies within the bounds of the board.
 *
 * @param gb Pointer to GameBoard.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return true if (x,y) is inside the board.
 */
bool Is_Move_In_Bounds(const GameBoard *gb, int x, int y);

/**
 * @brief Validate whether a penguin can legally move from (startX,startY) to (endX,endY).
 *
 * Enforces all official rules:
 *  - Both tiles must lie within the board.
 *  - The starting tile must contain a penguin.
 *  - Target must be floating and unoccupied.
 *  - Movement must be straight (no diagonal).
 *  - All tiles along the path must be valid and free.
 *
 * @param gb Pointer to GameBoard.
 * @param startX Starting X coordinate.
 * @param startY Starting Y coordinate.
 * * @param endX Destination X coordinate.
 * @param endY Destination Y coordinate.
 * @return true if the move satisfies all game rules.
 */
bool Is_Valid_Move(const GameBoard *gb, int startX, int startY, int endX, int endY);

/**
 * @brief Execute a penguin movement once validated.
 *
 * Transfers the penguin to the target tile, and collapses the starting tile
 * (removing its fish and making it non-floating).
 *
 * @param gm Pointer to GameManager.
 * @param startX Start X coordinate.
 * @param startY Start Y coordinate.
 * @param endX End X coordinate.
 * @param endY End Y coordinate.
 * @return true if the move is completed successfully.
 */
bool Move_Penguin(GameManager *gm, int startX, int startY, int endX, int endY);

/**
 * @brief Prompt the player to choose a penguin and destination, then attempt the move.
 *
 * Displays errors when the chosen penguin or movement is invalid.
 *
 * @param gm Pointer to GameManager.
 * @param currentPlayerIndex Index of the active player.
 */
void Player_Movement_Turn(GameManager *gm, int currentPlayerIndex);

#endif
