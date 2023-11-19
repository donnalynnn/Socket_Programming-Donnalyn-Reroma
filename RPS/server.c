#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>  // Include for srand and rand
#pragma comment(lib, "ws2_32.lib")

#define PORT 5555

// Enumeration for game choices
enum GameChoice {
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3
};

// Structure to hold the game state
struct GameState {
    int player1_choice;
    int player2_choice;
};

// Function to determine the game result
int determine_game_result(int choice1, int choice2) {
    // Your game logic goes here
    // Return 0 for a draw, 1 for player 1 wins, 2 for player 2 wins
    if (choice1 == choice2) {
        return 0; // Draw
    } else if ((choice1 == ROCK && choice2 == SCISSORS) ||
               (choice1 == PAPER && choice2 == ROCK) ||
               (choice1 == SCISSORS && choice2 == PAPER)) {
        return 1; // Player 1 wins
    } else {
        return 2; // Player 2 wins
    }
}

// Function to generate a random choice for the server
int generate_server_choice() {
    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Generate a random number between 1 and 3
    return (rand() % 3) + 1;
}

// Function declaration
void handle_client(SOCKET client_socket, int player_number);

// Rest of the code remains the same...

void handle_client(SOCKET client_socket, int player_number) {
    char buffer[1024];
    struct GameState game_state;

    // Game loop
    while (1) {
        // Inform players to make choices
        send(client_socket, "Make your choice (rock=1/paper=2/scissors=3, enter '0' to exit):", sizeof(buffer), 0);

        // Get the player's choice
        recv(client_socket, buffer, sizeof(buffer), 0);
        game_state.player1_choice = atoi(buffer);

        // Check if '0' is entered to exit the loop
        if (game_state.player1_choice == 0) {
            break;
        }

        // Acknowledge the client's choice
        send(client_socket, "Choice received. Waiting for the opponent...", sizeof(buffer), 0);

        // Inform the client that the server is making a choice
        send(client_socket, "Server is making a choice...", sizeof(buffer), 0);

        // Generate the server's choice
        game_state.player2_choice = generate_server_choice();

        // Send the server's choice to the client
        sprintf(buffer, "Server's choice: %d", game_state.player2_choice);
        send(client_socket, buffer, sizeof(buffer), 0);

        // Process game logic and determine the result
        int result = determine_game_result(game_state.player1_choice, game_state.player2_choice);

        // Send the result to both players
        if (result == 0) {
            send(client_socket, "It's a draw!", sizeof(buffer), 0);
        } else if (result == 1) {
            send(client_socket, "You win!", sizeof(buffer), 0);
        } else {
            send(client_socket, "Server wins!", sizeof(buffer), 0);
        }

        // Allow the server to input its choice
        send(client_socket, "Do you want the server to play? (1 for Yes, 0 for No):", sizeof(buffer), 0);

        // Receive client's choice about the server playing
        int play_as_server;
        recv(client_socket, buffer, sizeof(buffer), 0);
        play_as_server = atoi(buffer);

        if (play_as_server == 1) {
            // Inform the client that the server is playing
            send(client_socket, "Server is playing.", sizeof(buffer), 0);

            // Send the server's choices
            send(client_socket, "Server's choices:", sizeof(buffer), 0);

            // Generate the server's choice
            game_state.player2_choice = generate_server_choice();

            // Send the server's choice to the client
            sprintf(buffer, "Server's choice: %d", game_state.player2_choice);
            send(client_socket, buffer, sizeof(buffer), 0);

            // Process game logic and determine the result
            result = determine_game_result(game_state.player1_choice, game_state.player2_choice);

            // Send the result to both players
            if (result == 0) {
                send(client_socket, "It's a draw!", sizeof(buffer), 0);
            } else if (result == 1) {
                send(client_socket, "You win!", sizeof(buffer), 0);
            } else {
                send(client_socket, "Server wins!", sizeof(buffer), 0);
            }
        }
    }

    // Close the connection
    closesocket(client_socket);
}
