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

    int currentX = startX + stepX;
    int currentY = startY + stepY;

    while (currentX != endX || currentY != endY) {
        const IceFloe *f = &gb->floeGrid[currentY][currentX];
        if (!f->isFloating) return false;
        if (f->occupantId != -1) return false;
        currentX += stepX;
        currentY += stepY;
    }

    return true;
}

bool MovementPhase_MovePenguin(GameBoard *gb, const int startX, const int startY, const int endX, const int endY) {
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

void MovementPhase_Movement(GameBoard *gb, const int currentPlayer) {
    int startX, startY, endX, endY;

    printf("Player %d, choose penguin to move (x y): ", currentPlayer + 1);
    scanf("%d %d", &startX, &startY);

    if (!MovementPhase_IsInBounds(gb, startX, startY) ||
        gb->floeGrid[startY][startX].occupantId != currentPlayer)
    {
        printf("Invalid penguin.\n");
        return;
    }

    printf("Choose destination (x y): ");
    scanf("%d %d", &endX, &endY);

    if (MovementPhase_MovePenguin(gb, startX, startY, endX, endY)) {
        printf("Move successful!\n");
    } else {
        printf("Invalid move.\n");
    }
}
