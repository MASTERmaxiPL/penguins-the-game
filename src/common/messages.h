/**
 * @file messages.h
 * @brief This file contains all the message strings used in the Penguins the Game application.
 * It centralizes message definitions for easy management and localization.
 */

#ifndef PENGUINS_THE_GAME_MESSAGES_H
#define PENGUINS_THE_GAME_MESSAGES_H

#define TEXT_BUFFER_SIZE 128

// General Messages
#define MSG_WELCOME "Welcome to Penguins the Game!\n"
#define MSG_INITIALIZING "Initializing...\n"
#define MSG_INITIALIZED "Initialized!\n"

// Game Setup Messages
#define MSG_ENTER_NEW_OR_LOAD_GAME "Enter '%d' to run New Game or '%d' to Load Game:"
#define MSG_ENTER_TRY_AGAIN_OR_NEW_SETTINGS "Initialization failed.\nEnter '%d' to Re-generate board with same settings or '%d' to Change settings:"
#define MSG_GAME_OPTION_DOES_NOT_EXIST "Given value does not correspond to any game option. Try again.\n"

#define MSG_ENTER_BOARD_WIDTH "Enter board width (%d-%d):"
#define MSG_ENTER_BOARD_HEIGHT "Enter board height (%d-%d):"

#define MSG_ENTER_NUMBER_OF_PLAYERS "Enter number of players (%d-%d):"
#define MSG_ENTER_NUMBERS_OF_PENGUINS "Enter number of penguins per player (%d-%d):"

#define MSG_ENTER_PLAYER_TYPE "Is Player %d human (%d) or bot (%d):"

// New/Load Game Messages
#define MSG_NEW_GAME "Starting a new game...\n"
#define MSG_LOAD_GAME "Loading a saved game...\n"

#define MSG_GAME_LOADED "Game loaded successfully.\n"
#define MSG_GAME_LOAD_FAILED "Failed to load the game.\n"

// Game Running Messages
#define MSG_TRYING_TO_RUN "Trying to run...\n"
#define MSG_LOADED_GAME "Resuming loaded game...\n"
#define MSG_GAME_RUNNING "Game running...\n"

// Turn Messages
#define MSG_ROUND "\n===== ROUND %d =====\n"
#define MSG_PLAYER_TURN "Player %d's turn.\n"
#define MSG_PLAYER_POINTS "Player %d has %d fish.\n"
#define MSG_PLAYER_POINTS_BOT "Player %d (bot) has %d fish.\n"
#define MSG_AFTER_POSITION_UPDATE "Current Player placed his penguin on (%d,%d) and obtained %d fish. Now has %d fish.\n"
#define MSG_AFTER_POSITION_UPDATE_BOT "Current Player (bot) placed his penguin on (%d,%d) and obtained %d fish. Now has %d fish.\n"
#define MSG_AFTER_MOVE_UPDATE "Current Player moved his penguin from (%d,%d) to (%d,%d) and obtained %d fish. Now has %d fish.\n"
#define MSG_AFTER_MOVE_UPDATE_BOT "Current Player (bot) moved his penguin from (%d,%d) to (%d,%d) and obtained %d fish. Now has %d fish.\n"

// Placement Phase Messages
#define MSG_PLACEMENT_PHASE "Placement Phase\n"
#define MSG_PLACEMENT_PHASE_FINISHED "Placement Phase finished successfully!\n"

#define MSG_ENTER_COORDINATES "Put your x y coordinates:"

// Movement Phase Messages
#define MSG_MOVEMENT_PHASE "Movement Phase\n"
#define MSG_MOVEMENT_PHASE_FINISHED "Movement Phase finished successfully!\n"

#define MSG_CHOOSE_PENGUIN "Choose penguin to move (x y):"
#define MSG_CHOOSE_DESTINATION "Choose destination (x y):"
#define MSG_MOVE_SUCCESSFUL "Move successful!\n"
#define MSG_INVALID_MOVE "Invalid move. Try again.\n"
#define MSG_INVALID_PENGUIN "Invalid penguin.\n"
#define MSG_PLAYER_NO_AVAILABLE_MOVES "Player %d has no available moves, skipping...\n"
#define MSG_PLAYER_NO_AVAILABLE_MOVES_BOT "Player %d (bot) has no available moves, skipping...\n"
#define MSG_ALL_PLAYERS_NO_AVAILABLE_MOVES "\nNo players have any moves left. Game ends!\n"

// Game End Messages
#define MSG_FINAL_SCORES "\n ==================\n \n===== FINAL SCORES =====\n \n ==================\n"
#define MSG_WINNER "Player %d wins with %d fish! Congratulations!\n ==================\n\n"
#define MSG_WINNER_BOT "Player %d (bot) wins with %d fish! Congratulations!\n ==================\n\n"

#define MSG_GAME_FINISHED "Game finished, cleaning...\n"
#define MSG_GAME_CLEANED "Game cleaned!\n"
#define MSG_PLAY_AGAIN "Do you want to play again? (print 'y' to continue, else program finishes)\n"
#define MSG_GAME_CLOSED "Game Closed!\n"

// Save/Load Messages
#define MSG_GAME_SAVED "Game saved successfully.\n"
#define MSG_GAME_SAVE_FAILED "Failed to save the game.\n"

// Error Messages
#define MSG_INVALID_INPUT_TYPE "Invalid input! Please enter an integer.\n"
#define MSG_INPUT_OUT_OF_RANGE "Input out of range! Please enter a value between %d and %d.\n"
#define MSG_COORDINATES_OUT_OF_BOUNDS "Coordinates out of bounds! Please enter values between (%d,%d) and (%d,%d).\n"
#define MSG_INVALID_FORMAT "Invalid format! Please enter 'x y' (e.g., 2 3).\n"
#define MSG_NOT_ENOUGH_TILES "The game generated with not enough tiles with 1 fish, game finished.\n"
#define MSG_TILE_NOT_AVAILABLE "The selected tile is not available for placement. Please choose another tile.(%d, %d)\n"

#define MSG_MEMORY_ALLOCATION_FAILED "Memory allocation failed!\n"

#endif