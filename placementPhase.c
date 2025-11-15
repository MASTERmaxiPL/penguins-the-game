#include "placementPhase.h"
#include "boardGenerator.h"
#include "player.h"

#include <stdio.h>


bool Can_Player_Place(const Player *p) {
    return p->remainingPenguins > 0; //check if any penguins left
}

void Player_Place(Player *p, IceFloe *floe, const int x, const int y) {
    floe->occupantId = p->symbol; //place penguin
    p->remainingPenguins--; // one less penguin
    p->sumOfFish += floe->fishCount; // +Fish
    printf("%c: Current Player placed his penguin on (%d,%d) and obtained %d fish. Now has %d fish. Penguins left: %d\n", p->symbol, x, y, floe->fishCount,p->sumOfFish, p->remainingPenguins);
}

void PlacementPhase_Run(GameManager *gm) {
    printf("Placement Phase\n");
    while (Can_Player_Place(&gm->players[0]) || Can_Player_Place(&gm->players[1])) {
        Player *currentPlayer = &gm->players[gm->turn];
        if (Can_Player_Place(currentPlayer)) {
            int x;
            int y;
            GameBoard_Print(&gm->gb);
            while (1)
                {
                printf("Player %c, put your x y coordinates: \n", currentPlayer->symbol);
                const int inputCount = scanf("%d %d", &x, &y);
                if (inputCount != 2) {
                    printf("Invalid input! Please enter two integers.\n");
                    while (getchar() != '\n');
                    continue;
                }
                if (x >= gm->gb.boardHeight || y >= gm->gb.boardWidth || x < 0 || y < 0) {
                    printf("your coordinates are out of bounds!\n ");
                    continue;
                }

                IceFloe *floe = &gm->gb.floeGrid[y][x];
                if (!floe->isFloating) { //check if there is ice floe
                    printf("You cannot place your penguin here due to lack of ice floe. (%d,%d)!\n", x, y);
                    continue;
                }
                if (floe->occupantId != -1) { //check if occupied
                    printf("You cannot place your penguin here due to occupied ice floe. (%d,%d)!\n", x, y);

                }
                else
                {
                    Player_Place(currentPlayer, floe, x, y);
                    break;
                }
                }
        }
        gm->turn = 1 - gm->turn;
    }

    printf("Placement Phase finished successfully!\n");
    GameBoard_Print(gm);
    printf("Player %c has %d fish after placement phase.\n", gm->players[0].symbol, gm->players[0].sumOfFish);
    printf("Player %c has %d fish after placement phase.\n", gm->players[1].symbol, gm->players[1].sumOfFish);
}









