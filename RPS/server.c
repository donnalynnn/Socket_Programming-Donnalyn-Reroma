// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <arpa/inet.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include "rps.c"

#define PORT 5555

void handle_client(int client_socket, int player_number) {
    char buffer[1024];
    const char* choices[] = {"rock", "paper", "scissors"};

    // Send instructions to the client
    send(client_socket, "Welcome! Choose rock, paper, or scissors.", sizeof("Welcome! Choose rock, paper, or scissors."), 0);

    // Receive the player's choice
    recv(client_socket, buffer, sizeof(buffer), 0);
    char player_choice[1024];
    strcpy(player_choice, buffer);

    // Validate the choice
    int valid_choice = 0;
    for (int i = 0; i < 3; ++i) {
        if (strcmp(player_choice, choices[i]) == 0) {
            valid_choice = 1;
            break;
        }
    }

    if (!valid_choice) {
        send(client_socket, "Invalid choice. Please choose rock, paper, or scissors.", sizeof("Invalid choice. Please choose rock, paper, or scissors."), 0);
        closesocket(client_socket);
        return;
    }

    // Notify the player about their choice
    send(client_socket, "Waiting for the opponent...", sizeof("Waiting for the opponent..."), 0);

    // Receive the opponent's choice
    recv(client_socket, buffer, sizeof(buffer), 0);
    char opponent_choice[1024];
    strcpy(opponent_choice, buffer);

    // Validate the opponent's choice
    valid_choice = 0;
    for (int i = 0; i < 3; ++i) {
        if (strcmp(opponent_choice, choices[i]) == 0) {
            valid_choice = 1;
            break;
        }
    }

    if (!valid_choice) {
        send(client_socket, "Invalid opponent choice. Closing connection.", sizeof("Invalid opponent choice. Closing connection."), 0);
        closesocket(client_socket);
        return;
    }

    // Determine the winner and send the result to the player
    const char* result = determine_winner(player_choice, opponent_choice);
    send(client_socket, result, sizeof(buffer), 0);

    // Close the connection
    closesocket(client_socket);
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("WSAStartup failed");
        return EXIT_FAILURE;
    }

    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Use INADDR_ANY to bind to all available interfaces

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Binding failed");
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(server_socket, 2) == SOCKET_ERROR) {
        perror("Listening failed");
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d\n", PORT);

    int player_number = 1;

    while (1) {
        // Accept a connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        printf("Connection accepted for player %d\n", player_number);

        // Handle the client in a separate function
        handle_client(client_socket, player_number);

        player_number++;
    }


    // Cleanup Winsock
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
