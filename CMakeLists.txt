# /**
#  * @file CMakeLists.txt
#  * @brief CMake build configuration for the Penguins project.
#  *
#  * This file declares the project and lists the source files that compose the
#  * `penguins_the_game` executable. It sets the project language to C and
#  * specifies the C standard to use when compiling.
#  *
#  * @note This file was documented in-place with Doxygen-style comment blocks.
#  *       The remainder of the file is left unchanged.
#  */

cmake_minimum_required(VERSION 4.0)
project(penguins_the_game C)

# /**
#  * @brief Select the C language standard to compile with.
#  *
#  * Setting CMAKE_C_STANDARD to 23 requests C23.
#  */
set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# /**
#  * @brief Define the main executable and its source files.
#  *
#  * The add_executable command lists all source and header files that are part
#  * of the build for the `penguins_the_game` target. Headers are included
#  * here for convenience and to aid IDEs that read CMake targets.
#  *
#  * @note If you add or remove source files, update this list accordingly.
#  */
add_executable(penguins_the_game main.c
        gameManager.c
        gameManager.h
        boardGenerator.c
        boardGenerator.h
        placementPhase.c
        placementPhase.h
        movementPhase.c
        movementPhase.h
        iceFloe.h
        gameBoard.h
        noise.c
        inputHandler.c
        inputHandler.h
)
