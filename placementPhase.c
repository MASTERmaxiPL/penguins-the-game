#include "placementPhase.h"
#include "boardGenerator.h"

#include <stdio.h>

// Runs the entire placement phase where each player places all penguins.
void PlacementPhase_Run(GameManager *gm) {
    printf("Placement Phase\n");

    // Loop through placement turns (each player places one penguin per turn)
    for (int turn = 1; turn < gm->penguinsPerPlayer + 1; turn++) {
        printf("Turn %d\n", turn);

        // Players take turns placing their penguins
        for (int currentPlayerIndex = 0; currentPlayerIndex < gm->numOfPlayers; currentPlayerIndex++) {
            Print_Board(&gm->gb);                    // Display board before each placement
            Player_Placement_Turn(gm, currentPlayerIndex);  // Handle placement for current player
        }
    }

    printf("Placement Phase finished successfully!\n");
    Print_Board(&gm->gb);   // Show final board state after placement phase

    // Display how many fish each player collected during placement
    for (int i = 0; i < gm->numOfPlayers; i++) {
        printf("Player %d has %d fish after placement phase.\n",
               i + 1, gm->playersScore[i]);
    }
}

// Handles a single player's input and validates penguin placement.
void Player_Placement_Turn(GameManager *gm, const int currentPlayerIndex) {
    int x, y;

    while (1) {
        printf("Player %d, put your x y coordinates:\n", currentPlayerIndex + 1);

        const int inputCount = scanf("%d %d", &x, &y);

        // Check if two integers were provided
        if (inputCount != 2) {
            printf("Invalid input! Please enter two integers.\n");
            while (getchar() != '\n'){}  // Clear buffer
            continue;
        }

        // Check board bounds
        if (x >= gm->gb.boardWidth || y >= gm->gb.boardHeight || x < 0 || y < 0) {
            printf("Your coordinates are out of bounds!\n");
            continue;
        }

        IceFloe *floe = &gm->gb.floeGrid[y][x];

        // Valid tile must be floating, unoccupied, AND contain exactly 1 fish
        if (floe->isFloating && floe->occupantId == -1 && floe->fishCount == 1) {
            Player_Place(currentPlayerIndex, gm->playersScore, floe, x, y);
            break;
        }

        printf("You cannot place your penguin here. Try again! (%d,%d)!\n", x, y);
    }
}

// Places penguin on a selected tile and rewards the player with fish.
void Player_Place(const int playerIndex, int *players, IceFloe *floe, const int x, const int y) {
    floe->occupantId = playerIndex;              // Assign penguin to the tile
    players[playerIndex] += floe->fishCount;     // Add collected fish to player's score

    printf(
        "Current Player placed his penguin on (%d,%d) and obtained %d fish. "
        "Now has %d fish.\n",
        x, y, floe->fishCount, players[playerIndex]
    );
}
