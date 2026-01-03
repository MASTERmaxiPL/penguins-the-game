#ifndef PLACEMENT_PHASE_H
#define PLACEMENT_PHASE_H

#include "../core/gameBoard.h"
#include "../core/gameManager.h"

/**
 * @file placementPhase.h
 * @brief Declares functions responsible for the placement phase of the game.
 *
 * During the placement phase, players place their penguins on valid tiles
 * (floating, unoccupied, 1-fish floes) and collect initial fish.
 */

/**
 * @brief Run the complete placement phase.
 *
 * Handles multiple rounds of placement—each player places one penguin per
 * round until they have placed the required number.
 *
 * @param gm Pointer to the GameManager controlling the game.
 * @param isLoadedGame Indicates if the game was loaded from a saved state.
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus PlacementPhase_Run(GameManager *gm, bool isLoadedGame);

/**
 * @brief Place a penguin on the selected floe and award fish to the player.
 *
 * Updates:
 *  - The floe's occupant ID,
 *  - The player's accumulated fish,
 *  - Outputs a message describing the action.
 *
 * @param playerIndex Index of the player placing the penguin.
 * @param players Pointer to the players' score array.
 * @param floe Pointer to the target IceFloe.
 * @param x X-coordinate of the placement.
 * @param y Y-coordinate of the placement.
 */
void Player_Place(int playerIndex, int *players, IceFloe *floe, int x, int y);

/**
 * @brief Handle interaction for a single player's placement turn.
 *
 * Continuously prompts for coordinates until a valid tile is chosen:
 *  - Must be within bounds,
 *  - Must be floating,
 *  - Must contain exactly 1 fish,
 *  - Must be unoccupied.
 *
 * @param gm Pointer to the GameManager.
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus Player_Placement_Turn(GameManager *gm);

#endif
