#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inputHandler.h"
#include "messages.h"

#define INPUT_BUFFER_SIZE 20

/**
 * @brief Clears the input buffer to avoid overflow issues.
 */
static void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * @brief Prompts the user for an integer input within a specified range.
 *
 * This function repeatedly prompts the user until a valid integer within the
 * specified range is entered or until the user opts to exit by entering 'x'.
 *
 * @param prompt The message displayed to the user.
 * @param min The minimum acceptable integer value (inclusive).
 * @param max The maximum acceptable integer value (inclusive).
 * @param out_value Pointer to store the valid integer input.
 * @return INPUT_VALID if a valid input is received, INPUT_EXIT if the user opts to exit.
 */
InputStatus GetIntegerInRange(const char* prompt, const int min, const int max, int* out_value) {
    char buffer[INPUT_BUFFER_SIZE];

    while (1) {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return INPUT_EXIT;
        }

        if (strchr(buffer, '\n') == NULL) {
            ClearInputBuffer();
        }

        if (strcmp(buffer, "x\n") == 0) return INPUT_EXIT;

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

/**
 * @brief Prompts the user for coordinate input within specified ranges.
 *
 * This function repeatedly prompts the user until valid coordinates within the
 * specified ranges are entered or until the user opts to exit by entering 'x'.
 *
 * @param prompt The message displayed to the user.
 * @param minX The minimum acceptable X coordinate (inclusive).
 * @param maxX The maximum acceptable X coordinate (inclusive).
 * @param minY The minimum acceptable Y coordinate (inclusive).
 * @param maxY The maximum acceptable Y coordinate (inclusive).
 * @param out_valueX Pointer to store the valid X coordinate input.
 * @param out_valueY Pointer to store the valid Y coordinate input.
 * @return INPUT_VALID if valid coordinates are received, INPUT_EXIT if the user opts to exit.
 */
InputStatus GetCoordinatesInRange(
    const char* prompt,
    const int minX, const int maxX,
    const int minY, const int maxY,
    int* out_valueX, int* out_valueY
    ) {
    char buffer[INPUT_BUFFER_SIZE];

    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return INPUT_EXIT;
        }

        if (strchr(buffer, '\n') == NULL) {
            ClearInputBuffer();
        }

        if (strcmp(buffer, "sx\n") == 0 || strcmp(buffer, "xs\n") == 0)
        {
            //SAVE GAME
            return INPUT_EXIT;
        }
        if (strcmp(buffer, "s\n") == 0)
        {
            //SAVE GAME
            continue;
        }
        if (strcmp(buffer, "x\n") == 0) return INPUT_EXIT;

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
