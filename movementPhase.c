/**
 * @file movementPhase.c
 * @brief Implements logic for checking available moves, validating movement,
 *        executing penguin movement, and running the full movement phase loop.
 */

#include "movementPhase.h"
#include "boardGenerator.h"

#include <stdio.h>

/**
 * @brief Run the movement phase until all players are blocked.
 *
 * The phase proceeds in player order. Each player attempts a move; if a player
 * has no valid moves, they are counted as blocked. When all players are
 * consecutively blocked, the movement phase ends.
 *
 * @param gm Pointer to the GameManager (read-only).
 */
void MovementPhase_Run(GameManager *gm) {
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

        if (!Check_Player_Has_Any_Moves(&gm->gb, currentPlayerIndex)) {
            blocked_counter++;
            printf("Player %d has no moves available, skipping...\n", currentPlayerIndex + 1);
        } else {
            blocked_counter = 0;
            Player_Movement_Turn(gm, currentPlayerIndex);
        }

        if (blocked_counter == gm->numOfPlayers) {
            printf("\nNo players have any moves left. Game ends!\n");
            break;
        }
    } while (true);
}

/**
 * @brief Check whether a given player has *any* legal movement remaining.
 *
 * Iterates over all tiles, locating all penguins belonging to the player.
 * For each penguin, checks whether it has possible movement directions.
 *
 * @param gb Pointer to the GameBoard.
 * @param currentPlayerIndex Index of the player to check.
 * @return true if the player has at least one legal move.
 */
bool Check_Player_Has_Any_Moves(const GameBoard *gb, const int currentPlayerIndex) {
    bool availableMoves = false;
    for (int i = 0; i < gb->boardHeight; i++) {
        for (int j = 0; j < gb->boardWidth; j++) {
            if (gb->floeGrid[i][j].occupantId == currentPlayerIndex)
                availableMoves = Check_Penguin_Has_Any_Moves(gb, j, i, gb->boardHeight, gb->boardWidth);
            if (availableMoves)
                return availableMoves;
        }
    }
    return availableMoves;
}

/**
 * @brief Quick heuristic to determine if a penguin may have any moves.
 *
 * This does **not** fully validate moves — it only checks whether there is at
 * least one adjacent tile not occupied by the same player. Full validation is
 * handled by Is_Valid_Move().
 *
 * @param gb Pointer to GameBoard.
 * @param posX Penguin X position.
 * @param posY Penguin Y position.
 * @param boardHeight Height of board.
 * @param boardWidth Width of board.
 * @return true if at least one possible move direction exists.
 */
bool Check_Penguin_Has_Any_Moves(const GameBoard *gb, const int posX, const int posY, const int boardHeight, const int boardWidth)
{
    if (posX != 0)
        if (gb->floeGrid[posY][posX-1].occupantId ==-1 && gb->floeGrid[posY][posX-1].isFloating)
            return true;
    if (posY != 0)
        if (gb->floeGrid[posY-1][posX].occupantId ==-1 && gb->floeGrid[posY-1][posX].isFloating)
            return true;
    if (posX != boardWidth-1)
        if (gb->floeGrid[posX+1][posY].occupantId ==-1 && gb->floeGrid[posX+1][posY].isFloating)
            return true;
    if (posY != boardHeight-1)
        if (gb->floeGrid[posX][posY+1].occupantId ==-1 && gb->floeGrid[posX][posY+1].isFloating)
            return true;
    return false;
}

/**
 * @brief Execute one movement turn for the specified player.
 *
 * Prompts the user to:
 *  - Select a penguin to move,
 *  - Select a destination tile.
 *
 * The move is validated with Is_Valid_Move(). If legal, Move_Penguin() is
 * executed and the updated board is displayed.
 *
 * @param gm Pointer to GameManager.
 * @param currentPlayerIndex Index of the active player.
 */
void Player_Movement_Turn(GameManager *gm, const int currentPlayerIndex) {
    const GameBoard *gb = &gm->gb;

    int startX, startY, endX, endY;

    while(1){
        printf("Player %d, choose penguin to move (x y): ", currentPlayerIndex + 1);
        const int inputCount = scanf("%d %d", &startX, &startY);

        if (inputCount != 2) {
            printf("Invalid input! Please enter two integers.\n");
            while (getchar() != '\n'){}
            continue;
        }

        if (!Is_Move_In_Bounds(gb, startX, startY) ||
            gb->floeGrid[startY][startX].occupantId != currentPlayerIndex)
        {
            printf("Invalid penguin.\n");
            break;
        }

        printf("Choose destination (x y): ");
        scanf("%d %d", &endX, &endY);

        if (Move_Penguin(gm, startX, startY, endX, endY)) {
            printf("Move successful!\n");
            Print_Board(gb);
            break;
        }
        printf("Invalid move.\n");
    }
}

/**
 * @brief Check whether the given coordinates lie within the board.
 *
 * @param gb Pointer to GameBoard.
 * @param x X-coordinate to check.
 * @param y Y-coordinate to check.
 * @return true if (x,y) lies inside the board dimensions.
 */
bool Is_Move_In_Bounds(const GameBoard *gb, const int x, const int y) {
    return x >= 0 && x < gb->boardWidth && y >= 0 && y < gb->boardHeight;
}

/**
 * @brief Execute a penguin move after validating it.
 *
 * Transfers the penguin from the start tile to the target tile and removes
 * the starting floe (making it water).
 *
 * @param gm Pointer to GameManager.
 * @param startX Source X position.
 * @param startY Source Y position.
 * @param endX Destination X position.
 * @param endY Destination Y position.
 * @return true if the move was legal and executed successfully.
 */
bool Move_Penguin(GameManager *gm, const int startX, const int startY, const int endX, const int endY) {
    const GameBoard *gb = &gm->gb;

    const int playerId = gb->floeGrid[startY][startX].occupantId;
    if (playerId == -1) return false;
    
    if (!Is_Valid_Move(gb, startX, startY, endX, endY))
        return false;

    IceFloe *start  = &gb->floeGrid[startY][startX];
    IceFloe *target = &gb->floeGrid[endY][endX];

    const int collectedFish = target->fishCount;
    gm->playersScore[playerId] += collectedFish;

    printf("Player %d gains %d fish! Total = %d\n", playerId + 1, collectedFish, gm->playersScore[playerId]);

    target->occupantId = playerId;

    start->isFloating = false;
    start->occupantId = -1;

    return true;
}

/**
 * @brief Validate a penguin movement from (startX,startY) to (endX,endY).
 *
 * Rules enforced:
 *  - Both coordinates must be inside the board.
 *  - Starting tile must have a penguin.
 *  - Target must be floating and empty.
 *  - Movement must be straight (horizontal or vertical).
 *  - All tiles along the path must be floating and unoccupied.
 *
 * @param gb Pointer to GameBoard.
 * @param startX Starting X coordinate.
 * @param startY Starting Y coordinate.
 * @param endX Ending X coordinate.
 * @param endY Ending Y coordinate.
 * @return true if the move is valid under all game rules.
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
