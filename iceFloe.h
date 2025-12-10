#ifndef ICE_FLOE_H
#define ICE_FLOE_H

/**
 * @file iceFloe.h
 * @brief Representation of a single ice floe tile on the board.
 *
 * Each IceFloe indicates whether it exists (isFloating), how many fish it has,
 * and which player (if any) occupies it.
 */

typedef struct {
    bool isFloating; /**< true when there is an ice floe at this cell */
    int fishCount;   /**< number of fish currently on this floe (0..3) */
    int occupantId;  /**< -1 if empty; otherwise player index (0..n-1) */
} IceFloe;

#endif
