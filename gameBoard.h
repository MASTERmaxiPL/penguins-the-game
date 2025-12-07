#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "iceFloe.h"

typedef struct {
    int boardWidth;
    int boardHeight;
    int placeableFloeCount;
    IceFloe **floeGrid;
} GameBoard;

#endif
