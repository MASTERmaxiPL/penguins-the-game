#ifndef ICE_FLOE_H
#define ICE_FLOE_H

// Represents a single tile on the game board.
// isFloating  - true if this tile contains an ice floe (false = water)
// fishCount   - number of fish on the ice floe (0–3 depending on generation)
// occupantId  - ID of the penguin standing on this tile (-1 means no penguin)
typedef struct {
    bool isFloating;   // Indicates whether this tile is an ice floe
    int fishCount;     // Number of fish present on the tile
    int occupantId;    // Player ID of occupying penguin, or -1 if empty
} IceFloe;

#endif
