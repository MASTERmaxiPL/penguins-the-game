// Unit tests for inputHandler.c 

#include "unity.h"
#include "inputHandler.h"

#include <stdio.h>
#include <string.h>

// Helper to simulate stdin
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


