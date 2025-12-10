#include <stdio.h>
#include "gameManager.h"

int main() {
    GameManager game;            // Main game manager instance
    bool session_active = true;  // Controls whether the program should loop again

    do {
        char input;

        printf("Welcome to penguin game!\n");
        printf("Initiating...\n");

        GameManager_Init(&game);    // Set up the game (board, players, settings)

        printf("Trying to run...\n");
        GameManager_Run(&game);     // Run the main game flow (placement + movement)

        printf("Game finished, cleaning...\n");
        GameManager_Cleanup(&game); // Free allocated memory and reset state

        // Ask the player if they want to start another game session
        printf("Do you want to play again? (print 'y' to continue, else program finishes)\n");

        scanf(" %c", &input);       // Leading space avoids reading leftover newline

        if (input != 'y' && input != 'Y')
            session_active = false; // End program if user does not choose 'y' or 'Y'

    } while (session_active);       // Repeat if user chooses to continue

    printf("Game Closed!");         // Final message before program ends
    return 0;                       // Exit successfully
}
