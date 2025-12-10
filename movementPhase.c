#include "movementPhase.h"
#include "boardGenerator.h"

#include <stdio.h>

// Runs the movement phase until all players are blocked and cannot move.
void MovementPhase_Run(const GameManager *gm) {
    bool availableMoves = true;   // Controls the movement loop
    int blocked_counter = 0;      // Counts consecutive players with no moves
    int currentPlayerIndex = -1;  // Iteration starts by incrementing to 0

    do {
        currentPlayerIndex += 1;

        // If we exceed player count, wrap around
        if (currentPlayerIndex >= gm->numOfPlayers)
            currentPlayerIndex = 0;

        // Check if the current player has any valid moves
        if (!Check_Player_Has_Any_Moves(&gm->gb, currentPlayerIndex))
            blocked_counter++;
        else
            blocked_counter = 0;

        // If all players are blocked consecutively, no more moves can be made
        if (blocked_counter == gm->numOfPlayers)
            availableMoves = false;

        // Player attempts a movement turn
        Player_Movement_Turn(&gm->gb, currentPlayerIndex);

    } while (availableMoves);
}

// Checks if any penguin of a given player can make a move
bool Check_Player_Has_Any_Moves(const GameBoard *gb, const int currentPlayerIndex) {
    bool availableMoves = false;

    // Scan the entire board
    for (int i = 0; i < gb->boardHeight; i++) {
        for (int j = 0; j < gb->boardWidth; j++) {

            // Find penguin belonging to the player
            if (gb->floeGrid[i][j].occupantId == currentPlayerIndex)
                // Check if this specific penguin has any moves
                availableMoves = Check_Penguin_Has_Any_Moves(
                    gb, j, i, gb->boardHeight, gb->boardWidth, currentPlayerIndex
                );

            // If ANY move exists, return early
            if (availableMoves)
                return availableMoves;
        }
    }

    return availableMoves;
}

// Checks whether a single penguin has at least one valid direction to move
bool Check_Penguin_Has_Any_Moves(const GameBoard *gb,
                                 const int posX, const int posY,
                                 const int boardHeight, const int boardWidth,
                                 const int currentPlayerIndex) {

    // Check left
    if (posX != 0)
        if (gb->floeGrid[posY][posX - 1].occupantId != currentPlayerIndex)
            return true;

    // Check upward
    if (posY != 0)
        if (gb->floeGrid[posY - 1][posX].occupantId != currentPlayerIndex)
            return true;

    // Check right
    if (posX != boardWidth - 1)
        if (gb->floeGrid[posY][posX + 1].occupantId != currentPlayerIndex)
            return true;

    // Check downward
    if (posY != boardHeight - 1)
        if (gb->floeGrid[posY + 1][posX].occupantId != currentPlayerIndex)
            return true;

    return false;
}

// Executes a single player's movement turn
void Player_Movement_Turn(const GameBoard *gb, const int currentPlayerIndex) {
    int startX, startY, endX, endY;

    // Select penguin position
    printf("Player %d, choose penguin to move (x y): ", currentPlayerIndex + 1);
    scanf("%d %d", &startX, &startY);

    // Validate penguin selection
    if (!Is_Move_In_Bounds(gb, startX, startY) ||
        gb->floeGrid[startY][startX].occupantId != currentPlayerIndex)
    {
        printf("Invalid penguin.\n");
        return;
    }

    // Select destination tile
    printf("Choose destination (x y): ");
    scanf("%d %d", &endX, &endY);

    // Attempt the move
    if (Move_Penguin(gb, startX, startY, endX, endY)) {
        printf("Move successful!\n");
        Print_Board(gb);
    } else {
        printf("Invalid move.\n");
    }
}

// Checks whether coordinates are inside the board boundaries
bool Is_Move_In_Bounds(const GameBoard *gb, const int x, const int y) {
    return x >= 0 && x < gb->boardWidth &&
           y >= 0 && y < gb->boardHeight;
}

// Moves penguin from start to end tile, assuming the move is valid
bool Move_Penguin(const GameBoard *gb, const int startX, const int startY, const int endX, const int endY) {
    if (!Is_Valid_Move(gb, startX, startY, endX, endY))
        return false;

    IceFloe *start = &gb->floeGrid[startY][startX];
    IceFloe *target = &gb->floeGrid[endY][endX];

    // Move penguin onto target floe
    target->occupantId = start->occupantId;

    // Remove old floe completely
    start->isFloating = false;
    start->fishCount = 0;
    start->occupantId = -1;

    return true;
}

// Validates whether a movement from (startX,startY) to (endX,endY) is legal
bool Is_Valid_Move(const GameBoard *gb, const int startX, const int startY,
                   const int endX, const int endY) {

    // Both start and end must lie within board boundaries
    if (!Is_Move_In_Bounds(gb, startX, startY) || !Is_Move_In_Bounds(gb, endX, endY))
        return false;

    const IceFloe *start = &gb->floeGrid[startY][startX];
    const IceFloe *target = &gb->floeGrid[endY][endX];

    // Starting tile must contain a penguin
    if (start->occupantId == -1)
        return false;

    // Tile must contain floating ice and no penguin
    if (!target->isFloating) return false;
    if (target->occupantId != -1) return false;

    // Movement must be straight (horizontal or vertical)
    const int differenceX = endX - startX;
    const int differenceY = endY - startY;

    if (differenceX != 0 && differenceY != 0)
        return false;

    // Determine step direction (+1, -1, or 0)
    const int stepX = (differenceX > 0) ? 1 : (differenceX < 0 ? -1 : 0);
    const int stepY = (differenceY > 0) ? 1 : (differenceY < 0 ? -1 : 0);

    // Cannot stay on the same tile
    if (stepX == 0 && stepY == 0)
        return false;

    // Check intermediate tiles between start and destination
    int currentX = startX + stepX;
    int currentY = startY + stepY;

    while (currentX != endX || currentY != endY) {
        const IceFloe *f = &gb->floeGrid[currentY][currentX];

        if (!f->isFloating) return false; // Must be ice floe
        if (f->occupantId != -1) return false; // Can't jump over penguins

        currentX += stepX;
        currentY += stepY;
    }

    return true;
}
