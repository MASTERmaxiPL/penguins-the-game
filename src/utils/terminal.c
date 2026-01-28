#include "terminal.h"

#ifdef _WIN32
#include <windows.h>
#endif

/**@file terminal.c
 * @brief Implements terminal utilities for enabling ANSI escape code support.
 *
 * This file contains functions to configure the terminal for enhanced
 * text output capabilities, such as colored text using ANSI escape codes.
 * On Windows platforms, it enables Virtual Terminal Processing mode.
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
void Enable_VT_Mode(void) {
#ifdef _WIN32
    static int enabled = 0;
    if (enabled) return;
    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
    enabled = 1;
#else
    /* no-op on non-Windows */
    (void)0;
#endif
}