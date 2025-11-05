cmake_minimum_required(VERSION 4.0)
project(penguins_the_game C)

set(CMAKE_C_STANDARD 23)

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
        gameBoard.h)
