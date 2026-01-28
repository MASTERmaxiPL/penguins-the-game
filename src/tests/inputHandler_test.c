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

