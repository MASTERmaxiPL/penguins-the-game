#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "movementPhase.h"

static const int DIRS[4][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};

bool GameBoard_IsInBounds(const GameBoard *gb, int x, int y) {
    return x >= 0 && x < gb->boardWidth && y >= 0 && y < gb->boardHeight;
}

bool GameBoard_IsValidMove(const GameBoard *gb, int sx, int sy, int ex, int ey) {
    if (!GameBoard_IsInBounds(gb, sx, sy) || !GameBoard_IsInBounds(gb, ex, ey))
        return false;

    const IceFloe *start = &gb->floeGrid[sy][sx];
    const IceFloe *target = &gb->floeGrid[ey][ex];

    if (start->occupantId == -1) return false;
    if (!target->isFloating) return false;
    if (target->occupantId != -1) return false;

    int dx = ex - sx;
    int dy = ey - sy;

    for (int i = 0; i < 4; i++) {
        int stepx = DIRS[i][0];
        int stepy = DIRS[i][1];

        if (dx == 0 && stepx != 0) continue;
        if (dy == 0 && stepy != 0) continue;

        if (stepx != 0 && dx % stepx != 0) continue;
        if (stepy != 0 && dy % stepy != 0) continue;

        int steps = (stepx != 0) ? dx / stepx : dy / stepy;
        if (steps <= 0) continue;

        int cx = sx, cy = ey;
        for (int s = 1; s < steps; s++) {
            cx += stepx;
            cy += stepy;
            const IceFloe *f = &gb->floeGrid[cy][cx];

            if (!f->isFloating || f->occupantId != -1) return false;
        }

        return true;
    }

    return false;
}

bool GameBoard_MovePenguin(GameBoard *gb, int sx, int sy, int ex, int ey) {
    if (!GameBoard_IsValidMove(gb, sx, sy, ex, ey))
        return false;

    IceFloe *start = &gb->floeGrid[sy][sx];
    IceFloe *target = &gb->floeGrid[ey][ex];

    target->occupantId = start->occupantId;

    start->isFloating = false;
    start->fishCount = 0;
    start->occupantId = -1;

    return true;
}

void GameBoard_MovementPhase(GameBoard *gb, int currentPlayer) {
    int sx, sy, ex, ey;

    printf("Player %d, choose penguin to move (x y): ", currentPlayer + 1);
    scanf("%d %d", &sx, &sy);

    if (!GameBoard_IsInBounds(gb, sx, sy) ||
        gb->floeGrid[sy][sx].occupantId != currentPlayer) 
    {
        printf("Invalid penguin.\n");
        return;
    }

    printf("Choose destination (x y): ");
    scanf("%d %d", &ex, &ey);

    if (GameBoard_MovePenguin(gb, sx, sy, ex, ey)) {
        printf("Move successful!\n");
    } else {
        printf("Invalid move.\n");
    }
}
