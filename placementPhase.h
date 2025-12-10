#ifndef PLACEMENT_PHASE_H
#define PLACEMENT_PHASE_H

#include "gameBoard.h"    // Provides access to the IceFloe grid and board data
#include "gameManager.h"  // Provides GameManager structure for player and score handling

// Runs the entire placement phase.
// Each player places their penguins one by one in a turn-based manner.
void PlacementPhase_Run(GameManager *gm);

// Places a penguin for a given player on a chosen tile.
// playerIndex - ID of the player placing the penguin
// players     - pointer to score array for all players
// floe        - pointer to the selected IceFloe tile
// x, y        - coordinates of the selected tile
void Player_Place(int playerIndex, int *players, IceFloe *floe, int x, int y);

// Handles a single player's input and validates chosen placement coordinates.
void Player_Placement_Turn(GameManager *gm, int currentPlayerIndex);

#endif
