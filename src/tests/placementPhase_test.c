#include "unity.h"
#include "placementPhase.h"
#include "gameManager.h"
#include "gameBoard.h"
#include "iceFloe.h"

#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

static GameManager create_gm_for_placement(void) {
    GameManager gm = {0};
    gm.numOfPlayers = 2;
    gm.penguinsPerPlayer = 1;
    gm.currentPlayerIndex = 0;
    gm.playersScore = calloc(2, sizeof(int));

    gm.gb.boardWidth = 2;
    gm.gb.boardHeight = 2;
    gm.gb.floeGrid = calloc(2, sizeof(IceFloe*));
    for (int i = 0; i < 2; i++) {
        gm.gb.floeGrid[i] = calloc(2, sizeof(IceFloe));
    }

    gm.gb.floeGrid[0][0] = (IceFloe){true, 1, -1};
    gm.gb.floeGrid[0][1] = (IceFloe){true, 2, -1};
    gm.gb.floeGrid[1][0] = (IceFloe){false, 0, -1};
    gm.gb.floeGrid[1][1] = (IceFloe){true, 1, -1};

    return gm;
}

static void free_gm(GameManager *gm) {
    free(gm->playersScore);
    for (int i = 0; i < gm->gb.boardHeight; i++) {
        free(gm->gb.floeGrid[i]);
    }
    free(gm->gb.floeGrid);
}

void test_Player_Place_success(void) {
    GameManager gm = create_gm_for_placement();

    IceFloe *floe = &gm.gb.floeGrid[0][0];
    Player_Place(0, gm.playersScore, floe, 0, 0);

    TEST_ASSERT_EQUAL(0, floe->occupantId);
    TEST_ASSERT_EQUAL(1, gm.playersScore[0]);

    free_gm(&gm);
}

void test_Player_Place_does_not_modify_other_tiles(void) {
    GameManager gm = create_gm_for_placement();

    Player_Place(0, gm.playersScore, &gm.gb.floeGrid[1][1], 1, 1);

    TEST_ASSERT_EQUAL(0, gm.gb.floeGrid[1][1].occupantId);
    TEST_ASSERT_EQUAL(-1, gm.gb.floeGrid[0][0].occupantId);

    free_gm(&gm);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Player_Place_success);
    RUN_TEST(test_Player_Place_does_not_modify_other_tiles);
    return UNITY_END();
}
