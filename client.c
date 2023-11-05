#include <stdio.h>
#include <winsock2.h>

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    int send_size;
    char message[1024];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);

    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed. Error");
        return 1;
    }

    strcpy(message, "Hello, server!");
    send_size = send(client_socket, message, strlen(message), 0);
    if (send_size == SOCKET_ERROR) {
        printf("Send failed : %d", WSAGetLastError());
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
