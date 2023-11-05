#include <stdio.h>
#include <winsock2.h>

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int recv_size;
    char buffer[1024];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(12345);

    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }

    listen(server_socket, 5);

    printf("Server listening...\n");

    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed with error code : %d", WSAGetLastError());
    }

    recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
    if (recv_size == SOCKET_ERROR) {
        printf("Receive failed with error code : %d", WSAGetLastError());
    }

    printf("Received: %s\n", buffer);

    const char *response = "Message received!";
    send(client_socket, response, strlen(response), 0);

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
