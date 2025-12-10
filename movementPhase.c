#include "movementPhase.h"
#include "boardGenerator.h"

#include <stdio.h>

/**
 * @file movementPhase.c
 * @brief Movement phase implementation: turn loop, checking and executing moves.
 */

/**
 * @brief Run the movement phase until no players can move.
 *
 * Uses a blocked counter to detect when all players are consecutively blocked.
 *
 * @param gm Pointer to GameManager (const).
 */
void MovementPhase_Run(const GameManager *gm) {
    bool availableMoves = true;
    int blocked_counter = 0;
    int currentPlayerIndex = -1;

    do{
        currentPlayerIndex+=1;
        if (currentPlayerIndex >= gm->numOfPlayers)
            currentPlayerIndex = 0;

        if (!Check_Player_Has_Any_Moves(&gm->gb, currentPlayerIndex))
            blocked_counter++;
        else {
            blocked_counter = 0;
        }
        if (blocked_counter == gm->numOfPlayers)
            availableMoves = false;

        Player_Movement_Turn(&gm->gb, currentPlayerIndex);
    } while(availableMoves);
}

/**
 * @brief Scan board for any penguin owned by current player that has a move.
 *
 * Returns true early when a valid move is detected.
 *
 * @param gb Pointer to GameBoard.
 * @param currentPlayerIndex Player index to check.
 * @return true if any move exists.
 */
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

/**
 * @brief Quick-check whether a penguin may move from the given position.
 *
 * This checks immediate neighbors for non-blocking occupancy. It is a
 * conservative, cheap test — detailed move validity is done in Is_Valid_Move.
 *
 * @param gb Pointer to GameBoard.
 * @param posX Column of penguin.
 * @param posY Row of penguin.
 * @param boardHeight Board height.
 * @param boardWidth Board width.
 * @param currentPlayerIndex Player index.
 * @return true if an adjacent tile suggests a possible move.
 */
bool Check_Penguin_Has_Any_Moves(const GameBoard *gb, const int posX, const int posY, const int boardHeight, const int boardWidth, const int currentPlayerIndex) {
    if (posX != 0)
        if (gb->floeGrid[posY][posX-1].occupantId != currentPlayerIndex)
            return true;
    if (posY != 0)
        if (gb->floeGrid[posY-1][posX].occupantId != currentPlayerIndex)
            return true;
    if (posX != boardWidth-1)
        if (gb->floeGrid[posX+1][posY].occupantId != currentPlayerIndex)
            return true;
    if (posY != boardHeight-1)
        if (gb->floeGrid[posX][posY+1].occupantId != currentPlayerIndex)
            return true;
    return false;
}

/**
 * @brief Interactive movement turn for the given player.
 *
 * Prompts for start coordinates and destination, validates and executes move.
 *
 * @param gb Pointer to GameBoard.
 * @param currentPlayerIndex Player whose turn it is.
 */
void Player_Movement_Turn(const GameBoard *gb, const int currentPlayerIndex) {
    int startX, startY, endX, endY;

    printf("Player %d, choose penguin to move (x y): ", currentPlayerIndex + 1);
    scanf("%d %d", &startX, &startY);

    if (!Is_Move_In_Bounds(gb, startX, startY) ||
        gb->floeGrid[startY][startX].occupantId != currentPlayerIndex)
    {
        printf("Invalid penguin.\n");
        return;
    }

    printf("Choose destination (x y): ");
    scanf("%d %d", &endX, &endY);

    if (Move_Penguin(gb, startX, startY, endX, endY)) {
        printf("Move successful!\n");
        Print_Board(gb);
    } else {
        printf("Invalid move.\n");
    }
}

/**
 * @brief Check that coordinates are inside the board bounds.
 *
 * @param gb Pointer to GameBoard.
 * @param x X coordinate (column).
 * @param y Y coordinate (row).
 * @return true if within bounds.
 */
bool Is_Move_In_Bounds(const GameBoard *gb, const int x, const int y) {
    return x >= 0 && x < gb->boardWidth && y >= 0 && y < gb->boardHeight;
}

/**
 * @brief Validate a straight-line penguin move from start to end.
 *
 * Verifies bounds, presence of penguin at start, target is floating and free,
 * movement is axis-aligned, and every intermediate tile is floating and free.
 *
 * @param gb Pointer to GameBoard.
 * @param startX Start X coordinate.
 * @param startY Start Y coordinate.
 * @param endX End X coordinate.
 * @param endY End Y coordinate.
 * @return true if the move is valid.
 */
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

/**
 * @brief Execute a validated penguin move: set target occupant and remove start floe.
 *
 * @param gb Pointer to GameBoard.
 * @param startX Start X coordinate.
 * @param startY Start Y coordinate.
 * @param endX End X coordinate.
 * @param endY End Y coordinate.
 * @return true if move succeeded.
 */
bool Move_Penguin(const GameBoard *gb, const int startX, const int startY, const int endX, const int endY) {
    if (!Is_Valid_Move(gb, startX, startY, endX, endY))
        return false;

    IceFloe *start = &gb->floeGrid[startY][startX];
    IceFloe *target = &gb->floeGrid[endY][endX];

    target->occupantId = start->occupantId;

    start->isFloating = false;
    start->fishCount = 0;
    start->occupantId = -1;

    return true;
}
