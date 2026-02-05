#include "unity.h"
#include "movementPhase.h"
#include "gameBoard.h"
#include "iceFloe.h"
#include "gameManager.h"

#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

static GameBoard create_simple_board(void) {
    GameBoard gb = {0};
    gb.boardWidth = 3;
    gb.boardHeight = 3;
    gb.floeGrid = calloc(3, sizeof(IceFloe*));
    for (int i = 0; i < 3; i++) {
        gb.floeGrid[i] = calloc(3, sizeof(IceFloe));
        for (int j = 0; j < 3; j++) {
            gb.floeGrid[i][j].isFloating = true;
            gb.floeGrid[i][j].fishCount = 1;
            gb.floeGrid[i][j].occupantId = -1;
        }
    }
    return gb;
}

static void free_board(GameBoard *gb) {
    for (int i = 0; i < gb->boardHeight; i++) {
        free(gb->floeGrid[i]);
    }
    free(gb->floeGrid);
}

void test_Is_Move_In_Bounds(void) {
    GameBoard gb = create_simple_board();
    TEST_ASSERT_TRUE(Is_Move_In_Bounds(&gb, 0, 0));
    TEST_ASSERT_TRUE(Is_Move_In_Bounds(&gb, 2, 2));
    TEST_ASSERT_FALSE(Is_Move_In_Bounds(&gb, -1, 0));
    TEST_ASSERT_FALSE(Is_Move_In_Bounds(&gb, 3, 1));
    free_board(&gb);
}

void test_Check_Penguin_Has_Any_Moves_true(void) {
    GameBoard gb = create_simple_board();
    gb.floeGrid[1][1].occupantId = 0;
    TEST_ASSERT_TRUE(Check_Penguin_Has_Any_Moves(&gb, 1, 1, 3, 3));
    free_board(&gb);
}

void test_Check_Penguin_Has_Any_Moves_false_when_blocked(void) {
    GameBoard gb = create_simple_board();
    gb.floeGrid[1][1].occupantId = 0;

    gb.floeGrid[1][0].isFloating = false;
    gb.floeGrid[1][2].isFloating = false;
    gb.floeGrid[0][1].isFloating = false;
    gb.floeGrid[2][1].isFloating = false;

    TEST_ASSERT_FALSE(Check_Penguin_Has_Any_Moves(&gb, 1, 1, 3, 3));
    free_board(&gb);
}

void test_Is_Valid_Move_horizontal(void) {
    GameBoard gb = create_simple_board();
    gb.floeGrid[1][0].occupantId = 0;
    TEST_ASSERT_TRUE(Is_Valid_Move(&gb, 0, 1, 2, 1));
    free_board(&gb);
}

void test_Is_Valid_Move_diagonal_invalid(void) {
    GameBoard gb = create_simple_board();
    gb.floeGrid[0][0].occupantId = 0;
    TEST_ASSERT_FALSE(Is_Valid_Move(&gb, 0, 0, 1, 1));
    free_board(&gb);
}

void test_Move_Penguin_success(void) {
    GameManager gm = {0};
    gm.gb = create_simple_board();
    gm.currentPlayerIndex = 0;
    gm.playersScore = calloc(1, sizeof(int));

    gm.gb.floeGrid[1][0].occupantId = 0;
    gm.gb.floeGrid[1][2].fishCount = 2;

    TEST_ASSERT_TRUE(Move_Penguin(&gm, 0, 1, 2, 1));
    TEST_ASSERT_EQUAL(2, gm.playersScore[0]);
    TEST_ASSERT_EQUAL(-1, gm.gb.floeGrid[1][0].occupantId);
    TEST_ASSERT_FALSE(gm.gb.floeGrid[1][0].isFloating);
    TEST_ASSERT_EQUAL(0, gm.gb.floeGrid[1][2].occupantId);

    free(gm.playersScore);
    free_board(&gm.gb);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Is_Move_In_Bounds);
    RUN_TEST(test_Check_Penguin_Has_Any_Moves_true);
    RUN_TEST(test_Check_Penguin_Has_Any_Moves_false_when_blocked);
    RUN_TEST(test_Is_Valid_Move_horizontal);
    RUN_TEST(test_Is_Valid_Move_diagonal_invalid);
    RUN_TEST(test_Move_Penguin_success);
    return UNITY_END();
}
