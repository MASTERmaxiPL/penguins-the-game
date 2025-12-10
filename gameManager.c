#include "gameManager.h"
#include "boardGenerator.h"
#include "placementPhase.h"
#include "movementPhase.h"
#include "math.h"

#include <stdio.h>
#include <stdlib.h>

#define minWidth 10      // Minimum board width allowed
#define maxWidth 30      // Maximum board width allowed
#define minHeight 10     // Minimum board height allowed
#define maxHeight 30     // Maximum board height allowed

// Initializes the game state, reads all user-defined settings,
// allocates memory, and prepares the board.
void GameManager_Init(GameManager *gm) {
    int boardWidth;
    int boardHeight;

    // Ask for number of players until valid input is received
    while (1) {
        printf("Enter number of players (n>1): ");
        const int inputCountNOP = scanf("%d", &gm->numOfPlayers);

        // Validate input
        if (inputCountNOP != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n'){}   // Clear input buffer
            continue;
        }

        // There must be at least 2 players
        if (gm->numOfPlayers < 2) {
            printf("The number of players must be greater than 1.\n");
        } else {
            break;
        }
    }

    // Ask for board dimensions until valid input is received
    while (1) {
        printf("Enter board width (10<x<30): ");
        const int inputCountBW = scanf("%d", &boardWidth);

        if (inputCountBW != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n'){}
            continue;
        }

        // Check width boundaries
        if (boardWidth < minWidth || boardWidth > maxWidth) {
            printf("The conditions are not met!\n");
            continue;
        }

        printf("Enter board height (10<y<30): ");
        const int inputCountBH = scanf("%d", &boardHeight);

        if (inputCountBH != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n'){}
            continue;
        }

        // Check height boundaries
        if (boardHeight < minHeight || boardHeight > maxHeight) {
            printf("The conditions are not met!\n");
        } else {
            break;
        }
    }

    // Initialize game board with chosen size
    GameBoard_Init(&gm->gb, boardWidth, boardHeight);

    // Allocate memory for players' scores (all zeroed by calloc)
    gm->playersScore = calloc(gm->numOfPlayers, sizeof(int));

    // Game is now marked as running
    gm->isRunning = true;

    // Determine the maximum number of penguins each player may place
    const int maxPenguins = floor(gm->gb.placeableFloeCount / gm->numOfPlayers);

    // Ask user to choose how many penguins each player gets
    while (1) {
        printf("Enter number of penguins per player (0<n<=%d): ", maxPenguins);
        const int inputCountPPP = scanf("%d", &gm->penguinsPerPlayer);

        if (inputCountPPP != 1) {
            printf("Invalid input! Please enter integer.\n");
            while (getchar() != '\n'){}
            continue;
        }

        // Penguins per player must obey rule
        if (gm->penguinsPerPlayer < 1 || gm->penguinsPerPlayer > maxPenguins) {
            printf("The number of penguins must be between 1 and %d.\n", maxPenguins);
        } else {
            break;
        }
    }

    printf("Game initialized!\n");
}

// Starts and controls the flow of the game.
// Currently runs placement phase followed by movement phase once.
void GameManager_Run(GameManager *gm) {
    printf("Game running...\n");

    // Main loop (currently ends after one cycle)
    while (gm->isRunning) {
        PlacementPhase_Run(gm);   // Execute penguin placement
        MovementPhase_Run(gm);    // Execute penguin movement
        gm->isRunning = false;    // End after one iteration (temporary logic)
    }
}

// Cleans up board memory and prints confirmation
void GameManager_Cleanup(GameManager *gm) {
    GameBoard_Cleanup(&gm->gb);  // Free dynamically allocated board grid
    printf("Game cleaned up!\n");
}
