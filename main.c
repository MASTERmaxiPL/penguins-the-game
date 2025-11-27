#include <stdio.h>
#include "gameManager.h"

int main() {
    GameManager game;
    bool session_active = true;

    do {
        char input;
        printf("Welcome to penguin game!\n");
        printf("Initiating...\n");
        GameManager_Init(&game);
        printf("Trying to run...\n");
        GameManager_Run(&game);
        printf("Game finished, cleaning...\n");
        GameManager_Cleanup(&game);

        printf("Do you want to play again? (print 'y' to continue, else program finishes)\n");
        scanf(" %c", &input);
        if (input != 'y' && input != 'Y')
            session_active = false;
    }
    while (session_active);

    printf("Game Closed!");
    return 0;
}