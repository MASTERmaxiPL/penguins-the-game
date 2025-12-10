#include "movementPhase.h"
#include "boardGenerator.h"

#include <stdio.h>

void MovementPhase_Run(const GameManager *gm) {
    bool availableMoves = true;
    int blocked_counter = 0;
    int currentPlayerIndex = -1;
    int roundCounter = 1;

    printf("\n===== ROUND %d =====\n", roundCounter);

    do {
        currentPlayerIndex++;

        if (currentPlayerIndex >= gm->numOfPlayers) {
            currentPlayerIndex = 0;
            roundCounter++;
            printf("\n===== ROUND %d =====\n", roundCounter);
        }

        bool hasMoves = Check_Player_Has_Any_Moves(&gm->gb, currentPlayerIndex);

        if (!hasMoves) {
            blocked_counter++;
        } else {
            blocked_counter = 0;
        }

        if (blocked_counter == gm->numOfPlayers) {
            printf("\nNo players have any moves left. Game ends!\n");
            Print_Final_Scores(gm);
            availableMoves = false;
            break;
        }

        // Only let the player move if they have a valid move
        if (hasMoves) {
            Player_Movement_Turn(gm, currentPlayerIndex);
        } else {
            printf("Player %d has no moves available, skipping...\n", currentPlayerIndex + 1);
        }

    } while (availableMoves);
}


bool Check_Player_Has_Any_Moves(const GameBoard *gb, const int currentPlayerIndex) {
    bool availableMoves = false;
    for (int i = 0; i < gb->boardHeight; i++) {
        for (int j = 0; j < gb->boardWidth; j++) {
            if (gb->floeGrid[i][j].occupantId == currentPlayerIndex)
                availableMoves = Check_Penguin_Has_Any_Moves(gb, j, i, gb->boardHeight, gb->boardWidth, currentPlayerIndex);
            if (availableMoves)
                return availableMoves;
        }
    }
    return availableMoves;
}

bool Check_Penguin_Has_Any_Moves(const GameBoard *gb, int posX, int posY, int boardHeight, int boardWidth, int currentPlayerIndex)
{
    const int dirX[4] = { -1, 1, 0, 0 };
    const int dirY[4] = { 0, 0, -1, 1 };

    for (int d = 0; d < 4; d++) {
        int x = posX + dirX[d];
        int y = posY + dirY[d];

        while (x >= 0 && x < boardWidth && y >= 0 && y < boardHeight) {
            const IceFloe *f = &gb->floeGrid[y][x];

            if (!f->isFloating) break;
            if (f->occupantId != -1) break;

            return true;
        }
    }
    return false;
}


void Player_Movement_Turn(GameManager *gm, int currentPlayerIndex) {

    GameBoard *gb = &gm->gb;
    int startX, startY, endX, endY;

    while(1){
        printf("Player %d, choose penguin to move (x y): ", currentPlayerIndex + 1);
        scanf("%d %d", &startX, &startY);

        if (!Is_Move_In_Bounds(gb, startX, startY) ||
            gb->floeGrid[startY][startX].occupantId != currentPlayerIndex)
        {
            printf("Invalid penguin.\n");
            continue;
        }

        printf("Choose destination (x y): ");
        scanf("%d %d", &endX, &endY);

        if (Move_Penguin(gm, startX, startY, endX, endY)) {
            printf("Move successful!\n");
            Print_Board(gb);
            break;
        } else {
            printf("Invalid move.\n");
        }
    }
}

bool Is_Move_In_Bounds(const GameBoard *gb, const int x, const int y) {
    return x >= 0 && x < gb->boardWidth && y >= 0 && y < gb->boardHeight;
}

bool Move_Penguin(GameManager *gm, int startX, int startY, int endX, int endY) {

    GameBoard *gb = &gm->gb;

    int playerId = gb->floeGrid[startY][startX].occupantId;
    if (playerId == -1) return false;

    if (!Is_Valid_Move(gb, startX, startY, endX, endY))
        return false;

    IceFloe *start  = &gb->floeGrid[startY][startX];
    IceFloe *target = &gb->floeGrid[endY][endX];

    int collectedFish = target->fishCount;
    gm->playersScore[playerId] += collectedFish;

    printf("Player %d gains %d fish! Total = %d\n", playerId + 1, collectedFish, gm->playersScore[playerId]);

    target->occupantId = playerId;

    start->isFloating = false;
    start->fishCount = 0;
    start->occupantId = -1;
    target->fishCount = 0;

    return true;
}


bool Is_Valid_Move(const GameBoard *gb, const int startX, const int startY, const int endX, const int endY) {
    if (!Is_Move_In_Bounds(gb, startX, startY) || !Is_Move_In_Bounds(gb, endX, endY))
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

    const int stepX = differenceX > 0 ? 1 : differenceX < 0 ? -1 : 0;
    const int stepY = differenceY > 0 ? 1 : differenceY < 0 ? -1 : 0;

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

void Print_Final_Scores(const GameManager *gm) {
    printf("\n============================\n");
    printf("         FINAL SCORES       \n");
    printf("============================\n");

    for (int i = 0; i < gm->numOfPlayers; i++) {
        printf("Player %d: %d points\n", i + 1, gm->playersScore[i]);
    }

    int best = 0;
    for (int i = 1; i < gm->numOfPlayers; i++) {
        if (gm->playersScore[i] > gm->playersScore[best]){
            best = i;
        }
    }

    printf("Winner: Player %d!\n", best + 1);
    printf("============================\n\n");
}