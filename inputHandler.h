#ifndef PENGUINS_THE_GAME_INPUT_HANDLER_H
#define PENGUINS_THE_GAME_INPUT_HANDLER_H

/**
 * @file inputHandler.h
 * @brief Header file for user input handling functions.
 *
 * This file declares functions for obtaining and validating user input,
 * specifically for integer values and coordinate pairs within specified ranges.
 */

/**
 * @enum InputStatus
 * @brief Status codes for input handling functions.
 */
typedef enum {
    INPUT_VALID = 0,
    INPUT_EXIT,
    INPUT_ERROR_INIT_FAILED
} InputStatus;

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
InputStatus GetIntegerInRange(const char* prompt, int min, int max, int* out_value);

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
    int minX, int maxX,
    int minY, int maxY,
    int* out_valueX, int* out_valueY);

#endif