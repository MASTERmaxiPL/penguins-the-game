#include "movementPhase.h"

bool MovementPhase_IsInBounds(const GameBoard *gb, int x, int y) {
    return x >= 0 && x < gb->boardWidth && y >= 0 && y < gb->boardHeight;
}

bool MovementPhase_IsValidMove(const GameBoard *gb, int startx, int starty, int endx, int endy) {
    if (!MovementPhase_IsInBounds(gb, startx, starty) || !MovementPhase_IsInBounds(gb, endx, endy))
        return false;

    const IceFloe *start = &gb->floeGrid[starty][startx];
    const IceFloe *target = &gb->floeGrid[endy][endx];

    if (start->occupantId == -1) return false;

    if (!target->isFloating) return false;
    if (target->occupantId != -1) return false;

    int differencex = endx - startx;
    int differencey = endy - starty;

    if (differencex != 0 && differencey != 0)
        return false;

    int stepx = (differencex > 0) ? 1 : (differencex < 0 ? -1 : 0);
    int stepy = (differencey > 0) ? 1 : (differencey < 0 ? -1 : 0);

    if (stepx == 0 && stepy == 0)
        return false;

    int curentx = startx + stepx;
    int curenty = starty + stepy;

    while (curentx != endx || curenty != endy) {
        const IceFloe *f = &gb->floeGrid[curenty][curentx];
        if (!f->isFloating) return false;
        if (f->occupantId != -1) return false;
        curentx += stepx;
        curenty += stepy;
    }

    return true;
}
