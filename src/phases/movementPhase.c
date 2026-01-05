/**
 * @file movementPhase.c
 * @brief Implements logic for checking available moves, validating movement,
 *        executing penguin movement, and running the full movement phase loop.
 */

#include "movementPhase.h"
#include "../core/boardGenerator.h"
#include "../common/messages.h"

#include <stdio.h>

#include "../input/inputHandler.h"

/**
 * @brief Run the movement phase until all players are blocked.
 *
 * The phase proceeds in player order. Each player attempts a move; if a player
 * has no valid moves, they are counted as blocked. When all players are
 * consecutively blocked, the movement phase ends.
 *
 * @param gm Pointer to the GameManager (read-only).
 * @param isLoadedGame Indicates if the game was loaded from a saved state.
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus MovementPhase_Run(GameManager *gm, const bool isLoadedGame) {
    int *playerIndex = &gm->currentPlayerIndex;
    int *round = &gm->currentRound;

    if (!isLoadedGame) {
        *round = 1;
        *playerIndex = 0;
    }

    int blocked_counter = 0;

    printf(MSG_MOVEMENT_PHASE);
    printf(MSG_ROUND, *round);

    do {
        if (*playerIndex >= gm->numOfPlayers) {
            gm->currentPlayerIndex = 0;
            (*round)++;
            printf(MSG_ROUND, *round);
        }

        printf(MSG_PLAYER_TURN, *playerIndex + 1);

        if (!Check_Player_Has_Any_Moves(&gm->gb, *playerIndex)) {
            blocked_counter++;
            printf(MSG_PLAYER_NO_AVAILABLE_MOVES, *playerIndex + 1);
        } else {
            blocked_counter = 0;
            const InputStatus status = Player_Movement_Turn(gm);
            if (status == INPUT_EXIT) return status;
        }

        if (blocked_counter == gm->numOfPlayers) {
            printf(MSG_ALL_PLAYERS_NO_AVAILABLE_MOVES);
            break;
        }
        (*playerIndex)++;
    } while (1);
    return INPUT_VALID;
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
 * @return InputStatus indicating success or failure of initialization.
 */
InputStatus Player_Movement_Turn(GameManager *gm) {
    InputStatus status;

    const GameBoard *gb = &gm->gb;
    int startX, startY, endX, endY;

    while(1){
        Print_Board(gb);
        status = GetCoordinatesInRange(
            MSG_CHOOSE_PENGUIN,
            0, gb->boardWidth - 1,
            0, gb->boardHeight - 1,
            &startX, &startY);
        if (status == INPUT_SAVE || status == INPUT_SAVE_AND_EXIT)
            GameManager_SaveToFile(gm, "..\\saves\\data.json");
        if (status == INPUT_SAVE) continue;
        if (status == INPUT_SAVE_AND_EXIT || status == INPUT_EXIT) return INPUT_EXIT;

        IceFloe *start = &gb->floeGrid[startY][startX];

        if (start->occupantId == -1 ||start->occupantId != gm->currentPlayerIndex ||!Check_Penguin_Has_Any_Moves(gb, startX, startY,gb->boardHeight, gb->boardWidth)) {
            printf(MSG_INVALID_MOVE);
            continue;
        }

        status = GetCoordinatesInRange(
            MSG_CHOOSE_DESTINATION,
            0, gb->boardWidth - 1,
            0, gb->boardHeight - 1,
            &endX, &endY);
        if (status == INPUT_SAVE || status == INPUT_SAVE_AND_EXIT)
            GameManager_SaveToFile(gm, "..\\saves\\data.json");
        if (status == INPUT_SAVE) continue;
        if (status == INPUT_SAVE_AND_EXIT || status == INPUT_EXIT) return INPUT_EXIT;

        if (Move_Penguin(gm, startX, startY, endX, endY)) {
            printf(MSG_MOVE_SUCCESSFUL);
            break;
        }
        printf(MSG_INVALID_MOVE);
    }
    return status;
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

    if (playerId != gm->currentPlayerIndex) return false;
    
    if (!Is_Valid_Move(gb, startX, startY, endX, endY))
        return false;

    IceFloe *start  = &gb->floeGrid[startY][startX];
    IceFloe *target = &gb->floeGrid[endY][endX];

    const int collectedFish = target->fishCount;
    gm->playersScore[playerId] += collectedFish;

    printf(MSG_AFTER_POSITION_UPDATE, endX, endY, collectedFish, gm->playersScore[playerId]);

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
