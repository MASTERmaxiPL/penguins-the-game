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

if(NOT DEFINED CMAKE_POLICY_VERSION_MINIMUM)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum CMake policy version for fetched projects" FORCE)
endif()

include(FetchContent)

# Include external libraries( as static libs)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libs" FORCE)

FetchContent_Declare(
        cJSON
        GIT_REPOSITORY  https://github.com/DaveGamble/cJSON.git
        GIT_TAG         master
)

FetchContent_MakeAvailable(cjson)

if(TARGET cjson)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(cjson PRIVATE -Wno-error=float-conversion -Wno-float-conversion)
    endif()
endif()

# /**
#  * @brief Define the main executable and its source files.
#  *
#  * The add_executable command lists all source and header files that are part
#  * of the build for the `penguins_the_game` target. Headers are included
#  * here for convenience and to aid IDEs that read CMake targets.
#  *
#  * @note If you add or remove source files, update this list accordingly.
#  */
add_executable(penguins_the_game src/main.c
        src/core/gameManager.c
        src/core/gameManager.h
        src/core/boardGenerator.c
        src/core/boardGenerator.h
        src/phases/placementPhase.c
        src/phases/placementPhase.h
        src/phases/movementPhase.c
        src/phases/movementPhase.h
        src/core/iceFloe.h
        src/core/gameBoard.h
        src/utils/noise.c
        src/input/inputHandler.c
        src/input/inputHandler.h
        src/io/fileHandler.c
        src/io/fileHandler.h
        src/io/cjsonHandler.c
        src/io/cjsonHandler.h
)

target_link_libraries(penguins_the_game PRIVATE cjson)