// Unit tests for inputHandler.c 

#include "unity.h"
#include "inputHandler.h"

#include <stdio.h>
#include <string.h>

static void set_stdin(const char *input) {
    static char buffer[256];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    fmemopen(buffer, strlen(buffer), "r");
}

void setUp(void) {}
void tearDown(void) {}

void test_GetIntegerInRange_valid_input(void) {
    int value = 0;
    set_stdin("5\n");
    InputStatus status = GetIntegerInRange(">", 1, 10, &value);
    TEST_ASSERT_EQUAL(INPUT_VALID, status);
    TEST_ASSERT_EQUAL(5, value);
}

void test_GetIntegerInRange_exit(void) {
    int value = 0;
    set_stdin("x\n");
    InputStatus status = GetIntegerInRange(">", 1, 10, &value);
    TEST_ASSERT_EQUAL(INPUT_EXIT, status);
}

void test_GetIntegerInRange_out_of_range_then_valid(void) {
    int value = 0;
    set_stdin("20\n3\n");
    InputStatus status = GetIntegerInRange(">", 1, 10, &value);
    TEST_ASSERT_EQUAL(INPUT_VALID, status);
    TEST_ASSERT_EQUAL(3, value);
}

void test_GetCoordinatesInRange_valid(void) {
    int x = 0, y = 0;
    set_stdin("2 3\n");
    InputStatus status = GetCoordinatesInRange(">", 0, 5, 0, 5, &x, &y);
    TEST_ASSERT_EQUAL(INPUT_VALID, status);
    TEST_ASSERT_EQUAL(2, x);
    TEST_ASSERT_EQUAL(3, y);
}

void test_GetCoordinatesInRange_exit(void) {
    int x = 0, y = 0;
    set_stdin("x\n");
    InputStatus status = GetCoordinatesInRange(">", 0, 5, 0, 5, &x, &y);
    TEST_ASSERT_EQUAL(INPUT_EXIT, status);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_GetIntegerInRange_valid_input);
    RUN_TEST(test_GetIntegerInRange_exit);
    RUN_TEST(test_GetIntegerInRange_out_of_range_then_valid);
    RUN_TEST(test_GetCoordinatesInRange_valid);
    RUN_TEST(test_GetCoordinatesInRange_exit);
    return UNITY_END();
}





// Unit tests for cjsonHandler.c

#include "unity.h"
#include "cjsonHandler.h"
#include "gameManager.h"

#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

static GameManager create_sample_gamemanager(void) {
    GameManager gm = {0};
    gm.numOfPlayers = 2;
    gm.playersScore = calloc(2, sizeof(int));
    gm.isBotPlayers = calloc(2, sizeof(bool));
    gm.playersScore[0] = 10;
    gm.playersScore[1] = 20;
    gm.penguinsPerPlayer = 3;
    gm.isRunning = false;
    gm.currentPhase = PHASE_MOVEMENT;
    gm.currentPlayerIndex = 1;
    gm.currentRound = 4;

    gm.gb.boardWidth = 2;
    gm.gb.boardHeight = 2;
    gm.gb.placeableFloeCount = 1;

    gm.gb.floeGrid = calloc(2, sizeof(IceFloe *));
    for (int i = 0; i < 2; i++) {
        gm.gb.floeGrid[i] = calloc(2, sizeof(IceFloe));
    }

    gm.gb.floeGrid[0][0] = (IceFloe){true, 1, -1};
    gm.gb.floeGrid[0][1] = (IceFloe){false, 0, -1};
    gm.gb.floeGrid[1][0] = (IceFloe){true, 2, 0};
    gm.gb.floeGrid[1][1] = (IceFloe){true, 3, 1};

    return gm;
}

static void free_gamemanager(GameManager *gm) {
    free(gm->playersScore);
    free(gm->isBotPlayers);
    for (int i = 0; i < gm->gb.boardHeight; i++) {
        free(gm->gb.floeGrid[i]);
    }
    free(gm->gb.floeGrid);
}

