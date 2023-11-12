// client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <windows.h>

#define PORT 5555

void play_game() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Receive and print instructions
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    // Get the player's choice
    char player_choice[1024];
    printf("Your choice (rock/paper/scissors): ");
    scanf("%s", player_choice);
    send(client_socket, player_choice, sizeof(player_choice), 0);

    // Receive and print the acknowledgment
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    // Get the opponent's choice
    char opponent_choice[1024];
    printf("Opponent's choice: ");
    scanf("%s", opponent_choice);
    send(client_socket, opponent_choice, sizeof(opponent_choice), 0);

    // Receive and print the result
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    // Close the connection
    close(client_socket);
}

int main() {
    play_game();
    return 0;
}
