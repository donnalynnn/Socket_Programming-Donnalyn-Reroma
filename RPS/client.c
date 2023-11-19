// client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 5555

void play_game(SOCKET client_socket) {
    char buffer[1024];

    // Game loop
    while (1) {
        // Receive instructions and make choices
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);

        // Get the player's choice
        char player_choice[1024];
        printf("Your choice (rock=1/paper=2/scissors=3, enter '0' to exit): ");
        fgets(player_choice, sizeof(player_choice), stdin);
        send(client_socket, player_choice, sizeof(player_choice), 0);

        // Receive and print acknowledgment
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);

        // Check if '0' is entered to exit the loop
        if (player_choice[0] == '0') {
            break;
        }

        // Get the opponent's choice
        printf("Opponent's choice: ");
        fgets(player_choice, sizeof(player_choice), stdin);
        send(client_socket, player_choice, sizeof(player_choice), 0);

        // Receive and print the result
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);
    }

    // Close the connection
    closesocket(client_socket);
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("WSAStartup failed");
        return EXIT_FAILURE;
    }

    SOCKET client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Connection failed");
        return EXIT_FAILURE;
    }

    // Play the game
    play_game(client_socket);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
