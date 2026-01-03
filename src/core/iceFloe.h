#ifndef ICE_FLOE_H
#define ICE_FLOE_H

/**
 * @file iceFloe.h
 * @brief Defines the IceFloe structure, representing a single tile on the board.
 *
 * An IceFloe can be either floating (usable), or water (non-floating).
 * A floating floe may contain fish and optionally a penguin belonging to a player.
 */

/**
 * @struct IceFloe
 * @brief Represents a single ice tile in the game grid.
 *
 * Each tile stores whether it exists, how many fish it contains,
 * and whether a player's penguin occupies it.
 */
typedef struct {
    bool isFloating; /**< Whether this tile is a floating ice floe (true) or water (false). */
    int fishCount;   /**< Number of fish currently present (0–3 depending on generation rules). */
    int occupantId;  /**< Player index occupying the tile, or -1 if unoccupied. */
} IceFloe;

#endif
