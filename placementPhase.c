#include "placementPhase.h"
#include "boardGenerator.h"
#include "player.h"

#include <stdio.h>


bool Can_Player_Place(Player *p) {
    return p->remainingPenguins > 0; //check if any penguins left
}

void Player_Place(GameBoard *gb, Player *p, int x, int y) {
    IceFloe *floe = &gb->floeGrid[x][y];
    if (x > gb->boardHeight || y > gb->boardWidth || x < 0 || y < 0) {
        printf("your coordinates are out of bounds!\n ");
        return;
    }
    if (!floe->isFloating) { //check if there is ice floe
        printf("You cannot put your penguin here due to lack of ice floe. (%d,%d)!\n", y, x);
        return;
    }
    if (floe->occupantId != -1) { //check if occupied
        printf("You cannot put your penguin here due to occupied ice floe. (%d,%d)!\n", y, x);
        return;
    }
    floe->occupantId = p->symbol; //place penguin
    p->remainingPenguins--; // one less penguin
    p->FishSum += floe->fishCount; // +Fish
    printf("%c: Current Player put his penguin on (%d,%d) and obtained %d fish. Now has %d fish. Penguins left: %d\n", p->symbol, y, x, floe->fishCount,p->FishSum, p->remainingPenguins);



}

void PlacementPhase_Run(GameManager *gm) {
    printf("Placement Phase\n");
    while (Can_Player_Place(&gm->players[0]) || Can_Player_Place(&gm->players[1])) {
        Player *currentPlayer = &gm->players[gm->turn];

        if (Can_Player_Place(currentPlayer)) {
            int x;
            int y;
            GameBoard_Print(gm);
            printf("Player %c, put your x y coordinates: \n", currentPlayer->symbol);
            scanf("%d %d", &x, &y);
            //printf("Player %c, put your y coordinate: \n", currentPlayer->symbol);
            //scanf("%d", &y);
            Player_Place(&gm->gb, currentPlayer, y, x);
        }
        gm->turn = 1 - gm->turn;
    }
    printf("Placement Phase finished succesfully!\n");
    GameBoard_Print(gm);
    printf("Player %c has %d fish after placement phase.\n", gm->players[0].symbol, gm->players[0].FishSum);
    printf("Player %c has %d fish after placement phase.\n", gm->players[1].symbol, gm->players[1].FishSum);
}
