#ifndef PLACEMENT_PHASE_H
#define PLACEMENT_PHASE_H

#include "gameManager.h"

/**
 * @file placementPhase.h
 * @brief Placement phase APIs for initial penguin placement.
 */

/**
 * @brief Run the placement phase for all players.
 *
 * Players take turns placing penguins on floes that have exactly 1 fish.
 *
 * @param gm Pointer to GameManager that stores board and scores.
 */
void PlacementPhase_Run(GameManager *gm);

/**
 * @brief Handle a single player's placement turn (interactive).
 *
 * Prompts the player for coordinates, validates, and places the penguin.
 *
 * @param gm Pointer to GameManager.
 * @param currentPlayerIndex Index of the player placing.
 */
void Player_Placement_Turn(GameManager *gm, const int currentPlayerIndex);

/**
 * @brief Apply placement: assign occupant and update player score.
 *
 * @param playerIndex Index of player placing.
 * @param players Array of player scores (length numOfPlayers).
 * @param floe Pointer to target IceFloe.
 * @param x X coordinate of floe.
 * @param y Y coordinate of floe.
 */
void Player_Place(const int playerIndex, int *players, IceFloe *floe, const int x, const int y);

#endif
