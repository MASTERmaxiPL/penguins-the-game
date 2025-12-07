#include "placementPhase.h"
#include "boardGenerator.h"

#include <stdio.h>

void PlacementPhase_Run(GameManager *gm) {
    printf("Placement Phase\n");
    for ( int turn = 1; turn < gm->penguinsPerPlayer+1; turn++) {
        printf("Turn %d\n", turn);
        for ( int currentPlayerIndex = 0; currentPlayerIndex < gm->numOfPlayers; currentPlayerIndex++) {
            Print_Board(&gm->gb);
            Player_Placement_Turn(gm, currentPlayerIndex);
        }
    }

    printf("Placement Phase finished successfully!\n");
    Print_Board(&gm->gb);
    for ( int i = 0; i < gm->numOfPlayers; i++){
        printf("Player %d has %d fish after placement phase.\n", i+1, gm->playersScore[i]);
    }
}

void Player_Placement_Turn(GameManager *gm, const int currentPlayerIndex)
{
    int x, y;

    while (1)
    {
        printf("Player %d, put your x y coordinates: \n", currentPlayerIndex+1);
        const int inputCount = scanf("%d %d", &x, &y);
        if (inputCount != 2) {
            printf("Invalid input! Please enter two integers.\n");
            while (getchar() != '\n'){}
            continue;
        }
        if (x >= gm->gb.boardWidth || y >= gm->gb.boardHeight || x < 0 || y < 0) {
            printf("your coordinates are out of bounds!\n ");
            continue;
        }

        IceFloe *floe = &gm->gb.floeGrid[y][x];

        if (floe->isFloating && floe->occupantId == -1 && floe->fishCount == 1) {
            Player_Place(currentPlayerIndex, gm->playersScore, floe, x, y);
            break;
        }
        printf("You cannot place your penguin here. Try again! (%d,%d)!\n", x, y);
    }
}

void Player_Place(const int playerIndex, int *players, IceFloe *floe, const int x, const int y) {
    floe->occupantId = playerIndex;
    players[playerIndex] += floe->fishCount;
    printf("Current Player placed his penguin on (%d,%d) and obtained %d fish. Now has %d fish.\n", x, y, floe->fishCount, players[playerIndex]);
}
