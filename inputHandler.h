#ifndef PENGUINS_THE_GAME_INPUTREADER_H
#define PENGUINS_THE_GAME_INPUTREADER_H

typedef enum {
    INPUT_VALID = 0,
    INPUT_SAVE,
    INPUT_EXIT,
    INPUT_SAVE_AND_EXIT
} InputStatus;

InputStatus GetIntegerInRange(const char* prompt, int min, int max, int* out_value);

InputStatus GetCoordinatesInRange(
    const char* prompt,
    int minX, int maxX,
    int minY, int maxY,
    int* out_valueX, int* out_valueY);

#endif