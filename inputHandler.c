#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inputHandler.h"
#include "messages.h"

static void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

InputStatus GetIntegerInRange(const char* prompt, const int min, const int max, int* out_value) {
    char buffer[100];

    while (1) {
        printf(prompt, min, max);

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
            printf(MSG_INVALID_INPUT_TYPE);
            continue;
        }

        if (input < min || input > max) {
            printf(MSG_INPUT_OUT_OF_RANGE, min, max);
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
            printf(MSG_INVALID_FORMAT);
            continue;
        }

        if (inputX < minX || inputX > maxX || inputY < minY || inputY > maxY) {
            printf(MSG_COORDINATES_OUT_OF_BOUNDS, minX, maxX, minY, maxY);
            continue;
        }

        *out_valueX = inputX;
        *out_valueY = inputY;
        return INPUT_VALID;
    }
}
