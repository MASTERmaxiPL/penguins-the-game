#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inputHandler.h"

static void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

InputStatus GetIntegerInRange(const char* prompt, const int min, const int max, int* out_value) {
    char buffer[100];

    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return INPUT_EXIT;
        }

        if (strchr(buffer, '\n') == NULL) {
            ClearInputBuffer();
        }

        if (strcmp(buffer, "s\n") == 0) return INPUT_SAVE;
        if (strcmp(buffer, "x\n") == 0) return INPUT_EXIT;
        if (strcmp(buffer, "sx\n") == 0 || strcmp(buffer, "xs\n") == 0)
            return INPUT_SAVE_AND_EXIT;

        char* endptr;
        const long input = strtol(buffer, &endptr, 10);

        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input! Please enter an integer.\n");
            continue;
        }

        if (input <= min || input >= max) {
            printf("Input out of range! Please enter a value between %d and %d.\n", min, max);
            continue;
        }

        *out_value = (int)input;
        return INPUT_VALID;
    }
}

InputStatus GetCoordinatesInRange(
    const char* prompt,
    const int minX, const int maxX,
    const int minY, const int maxY,
    int* out_valueX, int* out_valueY
    ) {
    char buffer[100];

    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return INPUT_EXIT;
        }

        if (strchr(buffer, '\n') == NULL) {
            ClearInputBuffer();
        }

        if (strcmp(buffer, "s\n") == 0) return INPUT_SAVE;
        if (strcmp(buffer, "x\n") == 0) return INPUT_EXIT;
        if (strcmp(buffer, "sx\n") == 0 || strcmp(buffer, "xs\n") == 0)
            return INPUT_SAVE_AND_EXIT;

        int inputX, inputY;

        if (sscanf(buffer, "%d %d", &inputX, &inputY) != 2) {
            printf("Invalid format! Please enter 'x y' (e.g., 2 3).\n");
            continue;
        }

        if (inputX <= minX || inputX >= maxX || inputY <= minY || inputY >= maxY) {
            printf("Coordinates out of bounds! Range: %d-%d, %d-%d\n", minX, maxX, minY, maxY);
            continue;
        }

        *out_valueX = inputX;
        *out_valueY = inputY;
        return INPUT_VALID;
    }
}
