#include "movementPhase.h"

#include <stdio.h>

bool MovementPhase_IsInBounds(const GameBoard *gb, const int x, const int y) {
    return x >= 0 && x < gb->boardWidth && y >= 0 && y < gb->boardHeight;
}

bool MovementPhase_IsValidMove(const GameBoard *gb, const int startX, const int startY, const int endX, const int endY) {
    if (!MovementPhase_IsInBounds(gb, startX, startY) || !MovementPhase_IsInBounds(gb, endX, endY))
        return false;

    const IceFloe *start = &gb->floeGrid[startY][startX];
    const IceFloe *target = &gb->floeGrid[endY][endX];

    if (start->occupantId == -1) return false;

    if (!target->isFloating) return false;
    if (target->occupantId != -1) return false;

    const int differenceX = endX - startX;
    const int differenceY = endY - startY;

    if (differenceX != 0 && differenceY != 0)
        return false;

    const int stepX = (differenceX > 0) ? 1 : (differenceX < 0 ? -1 : 0);
    const int stepY = (differenceY > 0) ? 1 : (differenceY < 0 ? -1 : 0);

    if (stepX == 0 && stepY == 0)
        return false;

    int curentx = startX + stepX;
    int curenty = startY + stepY;

    while (curentx != endX || curenty != endY) {
        const IceFloe *f = &gb->floeGrid[curenty][curentx];
        if (!f->isFloating) return false;
        if (f->occupantId != -1) return false;
        curentx += stepX;
        curenty += stepY;
    }

    return true;
}

bool MovementPhase_MovePenguin(GameBoard gb, int startX, int startY, int endX, int endY) {
    if (!MovementPhase_IsValidMove(gb, startX, startY, endX, endY))
        return false;

    IceFloe *start = &gb->floeGrid[startY][startX];
    IceFloe *target = &gb->floeGrid[endY][endX];

    target->occupantId = start->occupantId;

    start->isFloating = false;
    start->fishCount = 0;
    start->occupantId = -1;

    return true;
}

void MovementPhase_Movement(GameBoard gb, int currentPlayer) {
    int startx, starty, endx, endy;

    printf("Player %d, choose penguin to move (x y): ", currentPlayer + 1);
    scanf("%d %d", &startx, &starty);

    if (!MovementPhase_IsInBounds(gb, startx, starty) ||
        gb->floeGrid[starty][startx].occupantId != currentPlayer) 
    {
        printf("Invalid penguin.\n");
        return;
    }

    printf("Choose destination (x y): ");
    scanf("%d %d", &endx, &endy);

    if (MovementPhase_MovePenguin(gb, startx, starty, endx, endy)) {
        printf("Move successful!\n");
    } else {
        printf("Invalid move.\n");
    }
}
