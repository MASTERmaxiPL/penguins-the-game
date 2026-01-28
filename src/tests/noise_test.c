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