void test_CJSON_roundtrip_serialization(void) {
    GameManager gm = create_sample_gamemanager();

    cJSON *json = CJSON_CreateFromGameManager(&gm);
    TEST_ASSERT_NOT_NULL(json);

    GameManager loaded = {0};
    bool ok = CJSON_LoadGameManagerFromJson(&loaded, json);
    TEST_ASSERT_TRUE(ok);

    TEST_ASSERT_EQUAL(gm.numOfPlayers, loaded.numOfPlayers);
    TEST_ASSERT_EQUAL(gm.playersScore[0], loaded.playersScore[0]);
    TEST_ASSERT_EQUAL(gm.playersScore[1], loaded.playersScore[1]);
    TEST_ASSERT_EQUAL(gm.penguinsPerPlayer, loaded.penguinsPerPlayer);
    TEST_ASSERT_EQUAL(gm.currentPhase, loaded.currentPhase);
    TEST_ASSERT_EQUAL(gm.currentRound, loaded.currentRound);

    TEST_ASSERT_EQUAL(gm.gb.boardWidth, loaded.gb.boardWidth);
    TEST_ASSERT_EQUAL(gm.gb.boardHeight, loaded.gb.boardHeight);
    TEST_ASSERT_EQUAL(gm.gb.placeableFloeCount, loaded.gb.placeableFloeCount);

    TEST_ASSERT_EQUAL(gm.gb.floeGrid[1][0].fishCount, loaded.gb.floeGrid[1][0].fishCount);
    TEST_ASSERT_EQUAL(gm.gb.floeGrid[1][1].occupantId, loaded.gb.floeGrid[1][1].occupantId);

    free_gamemanager(&gm);
    free_gamemanager(&loaded);
    cJSON_Delete(json);
}

void test_CJSON_load_null_inputs(void) {
    TEST_ASSERT_FALSE(CJSON_LoadGameManagerFromJson(NULL, NULL));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_CJSON_roundtrip_serialization);
    RUN_TEST(test_CJSON_load_null_inputs);
    return UNITY_END();
}





// Unit tests for movementPhase.c

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





// Unit tests for placementPhase.c 

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
    gm.isBotPlayers = calloc(2, sizeof(bool));
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
    free(gm->isBotPlayers);
    for (int i = 0; i < gm->gb.boardHeight; i++) {
        free(gm->gb.floeGrid[i]);
    }
    free(gm->gb.floeGrid);
}

void test_Player_Place_success(void) {
    GameManager gm = create_gm_for_placement();

    IceFloe *floe = &gm.gb.floeGrid[0][0];

    Player_Place(0, gm.playersScore, floe, 0, 0, &gm->isBotPlayers);

    TEST_ASSERT_EQUAL(0, floe->occupantId);
    TEST_ASSERT_EQUAL(1, gm.playersScore[0]);

    free_gm(&gm);
}

void test_Player_Place_does_not_modify_other_tiles(void) {
    GameManager gm = create_gm_for_placement();

    Player_Place(0, gm.playersScore, &gm.gb.floeGrid[1][1], 1, 1, &gm->isBotPlayers);

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





// Unit tests for noise.c

#include "unity.h"
#include "noise.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

void test_Fbm_deterministic_output_same_seed(void) {
    double v1 = Fbm(1.25, 3.75, 4, 2.0, 0.5, 1234);
    double v2 = Fbm(1.25, 3.75, 4, 2.0, 0.5, 1234);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, v1, v2);
}

void test_Fbm_different_seed_changes_output(void) {
    double v1 = Fbm(2.0, 2.0, 5, 2.0, 0.5, 1);
    double v2 = Fbm(2.0, 2.0, 5, 2.0, 0.5, 2);
    TEST_ASSERT_NOT_EQUAL(v1, v2);
}

void test_Fbm_output_range(void) {
    double v = Fbm(10.5, -7.25, 6, 2.0, 0.5, 42);
    TEST_ASSERT_TRUE(v >= 0.0);
    TEST_ASSERT_TRUE(v <= 1.0);
}

void test_Fbm_zero_octaves(void) {
    double v = Fbm(1.0, 1.0, 0, 2.0, 0.5, 99);
    TEST_ASSERT_TRUE(isnan(v) || isinf(v));
}

void test_Fbm_continuity_small_delta(void) {
    double v1 = Fbm(1.0, 1.0, 4, 2.0, 0.5, 777);
    double v2 = Fbm(1.001, 1.001, 4, 2.0, 0.5, 777);
    TEST_ASSERT_DOUBLE_WITHIN(0.05, v1, v2);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Fbm_deterministic_output_same_seed);
    RUN_TEST(test_Fbm_different_seed_changes_output);
    RUN_TEST(test_Fbm_output_range);
    RUN_TEST(test_Fbm_zero_octaves);
    RUN_TEST(test_Fbm_continuity_small_delta);
    return UNITY_END();
}
