#ifndef SRC_UTILS_TERMINAL_H
#define SRC_UTILS_TERMINAL_H

/** file terminal.h
 * @brief Terminal utilities for enabling ANSI escape codes on Windows.
 *
 * This header declares functions and macros for handling terminal
 * capabilities, specifically enabling Virtual Terminal (VT) mode on
 * Windows consoles to support ANSI escape codes for colored text output.
 */

/**
 * @brief Enable VT mode on Windows consoles to support ANSI escape codes.
 *
 * This function checks if the program is running on a Windows platform.
 * If so, it attempts to enable Virtual Terminal Processing mode for the
 * standard output handle, allowing the use of ANSI escape codes for
 * colored text and other terminal features.
 *
 * On non-Windows platforms, this function is a no-op.
 */
void Enable_VT_Mode(void);

/* ANSI color macros */
#define COLOR_RESET        "\x1b[0m"
#define COLOR_WATER        "\x1b[34m"
#define COLOR_ICE          "\x1b[37m"

/* Fish colors (kept distinct) */
#define COLOR_FISH_1       "\x1b[33m"
#define COLOR_FISH_2       "\x1b[93m"
#define COLOR_FISH_3       "\x1b[38;5;13m"

/* Label colors for odd/even column/row indices */
#define COLOR_LABEL_ODD    "\x1b[96m"  /* bright cyan */
#define COLOR_LABEL_EVEN   "\x1b[94m"  /* bright blue */

/* Player colors (distinct and readable) */
#define COLOR_PLAYER_1         "\x1b[91m"    /* bright red */
#define COLOR_PLAYER_2         "\x1b[38;5;154m"    /* green */
#define COLOR_PLAYER_3         "\x1b[38;5;104m"  /* bright blue */
#define COLOR_PLAYER_4         "\x1b[38;5;8m"   /* gray */

/* Helper to map zero-based player id to color */
static inline const char *PlayerColor(const int playerId) {
    switch (playerId) {
    case 0: return COLOR_PLAYER_1;
    case 1: return COLOR_PLAYER_2;
    case 2: return COLOR_PLAYER_3;
    case 3: return COLOR_PLAYER_4;
    default: return COLOR_ICE;
    }
}

#endif